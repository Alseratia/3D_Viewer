#ifndef SETTINGMANAGER_H
#define SETTINGMANAGER_H

#include <QColor>
#include <QObject>
#include <QOpenGLWidget>
#include <memory>

namespace S21 {

class Command {
 public:
  virtual ~Command() {}
  virtual void Set() = 0;
};

class ProectionType : public Command {};
class LineType : public Command {};
class DotType : public Command {};

class CentralProection : public ProectionType {
 public:
  void Set() override;
};
class ParallelProection : public ProectionType {
 public:
  void Set() override;
};

class SmoothDot : public DotType {
  void Set() override;
};
class SquareDot : public DotType {
  void Set() override;
};

class SolidLine : public LineType {
  void Set() override;
};
class DottedLine : public LineType {
  void Set() override;
};

// Класс, хранящий и устанавливающий настройки отрисовки. Методы Set..
// устанавливают нужный класс-команду. Методы Select Вызывают установку настроек
class SettingManager : public QObject {
  Q_OBJECT
 public:
  SettingManager();
  ~SettingManager() {}

  bool NoLine() const { return !line_type_; }
  bool NoDot() const { return !dot_type_; }

  void SelectBackground() const;
  void SelectLineSettings() const;
  void SelectDotSettings() const;
  void SelectProection() const;

 public slots:
  void SetProection(const QString& type);
  void SetLineType(const QString& type);
  void SetDotType(const QString& type);
  void SetBackground(const QColor& color);
  void SetLineColor(const QColor& color);
  void SetDotColor(const QColor& color);
  void SetLineWidth(const double value);
  void SetDotWidth(const double value);

 private:
  std::unique_ptr<ProectionType> proection_;
  std::unique_ptr<LineType> line_type_;
  std::unique_ptr<DotType> dot_type_;
  QColor background{}, line_color_{}, dot_color_{};
  double line_width_{}, dot_width_{};

 signals:
  void SettingsUpdate();
};  // class SettingManager

}  // namespace S21

#endif  // SETTINGMANAGER_H
