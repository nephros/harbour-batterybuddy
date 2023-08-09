TARGET = harbour-batterybuddy-daemon

QT = core dbus

PKGCONFIG += nemonotifications-qt5

# Keep this in sync with application.pro and .spec
VERSION = 4.3.2-1

DEFINES += APP_VERSION=\"\\\"$$VERSION\\\"\"
DEFINES += APP_NAME=\"\\\"$$TARGET\\\"\"

# Use "--verbose" and "--debug" at runtime.
# See main() and logger.h for details.
DEFINES += QT_NO_DEBUG_OUTPUT

LIBS += -lkeepalive -lnemonotifications-qt5

# build position-independent executable:
QMAKE_CXXFLAGS += -Werror
QMAKE_CFLAGS += -fPIE
QMAKE_CXXFLAGS += -fPIE
QMAKE_LFLAGS += -pie

HEADERS += \
    src/battery.h \
    src/dbus.h \
    src/dbus_interface.h \
    src/logger.h \
    src/settings.h \
    src/mynotification.h

SOURCES += \
    src/battery.cpp \
    src/dbus.cpp \
    src/logger.cpp \
    src/settings.cpp \
    src/harbour-batterybuddy-daemon.cpp \
    src/mynotification.cpp

OTHER_FILES += harbour-batterybuddy-daemon.service

service.path = /usr/lib/systemd/user/
service.files += harbour-batterybuddy-daemon.service
target.path = /usr/bin/

INSTALLS += target service


# DBus
dbus_iface.files += src/fi.kapsi.direc85.batterybuddy.xml
dbus_iface.path = /usr/share/dbus-1/interfaces/
INSTALLS += dbus_iface

# calls qdbusxml2cpp to generate batterybuddy_adaptor.h
dbus_adaptor.files = src/fi.kapsi.direc85.batterybuddy.xml
dbus_adaptor.source_flags = -c DaemonInterfaceAdaptor
dbus_adaptor.header_flags = -c DaemonInterfaceAdaptor -i src/dbus_interface.h
DBUS_ADAPTORS += dbus_adaptor
