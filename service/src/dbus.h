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

public:
    explicit DaemonInterface(Battery* battery = nullptr, QObject *parent = nullptr);
    virtual ~DaemonInterface();

public slots:
    void enableCharging();
    void disableCharging();
    bool isChargingEnabled();

    bool canToggleCharging();
    void shutdown();

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
