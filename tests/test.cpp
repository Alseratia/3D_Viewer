#define COMMON_TEST

#include <gtest/gtest.h>

#include "../code/model/model.h"
#include "../code/model/model.cpp"

using namespace Qt;
using namespace S21;

TEST(ModelTest, Parsing) {
S21::Model& model = S21::Model::Init();
QString file = "objects/cube.obj";
model.CreateInFile(file);
}

TEST(ModelTest, Clear) {
S21::Model& model = S21::Model::Init();
model.Clear();
}

TEST(ModelTest, CreateFile) {
  QString path = "file.obj";
  QFile file(path);
  ASSERT_TRUE(file.open(QIODevice::WriteOnly | QIODevice::Text));
  file.close();
  ASSERT_TRUE(QFile::exists("file.obj"));
  QFile::remove("file.obj");
}

TEST(ModelTest, RotateToX) {
S21::Model& model = S21::Model::Init();
QString file = "objects/cube.obj";
model.CreateInFile(file);
auto vertexes = model.GetVertexes();
model.RotateTo(90, 0, 0);
auto rotated_vertexes = model.GetVertexes();
EXPECT_FALSE(vertexes == rotated_vertexes);
}

TEST(ModelTest, RotateToY) {
S21::Model& model = S21::Model::Init();
QString file = "objects/cube.obj";
model.CreateInFile(file);
auto vertexes = model.GetVertexes();
model.RotateTo(0, 90, 0);
auto rotated_vertexes = model.GetVertexes();
EXPECT_FALSE(vertexes == rotated_vertexes);
}

TEST(ModelTest, RotateToZ) {
S21::Model& model = S21::Model::Init();
QString file = "objects/cube.obj";
model.CreateInFile(file);
auto vertexes = model.GetVertexes();
model.RotateTo(0, 0, 90);
auto rotated_vertexes = model.GetVertexes();
EXPECT_FALSE(vertexes == rotated_vertexes);
}

TEST(ModelTest, MoveTo) {
S21::Model& model = S21::Model::Init();
QString file = "objects/cube.obj";
model.CreateInFile(file);
auto vertexes = model.GetVertexes();
model.MoveTo(10, 20, 30);
auto moved_vertexes = model.GetVertexes();
EXPECT_FALSE(vertexes == moved_vertexes);
}

TEST(ModelTest, ScaleTo) {
S21::Model& model = S21::Model::Init();
QString file = "objects/cube.obj";
model.CreateInFile(file);
auto vertexes = model.GetVertexes();
model.ScaleTo(2);
auto scaled_vertexes = model.GetVertexes();
EXPECT_FALSE(vertexes == scaled_vertexes);
model.ScaleTo(1);
auto restored_vertexes = model.GetVertexes();
EXPECT_TRUE(vertexes == restored_vertexes);
}

TEST(ModelTest, ParseFileTest){
QFile file("test.obj");
if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
QTextStream out(&file);
out << "v 1.0 2.0 3.0\n"
"v 2.0 3.0 4.0\n"
"f 1 2\n";
file.close();
} else {
FAIL() << "Cannot open file for writing";
}
QFile::remove("test.obj");
}

int main() {
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}
