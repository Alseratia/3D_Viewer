#include "drawwindow.h"

namespace S21 {

DrawWindow::DrawWindow(QWidget* parent) : QOpenGLWidget(parent) {
  //передача сигнала из главного окна дальше в менеджер
  connect(this, SIGNAL(SetBackground(QColor)), &store_,
          SLOT(SetBackground(QColor)));
  connect(this, SIGNAL(SetLineColor(QColor)), &store_,
          SLOT(SetLineColor(QColor)));
  connect(this, SIGNAL(SetDotColor(QColor)), &store_,
          SLOT(SetDotColor(QColor)));
  connect(this, SIGNAL(SetLineWidth(double)), &store_,
          SLOT(SetLineWidth(double)));
  connect(this, SIGNAL(SetDotWidth(double)), &store_,
          SLOT(SetDotWidth(double)));
  connect(this, SIGNAL(SetProection(QString)), &store_,
          SLOT(SetProection(QString)));
  connect(this, SIGNAL(SetLineType(QString)), &store_,
          SLOT(SetLineType(QString)));
  connect(this, SIGNAL(SetDotType(QString)), &store_,
          SLOT(SetDotType(QString)));
  connect(&store_, SIGNAL(SettingsUpdate()), this, SLOT(UpdateWindow()));
}

void DrawWindow::initializeGL() {
  glEnable(GL_DEPTH_TEST);  // буфер глубины
  glEnable(GL_BLEND);  // для правильной работы сглаживания на разных машинах
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void DrawWindow::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void DrawWindow::paintGL() {
  store_.SelectBackground();
  store_.SelectProection();

  if (!controller_) return;
  glEnableClientState(GL_VERTEX_ARRAY);
  PaintVertex();
  PaintEdges();
  glDisableClientState(GL_VERTEX_ARRAY);
}

void DrawWindow::PaintVertex() {
  if (store_.NoDot()) return;

  auto array = controller_->GetVertexes();
  store_.SelectDotSettings();
  glVertexPointer(3, GL_FLOAT, sizeof(QVector3D), array.data());
  glDrawArrays(GL_POINTS, 0, array.size());
}

void DrawWindow::PaintEdges() {
  if (store_.NoLine()) return;

  AdapterLines array(controller_->GetEdges(), controller_->GetVertexes());
  store_.SelectLineSettings();

  glVertexPointer(3, GL_FLOAT, 0, array.data());
  glDrawArrays(GL_LINES, 0, array.size() / 3);
}

AdapterLines::AdapterLines(const Model::edges_array& edges,
                           const Model::vertex_array& vertex)
    : std::vector<float>() {
  for (auto& value : edges) {
    this->push_back(vertex[value.first].x());
    this->push_back(vertex[value.first].y());
    this->push_back(vertex[value.first].z());
    this->push_back(vertex[value.second].x());
    this->push_back(vertex[value.second].y());
    this->push_back(vertex[value.second].z());
  }
}

}  // namespace S21
