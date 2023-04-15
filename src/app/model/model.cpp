#include "model.h"

#include <QDebug>

namespace S21 {

Model& Model::Init() {
  static Model model;
  return model;
}

void Model::ParseFile(QFile& file) {
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    std::runtime_error("Can't open file");
  }
  try {
    while (!file.atEnd()) {
      QString str = file.readLine().simplified();
      ParseLine(str);
    }
    file.close();
    CheckAllVertexExist();

  } catch (const std::exception& ex) {
    ClearModel();
    if (file.isOpen()) file.close();
    throw std::logic_error(ex.what());
  }
}

void Model::ParseLine(const QString& str) {
  if (str.startsWith("v ")) {
    InsertVertex(str);
  }
  if (str.startsWith("f ")) {
    InsertEdge(str);
  }
}

void Model::CheckLineVertexFormat(const QStringList& list) {
  if (list.size() != 4 && list.size() != 5) {
    throw std::invalid_argument("Invalid edge in file");
  }
}

float Model::ToFloat(const QString& value) {
  bool is_float;
  float result = value.toFloat(&is_float);
  if (!is_float) throw std::invalid_argument("No float value in vertex");
  return result;
}

void Model::InsertVertex(const QString& str) {
  QStringList list = str.split(" ");
  CheckLineVertexFormat(list);
  vertexes_.push_back({ToFloat(list[1]), ToFloat(list[2]), ToFloat(list[3])});
}

void Model::InsertEdge(const QString& str) {
  QStringList list = str.split(" ");
  CheckLineEdgeFormat(list);

  qsizetype size = list.size();
  list[0] = list[size - 1].split(
      "/")[0];  // чтобы добавить ребро между первой и последней вершиной
  for (qsizetype i = 1; i < size; i++) {
    list[i] = list[i].split("/")[0];
    edges_.insert({ToUInt(list[i - 1]) - 1, ToUInt(list[i]) - 1});
  }
}

void Model::CheckLineEdgeFormat(const QStringList& list) {
  if (list.size() < 4) {
    throw std::invalid_argument("Invalid edge in file");
  }
}

qsizetype Model::ToUInt(const QString& value) {
  bool is_uint;
  qsizetype result = value.toUInt(&is_uint);
  if (!is_uint) throw std::invalid_argument("No u_int value in edge");
  return result;
}

void Model::CheckAllVertexExist() {
  qsizetype count_vertex = vertexes_.size();

  for (auto& edge : edges_) {
    if (edge.first >= count_vertex || edge.second >= count_vertex) {
      throw std::invalid_argument("Invalid edge in file");
    }
  }
}

void Model::CreateInFile(QString& pathToFile) {
  ClearModel();
  QFile file(pathToFile);
  ParseFile(file);

  RotateX(xRotation_);
  RotateY(yRotation_);
  RotateZ(zRotation_);
  Scale(scale_);
  Move(xMove_, yMove_, zMove_);
}

void Model::RotateTo(const int x_grad, const int y_grad, const int z_grad) {
  // перевод в радианы
  float x_rad = x_grad / 180.0 * M_PI;
  float y_rad = y_grad / 180.0 * M_PI;
  float z_rad = z_grad / 180.0 * M_PI;
  RotateTo(x_rad, y_rad, z_rad);
}

void Model::RotateTo(const float x_rad, const float y_rad, const float z_rad) {
  // вычисление на сколько надо повернуть относительно уже выполненного поворота
  float angle;
  if (x_rad != xRotation_) {
    angle = x_rad - xRotation_;
    RotateX(angle);
    xRotation_ = x_rad;
  }
  if (y_rad != yRotation_) {
    angle = y_rad - yRotation_;
    RotateY(angle);
    yRotation_ = y_rad;
  }
  if (z_rad != zRotation_) {
    angle = z_rad - zRotation_;
    RotateZ(angle);
    zRotation_ = z_rad;
  }
}

void Model::RotateX(const float angle) {
  for (auto& vertex : vertexes_) {
    float y = cos(angle) * vertex.y() - sin(angle) * vertex.z();
    float z = sin(angle) * vertex.y() + cos(angle) * vertex.z();
    vertex = {vertex.x(), y, z};
  }
}

void Model::RotateY(const float angle) {
  for (auto& vertex : vertexes_) {
    float x = cos(angle) * vertex.x() + sin(angle) * vertex.z();
    float z = -sin(angle) * vertex.x() + cos(angle) * vertex.z();
    vertex = {x, vertex.y(), z};
  }
}

void Model::RotateZ(const float angle) {
  for (auto& vertex : vertexes_) {
    float x = cos(angle) * vertex.x() - sin(angle) * vertex.y();
    float y = sin(angle) * vertex.x() + cos(angle) * vertex.y();
    vertex = {x, y, vertex.z()};
  }
}

void Model::MoveTo(const int x, const int y, const int z) {
  float moveX = x - xMove_;
  float moveY = y - yMove_;
  float moveZ = z - zMove_;
  Move(moveX, moveY, moveZ);
  xMove_ = x;
  yMove_ = y;
  zMove_ = z;
}

void Model::Move(const float x, const float y, const float z) {
  for (auto& vertex : vertexes_) vertex += {x, y, z};
}

void Model::ScaleTo(const double a) {
  float k = a * (1 / scale_);
  if (k == 0) return;
  Scale(k);
  scale_ = a;
}

void Model::Scale(const double k) {
  for (auto& vertex : vertexes_) {
    vertex *= k;
  }
}

void Model::Clear() {
  ClearModel();
  ClearSettings();
}

void Model::ClearModel() {
  vertexes_.clear();
  edges_.clear();
}

void Model::ClearSettings() {
  scale_ = 1;
  xRotation_ = 0;
  yRotation_ = 0;
  zRotation_ = 0;
  xMove_ = 0;
  yMove_ = 0;
  zMove_ = 0;
}

}  // namespace S21
