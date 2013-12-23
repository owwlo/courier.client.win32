#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QTcpSocket>
#include <QAbstractSocket>
#include <qca.h>
#include "connectionmanager.h"
#include "models.h"

static const int MaxBufferSize = 1024000;
static const int PRIVATE_KEY_LENGTH = 1024;
static const int MAX_KEY_GENERATOR_TIME = 5;

static const QString JSON_PORT	= "port";
static const QString JSON_IMXI	= "imxi";
static const QString JSON_TYPE	= "json_type";
static const QString JSON_VALUE	= "value";

static const QString JSON_TYPE_NEED_ENCRYPT	= "NeedEncryptionOption";
static const QString JSON_TYPE_PUBLIC_KEY	= "PublicKey";
static const QString JSON_TYPE_AES_KEY		= "AESKey";
static const QString JSON_TYPE_REPLY_MSG	= "ReplyMsg";

static const QString JSON_MESSAGE_ITEM_ADDRESS	= "address";
static const QString JSON_MESSAGE_ITEM_BODY		= "body";
static const QString JSON_MESSAGE_ITEM_DATE		= "date";
static const QString JSON_MESSAGE_ITEM_DATESENT	= "date_sent";
static const QString JSON_MESSAGE_ITEM_DELETED	= "deleted";
static const QString JSON_MESSAGE_ITEM_ID		= "_id";
static const QString JSON_MESSAGE_ITEM_PROTOCOL	= "protocol";
static const QString JSON_MESSAGE_ITEM_READ		= "read";
static const QString JSON_MESSAGE_ITEM_SEEN		= "seen";
static const QString JSON_MESSAGE_ITEM_STATUS	= "status";
static const QString JSON_MESSAGE_ITEM_THREADID	= "thread_id";
static const QString JSON_MESSAGE_ITEM_TYPE		= "type";
static const QString JSON_MESSAGE_USER_IMAGE	= "user_image";
static const QString JSON_MESSAGE_PERSON_ID		= "person";

static const QString JSON_MSG_PACK_SENDER		= "senderAddress";
static const QString JSON_MSG_PACK_SENDER_ID	= "senderId";
static const QString JSON_MSG_PACK_THREAD_ID	= "threadId";
static const QString JSON_MSG_PACK_MSG_ARRAY	= "messageArray";
static const QString JSON_MSG_PACK_IMG_ARRAY	= "imageArray";

struct COURIER::SOCKET_CLIENT_INFO;

class ClientHandler : public QTcpSocket
{
	Q_OBJECT
	
public:
	enum ClientState {
		Connecting,
		Authenticating,
		Connected,
		WaitingForAESKey
	};
	enum DataType {
		NeedEncryptionOption,
		NewTextMessagePack,
		NewTextMessage,
		ClientExit,
		UnknowType,
		AESKey,
		test
	};
	ClientState state() const;
	COURIER::SOCKET_CLIENT_INFO info() const;
	QString clientId() const;
	ClientHandler(QString id, COURIER::SOCKET_CLIENT_INFO clientInfo, QString authCode, QObject *parent = 0);
	void init();
	void setUpClientListener(/*ClientListener * listener*/);
	void sendMessage(COURIER::MESSAGE_ITEM);
	~ClientHandler();
private:
	//ClientListener * mClientListener;
	QString mID;
	COURIER::SOCKET_CLIENT_INFO mClientInfo;
	QString mAuthCode;
	ClientState mClientState;
	bool checkCommandValided(QByteArray);
	DataType getCommandType(QVariantMap);
	QVariantMap getJsonFromRawData( QByteArray line );
	void processData( DataType dataType, QVariantMap json );
	bool isEnableEncrypt;

	QCA::SymmetricKey * mKey;
	QCA::InitializationVector * mIv;
	QCA::PrivateKey * mPrivateKey;
	QCA::PublicKey * mPublicKey;
	
	void sendPublicKey();
	void sendString(QString, QString, QString );
	bool sendJson( QString );
	bool parseCommunicationKey( QVariantMap json );
	COURIER::MESSAGE_ITEM parseMessageItem( QVariantMap json );
	COURIER::MESSAGE_PACK parseMessagePack( QVariantMap json );

public slots:
	void processReadyRead();
	void ConnectSuccess();

signals:
	void signal_ReceiveMessage(QString id,COURIER::MESSAGE_ITEM message);
	void signal_ReceiveMessagePack(QString id,COURIER::MESSAGE_PACK pack);
	void signal_ClientExit(QString id);
};

#endif // CLIENTHANDLER_H
