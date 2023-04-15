CONFIG += c++17 warn_on
QT     += core gui
QMAKE_CXXFLAGS += -O0 -g --coverage 
QMAKE_LFLAGS += -O0 -g --coverage

LIBS += -lpthread -lgtest -lm
unix {
  LIBS += -lcheck
}
# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


HEADERS += \
            ../code/model/model.h \

SOURCES += \
            test.cpp \
            ../code/model/model.cpp \

