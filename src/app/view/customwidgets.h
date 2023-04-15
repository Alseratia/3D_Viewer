#ifndef CUSTOMWIDGETS_H
#define CUSTOMWIDGETS_H

#include <QColor>
#include <QColorDialog>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>

namespace S21 {

// Кнопка выбора цвета. При клике открывает меню.
// При выборе цвета - испускает сигнал смены цвета
class QColorButton : public QPushButton {
  Q_OBJECT
 public:
  QColorButton(QWidget* parent = nullptr) : QPushButton(parent) {
    connect(this, SIGNAL(clicked(bool)), this, SLOT(ColorClicked()));
  }

  void SetColor(const QColor& color) {
    QString color_string = color.name(QColor::HexRgb);
    SetColor(color_string);
  }

  void SetColor(const QString& color) {
    this->setStyleSheet("background-color: " + color);
    emit colorChanged(color);
  }

 private slots:
  void ColorClicked() {
    QColor color = QColorDialog::getColor();
    if (color.isValid()) SetColor(color);
  }

 signals:
  // Сигнал для изменения цвета кнопки
  void colorChanged(const QColor& color);

};  // QColorButton

// Класс реализующий Slider и связывает с ним spinbox, в которых автоматически
// обновляются значения при смене значения второго и наоборот
class FlexSlider : public QSlider {
  Q_OBJECT
 public:
  FlexSlider(QWidget* parent = nullptr) : QSlider(parent) {}
  double GetValue() const { return value(); }
  void BindSpinBox(QSpinBox* box) {
    if (!box) return;
    box_ = box;
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(SetBoxValue(int)));
    connect(box_, SIGNAL(editingFinished()), this, SLOT(SlotSetValue()));
  }
 private slots:
  void SlotSetValue() {
    if (box_ != nullptr) setValue(box_->value());
  }
  void SetBoxValue(const int value) {
    if (box_ != nullptr) {
      box_->blockSignals(true);
      box_->setValue(value);
      box_->blockSignals(false);
    }
  }

 private:
  QSpinBox* box_ = nullptr;

};  // FlexSlider

}  // namespace S21
#endif  // CUSTOMWIDGETS_H
