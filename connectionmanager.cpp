#include "connectionmanager.h"
#include "doorkeeper.h"
#include "clienthandler.h"
#include <QMutexLocker>

ConnectionManager * ConnectionManager::sInstance = nullptr;

ConnectionManager::ConnectionManager(QObject *parent)
	: QThread(parent)
{

}

ConnectionManager::~ConnectionManager()
{

}

ConnectionManager * ConnectionManager::getInstance()
{
	if(sInstance == nullptr)
	{
		sInstance = new ConnectionManager(0);
		sInstance->initServer();
	}
	return sInstance;
}

void ConnectionManager::run()
{

}

void ConnectionManager::initServer()
{
	mDoorKeeper = DoorKeeper::getInstance();
	mUiManager = UIManager::getInstance();
	
	mKnockedClientList = new QMap<QString, COURIER::SOCKET_CLIENT_INFO>;
	mClientConnectedList = new QMap<QString, ClientHandler*>;

	connect(this, SIGNAL(SIGNAL_REQUIRE_AUTHENTICATE_CODE()), mUiManager,	
		SLOT(OnAuthenticateCodeRequired()));
	connect(this, SIGNAL(SIGNAL_ON_RECONNECTED(COURIER::SOCKET_CLIENT_INFO)), mUiManager,	
		SLOT(onReconnected(COURIER::SOCKET_CLIENT_INFO)));
}

void ConnectionManager::OnReceiveMessage( QString message )
{

}

void ConnectionManager::OnClientExit()
{
	if (ClientHandler *client = qobject_cast<ClientHandler *>(sender()))
		removeConnection(client);
}

void ConnectionManager::OnDoorKnocked( COURIER::SOCKET_CLIENT_INFO info )
{
	QString clientId = getClientId(info);
	qDebug()<<"Qt: ID for "<<info.ip<<" port: "<<info.port<<" is: "<<clientId;
	QMutexLocker locker(&mKnockedClientList_mutex);
	//TODO: QMap��insert�����Ƿ���Ҫ�����̰߳�ȫ
	mKnockedClientList->insert(clientId, info);
	newSessionCheck();
}

void ConnectionManager::OnReconnect( COURIER::SOCKET_CLIENT_INFO clientInfo)
{
	QString id = getClientId(clientInfo);

	// ��ǰû�ж�Ӧid����֪�ͻ���
	if(clientInfo.ip.toString().compare("") == 0)
		return;

	ClientHandler * client =  new ClientHandler(id, clientInfo, "no_auth_code");
	client->init();
	QMutexLocker locker(&mClientConnectedList_mutex);
	mClientConnectedList->insert(id, client);

	// ֪ͨ�����ɹ�
	emit SIGNAL_ON_RECONNECTED(clientInfo);
}

/************************************************************************/
/* ��ip��ַ16���������λ��imxi������λ�ϲ���ΪClient��ID
/************************************************************************/
QString ConnectionManager::getClientId( COURIER::SOCKET_CLIENT_INFO info )
{
	qint32 ip = info.ip.toIPv4Address();
	QString ipId = QString::number(ip, 16);
	ipId = ipId.right(2);
	QString imxiId = info.imxi.right(4).left(2);
	return (ipId + imxiId);
}

qint32 ConnectionManager::getClientConnectedCount()
{
	return mClientConnectedList->count();
}

void ConnectionManager::newSessionCheck()
{
	//�����ǰû�ÿͻ������ӣ��򵯳�Ҫ���û�����������ĶԻ���
	if(getClientConnectedCount() == 0 && mKnockedClientList->count() > 0)
		emit SIGNAL_REQUIRE_AUTHENTICATE_CODE();
}

void ConnectionManager::onAuthenticateCodeEntered( QVariant authCode )
{
	QString id = getClientCodeFromAuthCode(authCode.toString());
	COURIER::SOCKET_CLIENT_INFO clientInfo = mKnockedClientList->value(id);
	
	// ��ǰû�ж�Ӧid����֪�ͻ���
	if(clientInfo.ip.toString().compare("") == 0)
		return;

	ClientHandler * client =  new ClientHandler(id, clientInfo, authCode.toString());
	client->init();
	QMutexLocker locker(&mClientConnectedList_mutex);
	mClientConnectedList->insert(id, client);
}

QString ConnectionManager::getClientCodeFromAuthCode( QString code )
{
	return (code.mid(0,1) + code.mid(2,1) + code.mid(4,2));
}

void ConnectionManager::removeConnection( ClientHandler * client )
{
	if (mKnockedClientList->keys().contains(client->clientId()))
	{
		// ����֪�ͻ����б���ɾ������Ϊ�Ͽ�����һ����ζ��session�������豸����
		QMutexLocker locker(&mKnockedClientList_mutex);
		mKnockedClientList->remove(client->clientId());

		// �������豸�б���ɾ��
		QMutexLocker lockerB(&mClientConnectedList_mutex);
		mClientConnectedList->remove(client->clientId());
	}
	client->deleteLater();
	newSessionCheck();
}

void ConnectionManager::connectionError( QAbstractSocket::SocketError e)
{
	qDebug()<<"Qt: SocketError "<<e<<" from sender"<<qobject_cast<ClientHandler *>(sender());
	if (ClientHandler *client = qobject_cast<ClientHandler *>(sender()))
		removeConnection(client);
}

void ConnectionManager::onMessagReplySubmittedByUser( QString clientId, COURIER::MESSAGE_ITEM msg)
{
	QMutexLocker locker(&mClientConnectedList_mutex);
	ClientHandler * handler = mClientConnectedList->value(clientId);
	handler->sendMessage(msg);
}
