#include "mainwindow.h"

#include <QDebug>

#include "ui_mainwindow.h"

namespace S21 {

MainWindow::MainWindow(Controller* c)
    : QMainWindow(nullptr),
      ui_(new Ui::MainWindow),
      controller_(c),
      conf(this) {
  ui_->setupUi(this);
  ConfigurateWindow();
}

MainWindow::~MainWindow() { conf.Upload(); }

void MainWindow::ConfigurateWindow() {
  BindSignals();
  SettingsToCustomWidgets();

  this->show();

  BindSettings();
  conf.SaveDefaultConfig();
  conf.Load();
}
void MainWindow::SettingsToCustomWidgets() {
  ui_->GL->SetController(controller_);
  ui_->XRotation->BindSpinBox(ui_->LXRotation);
  ui_->YRotation->BindSpinBox(ui_->LYRotation);
  ui_->ZRotation->BindSpinBox(ui_->LZRotation);
  ui_->Scale->BindSpinBox(ui_->LScale);
  ui_->progressBar->hide();
  ui_->LineColor->SetColor(Qt::white);
  ui_->DotColor->SetColor(Qt::white);
  ui_->Background->SetColor(Qt::darkGray);
}

void MainWindow::BindSignals() {
  // отрисовка если модель изменилась
  connect(controller_, SIGNAL(ModelChanged()), ui_->GL, SLOT(UpdateWindow()));

  // таймер для сохранения в гифку
  connect(timer_.get(), SIGNAL(timeout()), this, SLOT(RecordOneFrame()));

  // изменение настроек отрисовки
  connect(ui_->Background, SIGNAL(colorChanged(QColor)), ui_->GL,
          SIGNAL(SetBackground(QColor)));
  connect(ui_->LineColor, SIGNAL(colorChanged(QColor)), ui_->GL,
          SIGNAL(SetLineColor(QColor)));
  connect(ui_->DotColor, SIGNAL(colorChanged(QColor)), ui_->GL,
          SIGNAL(SetDotColor(QColor)));
  connect(ui_->LineWidth, SIGNAL(valueChanged(double)), ui_->GL,
          SIGNAL(SetLineWidth(double)));
  connect(ui_->DotWidth, SIGNAL(valueChanged(double)), ui_->GL,
          SIGNAL(SetDotWidth(double)));
  connect(ui_->ProectionType, SIGNAL(currentTextChanged(QString)), ui_->GL,
          SIGNAL(SetProection(QString)));
  connect(ui_->LineType, SIGNAL(currentTextChanged(QString)), ui_->GL,
          SIGNAL(SetLineType(QString)));
  connect(ui_->DotType, SIGNAL(currentTextChanged(QString)), ui_->GL,
          SIGNAL(SetDotType(QString)));

  // изменение модели
  connect(ui_->XRotation, SIGNAL(valueChanged(int)), this, SLOT(RotateModel()));
  connect(ui_->YRotation, SIGNAL(valueChanged(int)), this, SLOT(RotateModel()));
  connect(ui_->ZRotation, SIGNAL(valueChanged(int)), this, SLOT(RotateModel()));
  connect(ui_->Scale, SIGNAL(valueChanged(int)), this, SLOT(ScaleModel()));
  connect(ui_->XMove, SIGNAL(valueChanged(int)), this, SLOT(MoveModel()));
  connect(ui_->YMove, SIGNAL(valueChanged(int)), this, SLOT(MoveModel()));
  connect(ui_->ZMove, SIGNAL(valueChanged(int)), this, SLOT(MoveModel()));
}

void MainWindow::BindSettings() {
  conf.AddWidgetToConfigurate("XRotation");
  conf.AddWidgetToConfigurate("YRotation");
  conf.AddWidgetToConfigurate("ZRotation");
  conf.AddWidgetToConfigurate("Scale");
  conf.AddWidgetToConfigurate("XMove");
  conf.AddWidgetToConfigurate("YMove");
  conf.AddWidgetToConfigurate("ZMove");
  conf.AddWidgetToConfigurate("LineType");
  conf.AddWidgetToConfigurate("LineColor");
  conf.AddWidgetToConfigurate("LineWidth");
  conf.AddWidgetToConfigurate("DotType");
  conf.AddWidgetToConfigurate("DotColor");
  conf.AddWidgetToConfigurate("DotWidth");
  conf.AddWidgetToConfigurate("ProectionType");
  conf.AddWidgetToConfigurate("Background");
}

const QString MainWindow::FileAction(ActionType action) {
  QString file_name;
  if (action == SAVE) {
    file_name = QFileDialog::getExistingDirectory(
        this, QString("Сохранить в"), QString(), QFileDialog::ShowDirsOnly);
  }
  if (action == LOAD) {
    file_name =
        QFileDialog::getOpenFileName(this, tr("Открыть файл"), QString(),
                                     QString("Все объектные файлы (*.obj)"));
  }
  return file_name;
}

void MainWindow::on_OpenFile_activated(int index) {
  QComboBox* obj = ui_->OpenFile;
  if (index != 0) {
    CreateModel(obj->currentText());
    return;
  }
  QString fileName = FileAction(LOAD);
  if (fileName.isEmpty()) return;

  AddItemAndSet(obj, fileName);
  CreateModel(obj->currentText());
}

void MainWindow::on_SaveFile_activated(int index) {
  if (index != 0) return;

  QComboBox* obj = ui_->SaveFile;
  ;
  QString fileName = FileAction(SAVE);
  if (fileName.isEmpty()) return;
  AddItemAndSet(obj, fileName);
}

void MainWindow::AddItemAndSet(QComboBox* box, QString& item) {
  int index = box->count();
  box->addItem(item);
  box->setCurrentIndex(index);
}

void MainWindow::CreateModel(QString path) {
  try {
    controller_->OpenFile(path);
  } catch (std::logic_error& err) {
    QMessageBox::information(0, "Ошибка открытия файла", err.what());
  }
  ui_->Vertices->setText(QString::number(controller_->GetCountVertex()));
  ui_->Edges->setText(QString::number(controller_->GetCountEdges()));
}

void MainWindow::on_ButtonSave_clicked() {
  if (ui_->SaveFile->currentIndex() == 0) {
    QMessageBox::information(0, "Сохранение", "Не выбрана папка");
    return;
  }
  QString path = ui_->SaveFile->currentText();
  QString format = ui_->RadioGroup->checkedButton()->text();

  if (format == "gif") {
    SaveInGif();
  } else {
    SaveInImage(path, format);
  }
}

void MainWindow::SaveInImage(const QString& path, const QString& format) {
  bool isSuccess = ui_->GL->grab().save(path + "/image." + format, 0, 100);
  SaveMessage(isSuccess);
}

void MainWindow::SaveInGif() {
  ui_->SaveFile->setEnabled(false);
  ui_->progressBar->setValue(0);
  shots_ = 0;
  ui_->progressBar->show();
  gif_ = std::make_unique<QGifImage>();
  timer_->start(100);
}

void MainWindow::RecordOneFrame() {
  if (shots_ < 50) {
    QImage frame = ui_->GL->grabFramebuffer();
    ui_->GL->grab();
    QImage scaled_frame = frame.scaled(640, 480, Qt::IgnoreAspectRatio);
    gif_->addFrame(scaled_frame, 100);
    ui_->progressBar->setValue(shots_);
  } else {
    timer_->stop();
    bool isSuccess = gif_->save(ui_->SaveFile->currentText() + "/image.gif");
    ui_->SaveFile->setEnabled(true);
    gif_.reset();
    SaveMessage(isSuccess);
    ui_->progressBar->hide();
  }
  shots_++;
}

void MainWindow::SaveMessage(bool isSuccess) {
  if (isSuccess) {
    QMessageBox::information(0, "Сохранение", "Файл успешно сохранен");
  } else {
    QMessageBox::information(0, "Сохранение", "Не удалось сохранить файл");
  }
}

void MainWindow::on_ButtonUp_clicked() {
  ui_->YMove->triggerAction(QAbstractSlider::SliderSingleStepAdd);
}

void MainWindow::on_ButtonDown_clicked() {
  ui_->YMove->triggerAction(QAbstractSlider::SliderSingleStepSub);
}

void MainWindow::on_ButtonRight_clicked() {
  ui_->XMove->triggerAction(QAbstractSlider::SliderSingleStepAdd);
}

void MainWindow::on_ButtonLeft_clicked() {
  ui_->XMove->triggerAction(QAbstractSlider::SliderSingleStepSub);
}

void MainWindow::on_ButtonReset_clicked() {
  conf.LoadDefault();
  CreateModel(ui_->OpenFile->currentText());
}

void MainWindow::RotateModel() {
  controller_->Rotate(ui_->XRotation->value(), ui_->YRotation->value(),
                      ui_->ZRotation->value());
}

void MainWindow::MoveModel() {
  controller_->Move(ui_->XMove->value(), ui_->YMove->value(),
                    ui_->ZMove->value());
}

void MainWindow::ScaleModel() { controller_->Scale(ui_->Scale->GetValue()); }

}  // namespace S21
