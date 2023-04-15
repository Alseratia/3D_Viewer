#ifndef DRAWWINDOW_H
#define DRAWWINDOW_H

#include <QColor>
#include <QObject>
#include <QOpenGLWidget>
#include <memory>

#include "../controller/controller.h"
#include "settingmanager.h"

namespace S21 {

class DrawWindow : public QOpenGLWidget {
  Q_OBJECT
 public:
  DrawWindow(QWidget* parent = nullptr);
  ~DrawWindow() {}
  void SetController(Controller* c) { controller_ = c; }

 private:
  Controller* controller_{};
  QColor background{}, line_color_{}, dot_color_{};
  double line_width_{}, dot_width_{};
  SettingManager store_;

  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void PaintVertex();
  void PaintEdges();

 private slots:
  void UpdateWindow() { update(); }

 signals:
  void SetBackground(const QColor& color);
  void SetLineColor(const QColor& color);
  void SetDotColor(const QColor& color);
  void SetLineWidth(const double value);
  void SetDotWidth(const double value);
  void SetProection(const QString& type);
  void SetLineType(const QString& type);
  void SetDotType(const QString& type);
};  // DrawWindow

// Адаптер, преобразующий дерево в массив
class AdapterLines : public std::vector<float> {
 public:
  AdapterLines(const Model::edges_array& edges,
               const Model::vertex_array& vertex);
};

}  // namespace S21
#endif  // DRAWWINDOW_H
