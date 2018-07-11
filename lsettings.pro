TEMPLATE = lib
CONFIG += plugin
QT += qml
QT += quick
CONFIG += c++11

TARGET  = dnaisettingsplugin

pluginfiles.files += \


isEmpty(PROJECT_ROOT_DIRECTORY){
  PROJECT_ROOT_DIRECTORY = $$[QT_INSTALL_QML]
}

# message($${PROJECT_ROOT_DIRECTORY})


target.path += $${PROJECT_ROOT_DIRECTORY}/Dnai/Settings
pluginfiles.path += $${PROJECT_ROOT_DIRECTORY}/Dnai/Settings

qmldirsrc.path =  $${PROJECT_ROOT_DIRECTORY}/Dnai/Settings
qmldirsrc.files += \
    qmldir \
    plugins.qmltypes

INSTALLS += target pluginfiles qmldirsrc

CONFIG += install_ok

DISTFILES += \
    qmldir \

HEADERS += \
    dnaisettings_plugin.h \
    settings.h \
    settingsparameters.h

SOURCES += \
    dnaisettings_plugin.cpp \
    settings.cpp \
    settingsparameters.cpp
