#include "view/mainwindow.h"
#include <QApplication>

using namespace S21;

void SetStyle(QApplication& app) {
  QFile file(":/dark/stylesheet.qss");
  file.open(QFile::ReadOnly | QFile::Text);
  QTextStream stream(&file);
  app.setStyleSheet(stream.readAll());
}

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  SetStyle(app);
  Controller controller;
  MainWindow w(&controller);
  return app.exec();
}
