QT += quick
QT += serialport

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


RESOURCES += qml/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    appcontroller.h \
    appmodel.h \
    log.h \
    model/datarecored.h \
    model/recordstabelmodel.h \
    model/tabledatarecored.h \
    serial/portreader.h \
    appio.h \
    serial/serialreader.h

SOURCES += \
        appcontroller.cpp \
        appmodel.cpp \
        log.cpp \
        main.cpp \
        model/datarecored.cpp \
        model/recordstabelmodel.cpp \
        model/tabledatarecored.cpp \
        serial/portreader.cpp \
        appio.cpp \
        serial/serialreader.cpp

INCLUDEPATH += serial model

win32 {
    LIBS += -lws2_32 -lpsapi -ldbghelp -lwinmm
}

CONFIG(debug, debug|release) {
    DEFINES += DEBUG_MODE
}
