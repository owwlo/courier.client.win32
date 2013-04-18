#include "doorkeeper.h"
#include <QNetworkInterface>
#include "json.h"

DoorKeeper * DoorKeeper::sInstance = nullptr;

DoorKeeper::DoorKeeper(QObject *parent)
	: QThread(parent)
{
	updateAddresses();
	broadcastSocket.bind(QHostAddress::Any, broadcastPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
	connect(&broadcastSocket, SIGNAL(readyRead()), this, SLOT(readBroadcastDatagram()));
	broadcastTimer.setInterval(BroadcastInterval);
	connect(&broadcastTimer, SIGNAL(timeout()), this, SLOT(sendBroadcastDatagram()));
	broadcastTimer.start();
}

DoorKeeper::~DoorKeeper()
{

}

void DoorKeeper::run()
{

}

// 刷新本机各个网卡IP地址，更新ipAddresses和broadcastAddresses
void DoorKeeper::updateAddresses()
{
	broadcastAddresses.clear();
	ipAddresses.clear();
	foreach (QNetworkInterface interface, QNetworkInterface::allInterfaces()) {
		foreach (QNetworkAddressEntry entry, interface.addressEntries()) {
			QHostAddress broadcastAddress = entry.broadcast();
			if (broadcastAddress != QHostAddress::Null && entry.ip() != QHostAddress::LocalHost) {
				broadcastAddresses << broadcastAddress;
				ipAddresses << entry.ip();
				qDebug()<<"Qt: Found Network, broadcastAddresses: "<<broadcastAddresses<<" ipAddresses: "<<ipAddresses;
			}
		}
	}
}

void DoorKeeper::sendBroadcastDatagram()
{
	qDebug()<<"Qt: sendBroadcastDatagram";
	//QByteArray datagram(username);
	//datagram.append('@');
	//datagram.append(QByteArray::number(serverPort));

	//bool validBroadcastAddresses = true;
	//foreach (QHostAddress address, broadcastAddresses) {
	//	if (broadcastSocket.writeDatagram(datagram, address,
	//		broadcastPort) == -1)
	//		validBroadcastAddresses = false;
	//}

	//if (!validBroadcastAddresses)
	//	updateAddresses();
}

void DoorKeeper::readBroadcastDatagram()
{
	while (broadcastSocket.hasPendingDatagrams()) {
		QHostAddress senderIp;
		quint16 senderPort;
		QByteArray datagram;
		datagram.resize(broadcastSocket.pendingDatagramSize());
		if (broadcastSocket.readDatagram(datagram.data(), datagram.size(),
			&senderIp, &senderPort) == -1)
			continue;

		qDebug()<<"Qt: Got Pending Datagrams: "<<datagram;
		qDebug()<<"Qt: Client IP: "<<senderIp <<" Port: "<<senderPort;

		QByteArray decodeBytes = QByteArray::fromBase64(datagram);
		QString decodeString(decodeBytes);
		if (!decodeString.startsWith(BASE64_MAGIC_STRING))
			continue;

		qDebug()<<"Qt: Decode String: "<<decodeString;
		decodeString = decodeString.mid(QString(BASE64_MAGIC_STRING).length());

		bool isParsingSuccess;
		QVariantMap json = QtJson::parse(decodeString, isParsingSuccess).toMap();
		if(!isParsingSuccess)
			continue;

		SOCKET_CLIENT_INFO clientInfo;
		clientInfo.imxi = json["imxi"].toString();
		clientInfo.ip = senderIp;
		clientInfo.port = json["port"].toString().toInt();

		emit SIGNAL_KNOCK_DOOR(clientInfo);
		//if (isLocalHostAddress(senderIp) && senderServerPort == serverPort)
			//continue;

		//if (!client->hasConnection(senderIp)) {
		//	Connection *connection = new Connection(this);
		//	emit newConnection(connection);
		//	connection->connectToHost(senderIp, senderServerPort);
		//}
	}
}

DoorKeeper * DoorKeeper::getInstance()
{
	if(sInstance == nullptr) 
	{
		sInstance = new DoorKeeper(0);
		sInstance->init();
	}
	return sInstance;
}

void DoorKeeper::init()
{
	mConnectionManager = ConnectionManager::getInstance();
	connect(this,SIGNAL(SIGNAL_KNOCK_DOOR(SOCKET_CLIENT_INFO)),mConnectionManager,SLOT(OnDoorKnocked(SOCKET_CLIENT_INFO)));
}
