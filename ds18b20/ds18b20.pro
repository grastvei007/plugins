#-------------------------------------------------
#
# Project created by QtCreator 2018-08-23T16:18:04
#
#-------------------------------------------------

QT       -= gui
CONFIG += c++14
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += core


TEMPLATE = lib

DEFINES += DS18B20_LIBRARY
DEFINES += RASPBERRY_PI


if(debug){
	TARGET = ds18b20d
}
else{
	TARGET = ds18b20
}

DESTDIR = $$(DEV_LIBS)


if(debug){
	BUILDDIR = build/debug
}
else{
	BUILDDIR = build/release
}

OBJECTS_DIR = $$BUILDDIR/.obj
MOC_DIR = $$BUILDDIR/.moc
RCC_DIR = $$BUILDDIR/.qrc
UI_DIR = $$BUILDDIR/.ui


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$(JUNE_ROOT)

if(debug){

    LIBS += -L$$(DEV_LIBS) -ltagsystemd -lpluginLoadd
#LIBS += -ltagsystemd
}
else{
    LIBS += -L$$(DEV_LIBS) -ltagsystem -lpluginLoad
}

SOURCES += \
    ds18b20.cpp

HEADERS += \
    ds18b20.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

