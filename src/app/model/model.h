#ifndef MODEL_H
#define MODEL_H

#include <QFile>
#include <QPair>
#include <QVector3D>
#include <cmath>
#include <set>
#include <vector>

namespace S21 {

// Функция сравнения пар вершин для set, чтобы исключить повторяющиеся пары
struct PairLess {
  using pair = QPair<qsizetype, qsizetype>;
  using const_pair = const pair;
  bool operator()(const_pair& one, const_pair& two) const {
    if (one.first == two.second && one.second == two.first) return false;
    return one < two;
  }
};

class Model {
 public:
  using edges_array = std::set<QPair<qsizetype, qsizetype>, PairLess>;
  using vertex_array = std::vector<QVector3D>;

 public:
  static Model& Init();
  void CreateInFile(QString& pathToFile);
  qsizetype GetCountVertex() const { return vertexes_.size(); }
  qsizetype GetCountEdges() const { return edges_.size(); }
  const vertex_array& GetVertexes() const { return vertexes_; }
  const edges_array& GetEdges() const { return edges_; }
  void RotateTo(const int x, const int y, const int z);
  void MoveTo(const int x, const int y, const int z);
  void ScaleTo(const double a);
  void Clear();

 private:
  vertex_array vertexes_;
  edges_array edges_;
  float xRotation_{}, yRotation_{}, zRotation_{};
  int xMove_{}, yMove_{}, zMove_{};
  double scale_{1};

  Model() {}
  Model(const Model& model) = delete;
  Model& operator=(const Model& model) = delete;

  void ParseFile(QFile& file);
  void ParseLine(const QString& str);
  void CheckAllVertexExist();

  void InsertVertex(const QString& str);
  void CheckLineVertexFormat(const QStringList& list);
  float ToFloat(const QString& value);

  void InsertEdge(const QString& str);
  void CheckLineEdgeFormat(const QStringList& list);
  qsizetype ToUInt(const QString& value);

  void RotateTo(const float x, const float y, const float z);
  void RotateX(const float angle);
  void RotateY(const float angle);
  void RotateZ(const float angle);
  void Move(const float x, const float y, const float z);
  void Scale(const double k);

  void ClearModel();
  void ClearSettings();
};
}  // namespace S21
#endif  // MODEL_H
