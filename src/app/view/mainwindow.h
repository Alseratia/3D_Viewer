#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QComboBox>
#include <QFileDialog>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QPushButton>
#include <QRadioButton>

#include "../controller/controller.h"
#include "configurator.h"
#include "customwidgets.h"
#include "drawwindow.h"
#include "qgifimage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace S21 {

enum ActionType { LOAD, SAVE };

class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  explicit MainWindow(Controller* c);
  MainWindow(const MainWindow&) = delete;
  MainWindow& operator=(const MainWindow&) = delete;
  ~MainWindow();

 private slots:
  void on_OpenFile_activated(int index);
  void on_SaveFile_activated(int index);
  void on_ButtonSave_clicked();
  void on_ButtonUp_clicked();
  void on_ButtonDown_clicked();
  void on_ButtonRight_clicked();
  void on_ButtonLeft_clicked();
  void on_ButtonReset_clicked();
  void RecordOneFrame();
  void RotateModel();
  void MoveModel();
  void ScaleModel();

 private:
  std::unique_ptr<Ui::MainWindow> ui_;
  Controller* controller_;
  Configurator conf;

  std::unique_ptr<QTimer> timer_ = std::make_unique<QTimer>(this);
  std::unique_ptr<QGifImage> gif_;
  int shots_ = 0;

  void BindSignals();
  void BindSettings();
  void ConfigurateWindow();
  void SettingsToCustomWidgets();
  void CreateModel(QString path);
  const QString FileAction(ActionType action);
  void AddItemAndSet(QComboBox* box, QString& item);
  void SaveInGif();
  void SaveInImage(const QString& path, const QString& format);
  void SaveMessage(bool isSuccess);

 signals:
  void SetLineColor(const QColor& color);
};

}  // namespace S21
#endif  // MAINWINDOW_H
