TARGET = open-typer

QT += core gui charts network websockets
QTPLUGIN += qsvg
!wasm {
    QT += printsupport sql
    LIBS += -lssl -lcrypto
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Version and revision
win32 {
    GIT_LOCATION = $$system(where git)
}
!win32 {
    GIT_LOCATION = $$system(which git)
}
isEmpty(GIT_LOCATION) | !exists(.git) {
    exists(version) : exists(revision) {
        DEFINES += BUILD_VERSION=\\\"$$cat(version)\\\"
        DEFINES += BUILD_REVISION=\\\"$$cat(revision)\\\"
    }
} else {
    DEFINES += BUILD_VERSION=\\\"$$system(git describe --tags --abbrev=0)\\\"
    DEFINES += BUILD_REVISION=\\\"$$system(git rev-parse --short HEAD)\\\"
}

INCLUDEPATH += \
    src/include

win32 {
    INCLUDEPATH += C:/OpenSSL/include
}

SOURCES += \
    src/core/StringUtils.cpp \
    src/core/FileUtils.cpp \
    src/core/KeyboardUtils.cpp \
    src/core/ConfigParser.cpp \
    src/core/HistoryParser.cpp \
    src/core/DatabaseManager.cpp
!wasm {
    SOURCES += src/core/MonitorServer.cpp
}
SOURCES += \
    src/core/MonitorClient.cpp \
    src/core/BuiltInPacks.cpp \
    src/core/LanguageManager.cpp \
    src/core/Settings.cpp \
    src/core/ThemeEngine.cpp \
    src/InitialSetup.cpp \
    src/ExerciseSummary.cpp \
    src/options/ConnectionOptions.cpp \
    src/options/StudentOptions.cpp \
    src/StatsDialog.cpp \
    src/TimeDialog.cpp \
    src/TestWaitDialog.cpp \
    src/options/BehaviorOptions.cpp \
    src/options/AppearanceOptions.cpp \
    src/options/KeyboardOptions.cpp \
    src/options/OptionsWindow.cpp \
    src/packEditor/PackEditor.cpp \
    src/packEditor/PackSelector.cpp \
    src/updater/Updater.cpp \
    src/main.cpp \
    src/MainWindow.cpp \
    src/ExportDialog.cpp \
    src/server/UserManager.cpp \
    src/server/UserEdit.cpp \
    src/server/AdminSelector.cpp \
    src/server/ServerSetup.cpp \
    src/server/ClassEdit.cpp \
    src/server/StudentEdit.cpp \
    src/server/DeviceEdit.cpp \
    src/server/LoadExerciseDialog.cpp \
    src/server/ExerciseProgressDialog.cpp \
    src/server/widgets/ServerManager.cpp \
    src/server/widgets/ClassControls.cpp \
    src/server/widgets/StudentDetails.cpp \
    src/updater/UpdaterDialog.cpp \
    src/widgets/InputLabelWidget.cpp \
    src/widgets/TextView.cpp \
    src/widgets/LanguageList.cpp \
    src/widgets/KeyboardWidget.cpp

HEADERS += \
    src/include/core/StringUtils.h \
    src/include/core/FileUtils.h \
    src/include/core/KeyboardUtils.h \
    src/include/core/ConfigParser.h \
    src/include/core/HistoryParser.h \
    src/include/core/DatabaseManager.h
!wasm {
    HEADERS += src/include/core/MonitorServer.h
}
HEADERS += \
    src/include/core/MonitorClient.h \
    src/include/core/BuiltInPacks.h \
    src/include/core/LanguageManager.h \
    src/include/core/Settings.h \
    src/include/core/ThemeEngine.h \
    src/include/InitialSetup.h \
    src/include/ExerciseSummary.h \
    src/include/options/ConnectionOptions.h \
    src/include/options/StudentOptions.h \
    src/include/StatsDialog.h \
    src/include/TimeDialog.h \
    src/include/TestWaitDialog.h \
    src/include/options/BehaviorOptions.h \
    src/include/options/AppearanceOptions.h \
    src/include/options/KeyboardOptions.h \
    src/include/options/OptionsWindow.h \
    src/include/packEditor/PackEditor.h \
    src/include/packEditor/PackSelector.h \
    src/include/updater/Updater.h \
    src/include/updater/UpdaterDialog.h \
    src/include/MainWindow.h \
    src/include/ExportDialog.h \
    src/include/server/UserManager.h \
    src/include/server/UserEdit.h \
    src/include/server/AdminSelector.h \
    src/include/server/ServerSetup.h \
    src/include/server/ClassEdit.h \
    src/include/server/StudentEdit.h \
    src/include/server/DeviceEdit.h \
    src/include/server/LoadExerciseDialog.h \
    src/include/server/ExerciseProgressDialog.h \
    src/include/server/widgets/ServerManager.h \
    src/include/server/widgets/ClassControls.h \
    src/include/server/widgets/StudentDetails.h \
    src/include/widgets/InputLabelWidget.h \
    src/include/widgets/TextView.h \
    src/include/widgets/LanguageList.h \
    src/include/widgets/KeyboardWidget.h

FORMS += \
    ui/InitialSetup.ui \
    ui/options/BehaviorOptions.ui \
    ui/options/ConnectionOptions.ui \
    ui/options/AppearanceOptions.ui \
    ui/options/KeyboardOptions.ui \
    ui/ExerciseSummary.ui \
    ui/options/StudentOptions.ui \
    ui/StatsDialog.ui \
    ui/TimeDialog.ui \
    ui/TestWaitDialog.ui \
    ui/MainWindow.ui \
    ui/ExportDialog.ui \
    ui/server/UserManager.ui \
    ui/server/UserEdit.ui \
    ui/server/AdminSelector.ui \
    ui/server/ServerSetup.ui \
    ui/server/ClassEdit.ui \
    ui/server/StudentEdit.ui \
    ui/server/DeviceEdit.ui \
    ui/server/LoadExerciseDialog.ui \
    ui/server/ExerciseProgressDialog.ui \
    ui/server/widgets/ServerManager.ui \
    ui/server/widgets/ClassControls.ui \
    ui/server/widgets/StudentDetails.ui \
    ui/options/OptionsWindow.ui \
    ui/packEditor/PackEditor.ui \
    ui/packEditor/PackSelector.ui \
    ui/updater/UpdaterDialog.ui

TRANSLATIONS += \
    lang/Open-Typer_sk_SK.ts \
    lang/Open-Typer_de_DE.ts

RESOURCES += \
    res.qrc \
    res/images/class-icons/class-icons.qrc \
    res/keyboard-layouts/layouts.qrc

RESOURCES += \
    dark-theme/dark-style.qrc \
    light-theme/light-style.qrc

win32:RC_ICONS += res/images/icon.ico

# Third-party
include($$PWD/thirdparty/QWasmSettings/qwasmsettings.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target
