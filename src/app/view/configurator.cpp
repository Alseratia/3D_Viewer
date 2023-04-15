#include "configurator.h"

namespace S21 {

void Configurator::AddWidgetToConfigurate(const QString& widget_name) {
  if (widget_name.isEmpty()) return;
  widget_names_.push_back(widget_name);
}

void Configurator::SaveDefaultConfig() {
  QSettings settings(def_config_name_, QSettings::IniFormat);
  SaveSetting(settings, window_->centralWidget());
}

void Configurator::Load() {
  QFile file(config_name_);
  if (file.exists()) {
    QSettings settings(config_name_, QSettings::IniFormat);
    ApplySetting(settings, window_->centralWidget());
  }
}

void Configurator::ApplySetting(QSettings& settings, QWidget* widget) {
  QString widgetName = widget->objectName();

  if (settings.contains(widgetName)) {
    QVariant settingValue = settings.value(widgetName);
    if (settingValue.isValid()) {
      SetValue(widget, settingValue.toString());
    }
  }
  // рекурсивно перебирает все виджеты в окне,
  // если название виджета отслеживается, устанавливает значение
  foreach (QObject* child, widget->children()) {
    if (QWidget* childWidget = qobject_cast<QWidget*>(child)) {
      ApplySetting(settings, childWidget);
    }
  }
}

void Configurator::Upload() {
  QSettings settings(config_name_, QSettings::IniFormat);
  SaveSetting(settings, window_->centralWidget());
}

void Configurator::SaveSetting(QSettings& settings, QWidget* widget) {
  QString widgetName = widget->objectName();

  if (widget_names_.contains(widgetName)) {
    settings.setValue(widgetName, GetValue(widget));
  }

  foreach (QObject* child, widget->children()) {
    if (QWidget* childWidget = qobject_cast<QWidget*>(child)) {
      SaveSetting(settings, childWidget);
    }
  }
}

void Configurator::LoadDefault() {
  QFile file(def_config_name_);
  if (file.exists()) {
    QSettings settings(def_config_name_, QSettings::IniFormat);
    ApplySetting(settings, window_->centralWidget());
  }
}

const QString Configurator::GetValue(QWidget* widget) const {
  if (QColorButton* button = qobject_cast<QColorButton*>(widget)) {
    return button->palette().color(QPalette::Button).name();
  }
  if (QSlider* slider = qobject_cast<QSlider*>(widget)) {
    return QString::number(slider->value());
  }
  if (QComboBox* box = qobject_cast<QComboBox*>(widget)) {
    return box->currentText();
  }
  if (QDoubleSpinBox* box = qobject_cast<QDoubleSpinBox*>(widget)) {
    return QString::number(box->value());
  }
  if (QSpinBox* box = qobject_cast<QSpinBox*>(widget)) {
    return QString::number(box->value());
  }
  return "0";
}

bool Configurator::SetValue(QWidget* widget, const QString value) {
  if (QColorButton* button = qobject_cast<QColorButton*>(widget)) {
    button->SetColor(value);
    return true;
  }
  if (FlexSlider* slider = qobject_cast<FlexSlider*>(widget)) {
    slider->setValue(value.toInt());
    return true;
  }
  if (QComboBox* box = qobject_cast<QComboBox*>(widget)) {
    box->setCurrentText(value);
    return true;
  }
  if (QDoubleSpinBox* box = qobject_cast<QDoubleSpinBox*>(widget)) {
    box->setValue(value.toDouble());
    return true;
  }
  if (QSpinBox* box = qobject_cast<QSpinBox*>(widget)) {
    box->setValue(value.toInt());
    return true;
  }
  return false;
}

}  // namespace S21
