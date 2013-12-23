#ifndef DOORKEEPER_H
#define DOORKEEPER_H

#include <QThread>
#include <QUdpSocket>
#include <QTimer>
#include "connectionmanager.h"
#include <QMap>
#include "models.h"

static const qint32 broadcastPort = 27392;
static const qint32 BroadcastInterval = 60*1000;

#define BASE64_MAGIC_STRING "CoUrIeR"

class ConnectionManager;

struct COURIER::SOCKET_CLIENT_INFO;

class DoorKeeper : public QThread
{
	Q_OBJECT

public:
	static DoorKeeper * getInstance();
	~DoorKeeper();

private slots:
	void sendBroadcastDatagram();
	void readBroadcastDatagram();

signals:
	void SIGNAL_KNOCK_DOOR(COURIER::SOCKET_CLIENT_INFO);
	void SIGNAL_RECONNECT(COURIER::SOCKET_CLIENT_INFO);

private:
	QList<QHostAddress> broadcastAddresses;
	QList<QHostAddress> ipAddresses;
	QUdpSocket broadcastSocket;
	QTimer broadcastTimer;
	static DoorKeeper * sInstance;
	ConnectionManager * mConnectionManager;

	DoorKeeper(QObject *parent);
	virtual void run();
	void updateAddresses();
	void init();
};

#endif // DOORKEEPER_H
