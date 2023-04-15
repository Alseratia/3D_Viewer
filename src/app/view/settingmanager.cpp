#include "settingmanager.h"

namespace S21 {

void CentralProection::Set() {
  glLoadIdentity();
  glFrustum(-10, 10, -10, 10, 20, 1500);
  glTranslated(0, 10, -600);
}

void ParallelProection::Set() {
  glLoadIdentity();
  glOrtho(-300, 300, -200, 200, -2000, 2000);
}

void SmoothDot::Set() { glEnable(GL_POINT_SMOOTH); }

void SquareDot::Set() { glDisable(GL_POINT_SMOOTH); }

void SolidLine::Set() { glDisable(GL_LINE_STIPPLE); }

void DottedLine::Set() {
  glEnable(GL_LINE_STIPPLE);
  glLineStipple(1, 0x00FF);
}

SettingManager::SettingManager()
    : proection_{std::make_unique<ParallelProection>()},
      line_type_{std::make_unique<SolidLine>()},
      dot_type_{std::make_unique<SquareDot>()} {}

void SettingManager::SelectBackground() const {
  glClearColor(background.redF(), background.greenF(), background.blueF(),
               background.alphaF());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SettingManager::SelectLineSettings() const {
  glColor3d(line_color_.redF(), line_color_.greenF(), line_color_.blueF());
  glLineWidth(line_width_);
  if (line_type_) line_type_->Set();
}

void SettingManager::SelectDotSettings() const {
  glColor3d(dot_color_.redF(), dot_color_.greenF(), dot_color_.blueF());
  glPointSize(dot_width_);
  if (dot_type_) dot_type_->Set();
}

void SettingManager::SelectProection() const {
  if (proection_) {
    proection_->Set();
  }
}

void SettingManager::SetProection(const QString& type) {
  if (type == "Параллельная") {
    proection_ = std::make_unique<ParallelProection>();
  }
  if (type == "Центральная") {
    proection_ = std::make_unique<CentralProection>();
  }
  emit SettingsUpdate();
}

void SettingManager::SetLineType(const QString& type) {
  if (type == "Сплошная") {
    line_type_ = std::make_unique<SolidLine>();
  }
  if (type == "Пунктирная") {
    line_type_ = std::make_unique<DottedLine>();
  }
  if (type == "Отсутствует") {
    line_type_.reset();
  }
  emit SettingsUpdate();
}

void SettingManager::SetDotType(const QString& type) {
  if (type == "Квадрат") {
    dot_type_ = std::make_unique<SquareDot>();
  }
  if (type == "Круг") {
    dot_type_ = std::make_unique<SmoothDot>();
  }
  if (type == "Отсутствует") {
    dot_type_.reset();
  }
  emit SettingsUpdate();
}

void SettingManager::SetBackground(const QColor& color) {
  background = color;
  emit SettingsUpdate();
}

void SettingManager::SetLineColor(const QColor& color) {
  line_color_ = color;
  emit SettingsUpdate();
}

void SettingManager::SetDotColor(const QColor& color) {
  dot_color_ = color;
  emit SettingsUpdate();
}

void SettingManager::SetLineWidth(const double value) {
  line_width_ = value;
  emit SettingsUpdate();
}

void SettingManager::SetDotWidth(const double value) {
  dot_width_ = value;
  emit SettingsUpdate();
}

}  // namespace S21
