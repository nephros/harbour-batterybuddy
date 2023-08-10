#ifndef DBUS_H
#define DBUS_H

#include <QtCore/QObject>

#include <QDBusContext>

#include "battery.h"
#include "logger.h"

class DaemonInterfaceAdaptor;
class DaemonInterface : public QObject, public QDBusContext
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", DBUS_SERVICE_NAME)

    Q_PROPERTY(bool canToggleCharging READ canToggleCharging); // NOTIFY canToggleChargingChanged);
    Q_PROPERTY(bool chargingEnabled READ chargingEnabled);

public:
    explicit DaemonInterface(Battery* battery = nullptr, QObject *parent = nullptr);
    virtual ~DaemonInterface();

public slots:
    void enableCharging();
    void disableCharging();

    bool canToggleCharging();
    bool chargingEnabled();

    void shutdown();

//signals:
//    void canToggleChargingChanged(bool state);

private:
    void registerDBus();

    bool m_chargingEnabled;
    bool m_dbusRegistered = false;

    DaemonInterfaceAdaptor *m_adaptor = nullptr;
    Battery *m_battery = nullptr;
    Logger *m_logger = nullptr;

private slots:
    void doRegisterDBus();
};
#endif
