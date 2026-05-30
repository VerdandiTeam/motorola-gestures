TARGET = motorola-gestures

QT += core dbus network

CONFIG += console c++11 link_pkgconfig

PKGCONFIG += mlite5

SOURCES += src/miami-gestures.cpp \
    src/gesturemanager.cpp \
    src/sensor.cpp \
    src/sensormanager.cpp \
    src/socketclient.cpp

DISTFILES += qml/miami-gestures.qml \
    qml/Settings.qml \
    rpm/motorola-gestures.spec \
    motorola-gestures-settings.json \
    i18n/*.ts

HEADERS += \
    src/gesturemanager.h \
    src/sensor.h \
    src/genericdata.h \
    src/sensormanager.h \
    src/socketclient.h \
    src/timedunsigned.h

target.path = /usr/bin

entries.path = /usr/share/jolla-settings/entries
entries.files = motorola-gestures-settings.json

pages.path = /usr/share/jolla-settings/pages/motorola-gestures
pages.files = qml/Settings.qml

translations.path = /usr/share/translations
translations.files = i18n/*.qm

system(lrelease -idbased $$PWD/i18n/*.ts)

INSTALLS += target entries pages translations
