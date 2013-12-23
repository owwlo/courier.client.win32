#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include <QThread>
#include <QList>
#include <QMap>
#include "doorkeeper.h"
#include <QMutex>
#include "uimanager.h"
#include "models.h"

// 接收缓冲区默认大小
#define RCVBUFFER_LENGTH 1024*10
#define PORT_TO_LISTEN 38757

/************************************************************************/
/* 每个连接的应答线程
/************************************************************************/
class ClientHandler;

/************************************************************************/
/* 应答线程动作的监听器
/************************************************************************/
class ClientListener;

/************************************************************************/
/* UDP应答线程
/************************************************************************/
class DoorKeeper;

class UIManager;

enum CLIENT_TYPE;

class ConnectionManager : public QThread
{
	Q_OBJECT

public:
	static ConnectionManager * getInstance();
	~ConnectionManager();
	qint32 getClientConnectedCount();

public slots:
	void OnReceiveMessage(QString message);
	void OnClientExit();
	void OnDoorKnocked(COURIER::SOCKET_CLIENT_INFO);
	void OnReconnect(COURIER::SOCKET_CLIENT_INFO);
	void onAuthenticateCodeEntered(QVariant);
	void connectionError(QAbstractSocket::SocketError);
	void onMessagReplySubmittedByUser(QString id, COURIER::MESSAGE_ITEM);

signals:
	void SIGNAL_REQUIRE_AUTHENTICATE_CODE();
	void SIGNAL_ON_RECONNECTED(COURIER::SOCKET_CLIENT_INFO);

private:
	void initServer();
	virtual void run();
	ConnectionManager(QObject *parent);
	void newClientAdded(/*SOCKET*/);
	QString getClientId( COURIER::SOCKET_CLIENT_INFO info );
	void newSessionCheck();
	QString getClientCodeFromAuthCode( QString code );
	void removeConnection( ClientHandler * client );
	static ConnectionManager * sInstance;
	
	QMutex mClientConnectedList_mutex;
	// 客户端ID和客户端应答线程的对应Map，客户端连入时间可以做为ID
	QMap<QString, ClientHandler*> * mClientConnectedList;

	// 响应ConnectionManager管理的所有的Client线程的监听器
	ClientListener * mListener;
	DoorKeeper * mDoorKeeper;
	UIManager * mUiManager;

	QMutex mKnockedClientList_mutex;
	QMap<QString, COURIER::SOCKET_CLIENT_INFO> * mKnockedClientList;

	class ClientListener
	{
		virtual void OnReceiveMessage(QString message) = 0;
		virtual void OnClientExit(QString id) = 0;
	};

};

#endif // CONNECTIONMANAGER_H
