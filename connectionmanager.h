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

// ���ջ�����Ĭ�ϴ�С
#define RCVBUFFER_LENGTH 1024*10
#define PORT_TO_LISTEN 38757

/************************************************************************/
/* ÿ�����ӵ�Ӧ���߳�
/************************************************************************/
class ClientHandler;

/************************************************************************/
/* Ӧ���̶߳����ļ�����
/************************************************************************/
class ClientListener;

/************************************************************************/
/* UDPӦ���߳�
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
	// �ͻ���ID�Ϳͻ���Ӧ���̵߳Ķ�ӦMap���ͻ�������ʱ�������ΪID
	QMap<QString, ClientHandler*> * mClientConnectedList;

	// ��ӦConnectionManager��������е�Client�̵߳ļ�����
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
