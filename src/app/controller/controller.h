#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

#include "../model/model.h"

namespace S21 {

class Controller : public QObject {
  Q_OBJECT
 public:
  Controller() {}
  ~Controller() {}

 public:
  qsizetype GetCountVertex() const { return model.GetCountVertex(); }

  qsizetype GetCountEdges() const { return model.GetCountEdges(); }

  const Model::vertex_array& GetVertexes() const { return model.GetVertexes(); }

  const Model::edges_array& GetEdges() const { return model.GetEdges(); }

  void OpenFile(QString& file) {
    model.CreateInFile(file);
    emit ModelChanged();
  }

  void Rotate(const int x, const int y, const int z) {
    model.RotateTo(x, y, z);
    emit ModelChanged();
  }

  void Move(const int x, const int y, const int z) {
    model.MoveTo(x, y, z);
    emit ModelChanged();
  }

  void Scale(double a) {
    model.ScaleTo(a);
    emit ModelChanged();
  }

 private:
  Model& model = Model::Init();

 signals:
  void ModelChanged();
};

}  // namespace S21
#endif  // CONTROLLER_H
