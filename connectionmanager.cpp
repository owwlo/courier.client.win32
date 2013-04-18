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
	
	mKnockedClientList = new QMap<QString, SOCKET_CLIENT_INFO>;
	mClientConnectedList = new QMap<QString, ClientHandler*>;

	connect(this, SIGNAL(SIGNAL_REQUIRE_AUTHENTICATE_CODE()), mUiManager, SLOT(OnAuthenticateCodeRequired()));
}

void ConnectionManager::OnReceiveMessage( QString message )
{

}

void ConnectionManager::OnClientExit()
{
	if (ClientHandler *client = qobject_cast<ClientHandler *>(sender()))
		removeConnection(client);
}

void ConnectionManager::OnDoorKnocked( SOCKET_CLIENT_INFO info )
{
	QString clientId = getClientId(info);
	qDebug()<<"Qt: ID for "<<info.ip<<" port: "<<info.port<<" is: "<<clientId;
	QMutexLocker locker(&mKnockedClientList_mutex);
	//TODO: QMap的insert函数是否需要控制线程安全
	mKnockedClientList->insert(clientId, info);
	newSessionCheck();
}

/************************************************************************/
/* 将ip地址16进制最后两位和imxi倒数两位合并做为Client的ID
/************************************************************************/
QString ConnectionManager::getClientId( SOCKET_CLIENT_INFO info )
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
	//如果当前没用客户端连接，则弹出要求用户输入链接码的对话框
	if(getClientConnectedCount() == 0 && mKnockedClientList->count() > 0)
		emit SIGNAL_REQUIRE_AUTHENTICATE_CODE();
}

void ConnectionManager::onAuthenticateCodeEntered( QVariant authCode )
{
	QString id = getClientCodeFromAuthCode(authCode.toString());
	SOCKET_CLIENT_INFO clientInfo = mKnockedClientList->value(id);
	
	// 当前没有对应id的已知客户端
	if(clientInfo.ip.toString() == "")
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
		// 从已知客户端列表里删除，因为断开连接一般意味着session结束、设备下线
		QMutexLocker locker(&mKnockedClientList_mutex);
		mKnockedClientList->remove(client->clientId());

		// 从连接设备列表里删除
		QMutexLocker lockerB(&mClientConnectedList_mutex);
		mClientConnectedList->remove(client->clientId());
	}
	client->deleteLater();
	newSessionCheck();
}

void ConnectionManager::connectionError( QAbstractSocket::SocketError )
{
	if (ClientHandler *client = qobject_cast<ClientHandler *>(sender()))
		removeConnection(client);
}