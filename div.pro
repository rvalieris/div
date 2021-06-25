
TARGET = div
VERSION = 0.1.8
HEADERS += src/*.h
SOURCES += src/*.cpp
CONFIG += c++11
TEMPLATE = app
RESOURCES = resources/resources.qrc
QT = widgets svg
LIBS += -larchive
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

DESTDIR = build
OBJECTS_DIR = $$DESTDIR/
MOC_DIR = $$DESTDIR/
RCC_DIR = $$DESTDIR/
UI_DIR = $$DESTDIR/
