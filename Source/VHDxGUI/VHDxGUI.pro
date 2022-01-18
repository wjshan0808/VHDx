#详情帮助搜索qmake Manual下 Variables, Test Functions 关键字

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#目标文件模板
#TARGET = tar
#TEMPLATE = app

#消息
#message("PWD="$$PWD)
#message("OUT_PWD="$$OUT_PWD)


#改变Build目录在项目中修改
#Include目录(相对于Build目录)
INCLUDEPATH += ../Include
#Library目录(相对于Build目录)
LIBS += -L../Library/ -lVHDX
#执行文件输出目录(相对于Build目录)
DESTDIR += ../Release


#UI中间文件输出目录
#UI_DIR += ../../Temp
#UI_HEADERS_DIR +=
#UI_SOURCES_DIR +=
#MOC中间文件输出目录
#MOC_DIR += ../../Temp
#QRC中间文件输出目录
#RCC_DIR += ../../Temp
#OBJ文件输出目录
#OBJECTS_DIR += ../../Temp


CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    MainWindow.h

FORMS += \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
