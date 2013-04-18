#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QTcpSocket>
#include "connectionmanager.h"
#include <QAbstractSocket>
#include <qca.h>

struct SOCKET_CLIENT_INFO;

static const int MaxBufferSize = 1024000;
static const int PRIVATE_KEY_LENGTH = 1024;
static const int MAX_KEY_GENERATOR_TIME = 5;

class ClientHandler : public QTcpSocket
{
	Q_OBJECT
	
public:
	enum ClientState {
		Connecting,
		Authenticating,
		Connected
	};
	enum DataType {
		NeedEncryptionOption,
		NewTextMessagePack,
		ClientExit,
		UnknowType
	};
	ClientState state() const;
	SOCKET_CLIENT_INFO info() const;
	QString clientId() const;
	ClientHandler(QString id, SOCKET_CLIENT_INFO clientInfo, QString authCode, QObject *parent = 0);
	void init();
	void setUpClientListener(/*ClientListener * listener*/);
	~ClientHandler();
private:
	//ClientListener * mClientListener;
	QString mID;
	SOCKET_CLIENT_INFO mClientInfo;
	QString mAuthCode;
	ClientState mClientState;
	bool checkCommandValided(QByteArray);
	DataType getCommandType(QVariantMap);
	QVariantMap getJsonFromRawData( QByteArray line );
	void processData( DataType dataType, QVariantMap json );
	bool isEnableEncrypt;
	QString mKey;
	QCA::PrivateKey mPrivateKey;
	QCA::PublicKey mPublicKey;
	void sendPublicKey();
	void sendString( QString, QString );
	bool sendJson( QString );

public slots:
	void processReadyRead();

signals:
	void signal_ReceiveMessage(QString message);
	void signal_ClientExit(QString id);
};

#endif // CLIENTHANDLER_H
