/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the plugins of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QNETWORKMANAGERSERVICE_H
#define QNETWORKMANAGERSERVICE_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtDBus/QDBusAbstractInterface>
#include <QtDBus/QDBusPendingCallWatcher>
#include <QtDBus/QDBusObjectPath>

// Matches 'NMDeviceState' from https://developer.gnome.org/NetworkManager/stable/nm-dbus-types.html
enum NMDeviceState {
    NM_DEVICE_STATE_UNKNOWN = 0,
    NM_DEVICE_STATE_UNMANAGED = 10,
    NM_DEVICE_STATE_UNAVAILABLE = 20,
    NM_DEVICE_STATE_DISCONNECTED = 30,
    NM_DEVICE_STATE_PREPARE = 40,
    NM_DEVICE_STATE_CONFIG = 50,
    NM_DEVICE_STATE_NEED_AUTH = 60,
    NM_DEVICE_STATE_IP_CONFIG = 70,
    NM_DEVICE_STATE_ACTIVATED = 100,
    NM_DEVICE_STATE_DEACTIVATING = 110,
    NM_DEVICE_STATE_FAILED = 120
};

QT_BEGIN_NAMESPACE

// This tiny class exists for the purpose of seeing if NetworkManager is available without
// initializing everything the derived/full class needs.
class QNetworkManagerInterfaceBase : public QDBusAbstractInterface
{
    Q_OBJECT
public:
    QNetworkManagerInterfaceBase(QObject *parent = nullptr);
    ~QNetworkManagerInterfaceBase() = default;

    static bool networkManagerAvailable();

private:
    Q_DISABLE_COPY_MOVE(QNetworkManagerInterfaceBase)
};

class QNetworkManagerInterface final : public QNetworkManagerInterfaceBase
{
    Q_OBJECT

public:
    // Matches 'NMState' from https://developer.gnome.org/NetworkManager/stable/nm-dbus-types.html
    enum NMState {
        NM_STATE_UNKNOWN = 0,
        NM_STATE_ASLEEP = 10,
        NM_STATE_DISCONNECTED = 20,
        NM_STATE_DISCONNECTING = 30,
        NM_STATE_CONNECTING = 40,
        NM_STATE_CONNECTED_LOCAL = 50,
        NM_STATE_CONNECTED_SITE = 60,
        NM_STATE_CONNECTED_GLOBAL = 70
    };
    Q_ENUM(NMState);
    // Matches 'NMConnectivityState' from
    // https://developer.gnome.org/NetworkManager/stable/nm-dbus-types.html#NMConnectivityState
    enum NMConnectivityState {
        NM_CONNECTIVITY_UNKNOWN = 0,
        NM_CONNECTIVITY_NONE = 1,
        NM_CONNECTIVITY_PORTAL = 2,
        NM_CONNECTIVITY_LIMITED = 3,
        NM_CONNECTIVITY_FULL = 4,
    };
    Q_ENUM(NMConnectivityState);
    // Matches 'NMDeviceType' from
    // https://developer-old.gnome.org/NetworkManager/stable/nm-dbus-types.html#NMDeviceType
    enum NMDeviceType {
        NM_DEVICE_TYPE_UNKNOWN = 0,
        NM_DEVICE_TYPE_GENERIC = 14,
        NM_DEVICE_TYPE_ETHERNET = 1,
        NM_DEVICE_TYPE_WIFI = 2,
        NM_DEVICE_TYPE_UNUSED1 = 3,
        NM_DEVICE_TYPE_UNUSED2 = 4,
        NM_DEVICE_TYPE_BT = 5,
        NM_DEVICE_TYPE_OLPC_MESH = 6,
        NM_DEVICE_TYPE_WIMAX = 7,
        NM_DEVICE_TYPE_MODEM = 8,
        NM_DEVICE_TYPE_INFINIBAND = 9,
        NM_DEVICE_TYPE_BOND = 10,
        NM_DEVICE_TYPE_VLAN = 11,
        NM_DEVICE_TYPE_ADSL = 12,
        NM_DEVICE_TYPE_BRIDGE = 13,
        NM_DEVICE_TYPE_TEAM = 15,
        NM_DEVICE_TYPE_TUN = 16,
        NM_DEVICE_TYPE_IP_TUNNEL = 17,
        NM_DEVICE_TYPE_MACVLAN = 18,
        NM_DEVICE_TYPE_VXLAN = 19,
        NM_DEVICE_TYPE_VETH = 20,
        NM_DEVICE_TYPE_MACSEC = 21,
        NM_DEVICE_TYPE_DUMMY = 22,
        NM_DEVICE_TYPE_PPP = 23,
        NM_DEVICE_TYPE_OVS_INTERFACE = 24,
        NM_DEVICE_TYPE_OVS_PORT = 25,
        NM_DEVICE_TYPE_OVS_BRIDGE = 26,
        NM_DEVICE_TYPE_WPAN = 27,
        NM_DEVICE_TYPE_6LOWPAN = 28,
        NM_DEVICE_TYPE_WIREGUARD = 29,
        NM_DEVICE_TYPE_WIFI_P2P = 30,
        NM_DEVICE_TYPE_VRF = 31,
    };

    QNetworkManagerInterface(QObject *parent = nullptr);
    ~QNetworkManagerInterface();

    NMState state() const;
    NMConnectivityState connectivityState() const;
    NMDeviceType deviceType() const;

Q_SIGNALS:
    void stateChanged(NMState);
    void connectivityChanged(NMConnectivityState);
    void deviceTypeChanged(NMDeviceType);

private Q_SLOTS:
    void setProperties(const QString &interfaceName, const QMap<QString, QVariant> &map,
                       const QStringList &invalidatedProperties);

private:
    Q_DISABLE_COPY_MOVE(QNetworkManagerInterface)

    NMDeviceType extractDeviceType(QDBusObjectPath devicePath) const;

    QVariantMap propertyMap;
};

class PropertiesDBusInterface : public QDBusAbstractInterface
{
public:
    PropertiesDBusInterface(const QString &service, const QString &path, const QString &interface,
                            const QDBusConnection &connection, QObject *parent = 0)
        : QDBusAbstractInterface(service, path, interface.toLatin1().data(), connection, parent)
    {
    }
};

QT_END_NAMESPACE

#endif
