#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <QFile>
#include <QSettings>
#include <QString>
#include <QtWidgets>

#include "customwidgets.h"

namespace S21 {

// Класс предназначен для сохранения и загрузки настроек главного окна
// Принимает указатель на окно, настройки виджетов которого надо отслеживать
// AddWidget... Добавляет имя виджета в отслеживание
// Универсален, но для использования на неуказанных в SetValue/GetValue виджетах
// необходимо дописать метод установки/получения значения

class Configurator {
 public:
  explicit Configurator(QMainWindow* window) : window_(window) {}
  ~Configurator() {}

  void AddWidgetToConfigurate(const QString& widget_name);

  void Load();

  void Upload();

  // Сохранить текущие отслеживаемые настройки как дефолтные
  void SaveDefaultConfig();

  // Загрузка дефолтных настроек
  void LoadDefault();

 private:
  QMainWindow* window_;
  QStringList widget_names_{};
  QString config_name_ = QDir::currentPath() + "/configurate.ini";
  QString def_config_name_ = QDir::currentPath() + "/def.ini";

  void GenerateDefaultIni();
  void ApplySetting(QSettings& settings, QWidget* widget);
  void SaveSetting(QSettings& settings, QWidget* widget);

  const QString GetValue(QWidget* widget) const;

  bool SetValue(QWidget* widget, const QString value);
};

}  // namespace S21
#endif  // CONFIGURATOR_H
