
#include "dbus.h"
#include "dbus_interface.h"
#include "batterybuddy_adaptor.h"

#include <QtDBus/QDBusConnection>

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

#define NAME(x) #x

DaemonInterface::DaemonInterface(Battery* battery, QObject *parent)
    : QObject(parent)
{
    m_logger = new Logger(true, true, false);

    if (battery)
        m_battery = battery;

    doRegisterDBus();
}

DaemonInterface::~DaemonInterface()
{
    if (m_dbusRegistered) {
        QDBusConnection connection = QDBusConnection::sessionBus();
        connection.unregisterService(DBUS_SERVICE_NAME);
        connection.unregisterObject(DBUS_PATH_NAME);
    }
}

void DaemonInterface::registerDBus()
{
    QMetaObject::invokeMethod(this, NAME(doRegisterDBus), Qt::QueuedConnection);
}

void DaemonInterface::doRegisterDBus()
{
    if (m_dbusRegistered) {
        return;
    }

    QDBusConnection connection = QDBusConnection::sessionBus();

    if (connection.interface()->isServiceRegistered(DBUS_SERVICE_NAME)) {
        qWarning() << Q_FUNC_INFO << "Already was registered D-Bus service" << DBUS_SERVICE_NAME;
        return;
    }

    if (!connection.registerObject(DBUS_PATH_NAME, this)) {
        qCritical() << Q_FUNC_INFO << "Cannot register D-Bus object" << DBUS_PATH_NAME;
        return;
    }

    qInfo() << Q_FUNC_INFO << "Successfully registered D-Bus object" << DBUS_PATH_NAME;
    m_logger->log(QString("Successfully registered D-Bus object %1").arg(DBUS_PATH_NAME));

    if (!connection.registerService(DBUS_SERVICE_NAME)) {
        qCritical() << Q_FUNC_INFO << "Cannot register D-Bus service" << DBUS_SERVICE_NAME;
        return;
    }

    m_adaptor = new DaemonInterfaceAdaptor(this);

    qInfo() << Q_FUNC_INFO << "Successfully registered D-Bus service" << DBUS_SERVICE_NAME;
    m_logger->log(QString("Successfully registered D-Bus service %1").arg(DBUS_SERVICE_NAME));

    m_dbusRegistered = true;
}

void DaemonInterface::shutdown() {

    if (m_dbusRegistered) {
        QDBusConnection connection = QDBusConnection::sessionBus();
        connection.unregisterService(DBUS_SERVICE_NAME);
        connection.unregisterObject(DBUS_PATH_NAME);
        m_dbusRegistered = false;
    }
}

bool DaemonInterface::chargingEnabled() {
    return m_battery->getChargingEnabled();
}

void DaemonInterface::enableCharging() {
    bool old = m_battery->getChargingEnabled();
    bool ok = m_battery->setChargingEnabled(true);
    if (!ok) {
        qWarning() << Q_FUNC_INFO << "Could not toggle Charging";
    }
    if (ok && old == false) {
        if (m_adaptor)
            emit m_adaptor->chargingEnabledChanged();
    }
}

void DaemonInterface::disableCharging() {
    bool old = m_battery->getChargingEnabled();
    bool ok = m_battery->setChargingEnabled(false);
    if (!ok) {
        qWarning() << Q_FUNC_INFO << "Could not toggle Charging";
    }
    if (ok && old == true) {
        if (m_adaptor)
            emit m_adaptor->chargingEnabledChanged();
    }
}

bool DaemonInterface::canToggleCharging() {
    return m_battery->canToggleCharging();
}
