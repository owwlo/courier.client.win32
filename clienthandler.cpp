#include "clienthandler.h"
#include <qca.h>
#include "json.h"
#include "doorkeeper.h"
#include "base64.h"
#include <QTextCodec>

ClientHandler::ClientHandler(QString id, COURIER::SOCKET_CLIENT_INFO clientInfo, QString authCode, QObject *parent)
	: QTcpSocket(parent)
{
	mClientInfo = clientInfo;
	mID = id;
	mAuthCode = authCode;
	isEnableEncrypt = false;

	mPrivateKey = new QCA::PrivateKey;
	mPublicKey = new QCA::PublicKey;

	// for test purpose
	//mClientInfo.ip = QHostAddress("192.168.0.169");
}

ClientHandler::~ClientHandler()
{
	disconnect(UIManager::getInstance());
}

void ClientHandler::init()
{
	mClientState = Connecting;
	connect(this, SIGNAL(readyRead()), this, SLOT(processReadyRead()));
	connect(this, SIGNAL(error(QAbstractSocket::SocketError)), 
		ConnectionManager::getInstance(), SLOT(connectionError(QAbstractSocket::SocketError)));
	connect(this, SIGNAL(disconnected()), ConnectionManager::getInstance(), SLOT(OnClientExit()));
	connect(this, SIGNAL(connected()), this, SLOT(ConnectSuccess()));
	connect(this, SIGNAL(signal_ReceiveMessage(QString,COURIER::MESSAGE_ITEM)), UIManager::getInstance(), SLOT(OnNewMessage(QString,COURIER::MESSAGE_ITEM)));
	connect(this, SIGNAL(signal_ReceiveMessagePack(QString,COURIER::MESSAGE_PACK)), UIManager::getInstance(), SLOT(OnNewMessagePack(QString,COURIER::MESSAGE_PACK)));

	qDebug()<<"Qt: connect to "<<mClientInfo.port<<"@"<<mClientInfo.ip;
	connectToHost(mClientInfo.ip, mClientInfo.port);
}

ClientHandler::ClientState ClientHandler::state() const
{
	return mClientState;
}

void ClientHandler::processReadyRead()
{
	while(canReadLine())
	{
		QByteArray line = readLine();
		if (!checkCommandValided(line))
			continue;
		qDebug()<<"Qt: got line from "<<mClientInfo.ip<<" :"<<line;
		QVariantMap json = getJsonFromRawData(line);
		DataType dataType = getCommandType(json);
		processData(dataType, json);
	}
}

COURIER::SOCKET_CLIENT_INFO ClientHandler::info() const
{
	return mClientInfo;
}

QString ClientHandler::clientId() const
{
	return mID;
}

bool ClientHandler::checkCommandValided( QByteArray line )
{
	// TODO验证数据完整性
	return true;
}

ClientHandler::DataType ClientHandler::getCommandType( QVariantMap json )
{
	QString type = json["json_type"].toString();
	if (!type.compare("NeedEncryptionOption"))
		return NeedEncryptionOption;
	if (!type.compare("NewTextMessagePack"))
		return NewTextMessagePack;
	if (!type.compare("NewTextMessage"))
		return NewTextMessage;
	if (!type.compare("ClientExit"))
		return ClientExit;
	if (!type.compare("AESKey"))
		return AESKey;
	if (!type.compare("test"))
		return test;
	return UnknowType;
}

void ClientHandler::processData( DataType dataType, QVariantMap json )
{
	qDebug()<<"Qt: Processing Received Data: "<<dataType<<" "<<json;
	switch(dataType)
	{
	case NeedEncryptionOption:
		//当前状态PC已将publicKey发送给手机，等待手机返回
		mClientState = WaitingForAESKey;
		sendPublicKey();
		break;
	case ClientExit:
		break;
	case NewTextMessagePack:
		emit signal_ReceiveMessagePack(mID, parseMessagePack(json));
		break;
	case NewTextMessage:
		emit signal_ReceiveMessage(mID, parseMessageItem(json));
		break;
	case AESKey:
		bool isKeyParsed;
		isKeyParsed = parseCommunicationKey(json);
		if (isKeyParsed)
		{
			mClientState = Connected;
		}
		break;
	case test:
		qDebug()<<"Qt: test message received: "<<json;
		break;
	default:
		break;
	}
}

QVariantMap ClientHandler::getJsonFromRawData( QByteArray raw )
{
	QByteArray deBase64 = QByteArray::fromBase64(raw);
	//QString line = QString(deBase64);
	qDebug()<<"Qt: deBase64: "<<QString(deBase64);

	//TODO 采用了开源的BASE64实现，改成Qt内部类调用
	//QString line = QString::fromStdString(base64_decode(QString(raw).toStdString()));
	//qDebug()<<"Qt: deBase64 result: "<<line;

	//QString encryptLine = line.mid(QString(BASE64_MAGIC_STRING).length());
	QByteArray resultLine = deBase64.mid(QString(BASE64_MAGIC_STRING).toLocal8Bit().length());
	qDebug()<<QCA::arrayToHex(resultLine);
	if (isEnableEncrypt)
	{
		QCA::Cipher cipher(QString("aes256"),QCA::Cipher::CBC, QCA::Cipher::PKCS7,
			QCA::Decode, *mKey, *mIv);
		QCA::SecureArray u = cipher.update(resultLine);
		QCA::SecureArray f = cipher.final();
		QCA::SecureArray plainText = u.append(f);
		resultLine = plainText.toByteArray();
		qDebug()<<"Qt: Decrypt: "<<QString(resultLine);
	}
	bool isParserSuccess;
	qDebug()<<"Qt: Decrypt: "<<QString::fromUtf8(resultLine);
	QVariantMap result = QtJson::parse(QString::fromUtf8(resultLine), isParserSuccess).toMap();
	if (!isParserSuccess)
		qDebug()<<"Qt: Parser JSON failed.";
	return result;
}

void ClientHandler::sendPublicKey()
{
	int tryTime = 0;

	//放在堆上，析构函数执行清理工作
	new QCA::Initializer;
	
	while ((mPrivateKey->isNull() || !mPublicKey->canEncrypt()) && tryTime < MAX_KEY_GENERATOR_TIME)
	{
		mPrivateKey = new QCA::PrivateKey(QCA::KeyGenerator().createRSA(PRIVATE_KEY_LENGTH));
		QCA::PrivateKey * cpy = new QCA::PrivateKey(QCA::PrivateKey::fromPEM(mPrivateKey->toPEM()));
		mPublicKey = new QCA::PublicKey(cpy->toPublicKey());
		tryTime++;
	}
	if (tryTime == MAX_KEY_GENERATOR_TIME)
	{
		qDebug()<<"Qt: RSA Key generating failed.";
	}
	sendString(JSON_TYPE_PUBLIC_KEY, JSON_VALUE, mPublicKey->toPEM());
}

void ClientHandler::sendString( QString type, QString k, QString v )
{
	QVariantMap map;
	map[k] = v;
	map[JSON_TYPE] = type;
	QByteArray data = QtJson::serialize(map);
	sendJson(QString(data));
}

bool ClientHandler::sendJson( QString json )
{
	qDebug()<<"Qt: Send Json to client: "<<json;
	if(isEnableEncrypt)
	{
		// TODO JSON加密处理
		qDebug()<<"Qt: Encrypt with key.";
	}
	//加上包装头，TODO 这部分可以在增加签名功能之后去掉
	json = BASE64_MAGIC_STRING + json;
	// TODO JSON签名处理
	QByteArray msg = json.toUtf8().toBase64();
	qDebug()<<"Qt: Send Bytes: "<<msg;

	//TODO 必须要加上Line Feed和Carriage Return吗？
	msg += "\n\r";

	bool isSendSuccess = write(msg) == msg.size();
	return isSendSuccess;
}

bool ClientHandler::parseCommunicationKey( QVariantMap json )
{
	QString aesKeyStr = json[JSON_VALUE].toString();
	QByteArray deBase64 = QByteArray::fromHex(QByteArray::fromBase64(aesKeyStr.toLocal8Bit()));

	//QCA::Initializer init;
	QCA::SecureArray decrypt;
	mPrivateKey->decrypt(QCA::SecureArray(deBase64),&decrypt, QCA::EME_PKCS1v15);
	qDebug()<<"Qt: AESKey Decoded: "<<decrypt.toByteArray();
	mKey = new QCA::SymmetricKey(decrypt);
	static const char mydata[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	mIv = new QCA::InitializationVector(QByteArray(mydata));
	
	//TODO 判断获取KEY成功与否

	isEnableEncrypt = true;

	//执行到现在，已经成功Parse到Key
	return true;
}

void ClientHandler::ConnectSuccess()
{
	mClientState = Connected;
}

COURIER::MESSAGE_PACK ClientHandler::parseMessagePack( QVariantMap json )
{
	COURIER::MESSAGE_PACK pack;
	pack.rawJSON = json;

	pack.senderAddress = json[JSON_MSG_PACK_SENDER].toString();
	pack.threadId = json[JSON_MSG_PACK_THREAD_ID].toInt();
	pack.senderId = json[JSON_MSG_PACK_SENDER_ID].toString();
	
	QVariantList msgList = json[JSON_MSG_PACK_MSG_ARRAY].toList();
	QList<COURIER::MESSAGE_ITEM> msgItemList;
	foreach (QVariant entry, msgList)
	{
		QVariantMap entryMap = entry.toMap();
		msgItemList.append(parseMessageItem(entryMap));
	}
	pack.messageArray = msgItemList;

	QVariantMap imageMap = json[JSON_MSG_PACK_IMG_ARRAY].toMap();
	foreach (QString key, imageMap.keys())
	{
		QImage image;
		image.loadFromData(QByteArray::fromBase64(imageMap[key].toByteArray()), "png");
		pack.userImages.insert(key, image);
	}

	return pack;
}

COURIER::MESSAGE_ITEM ClientHandler::parseMessageItem( QVariantMap json )
{
	COURIER::MESSAGE_ITEM item;
	item.address = json[JSON_MESSAGE_ITEM_ADDRESS].toString();
	item.body = json[JSON_MESSAGE_ITEM_BODY].toString();
	item.date = json[JSON_MESSAGE_ITEM_DATE].toLongLong();
	item.dateSent = json[JSON_MESSAGE_ITEM_DATESENT].toLongLong();
	item.deleted = json[JSON_MESSAGE_ITEM_DELETED].toInt();
	item.id = json[JSON_MESSAGE_ITEM_ID].toInt();
	item.protocol = json[JSON_MESSAGE_ITEM_PROTOCOL].toInt();
	item.read = json[JSON_MESSAGE_ITEM_READ].toInt();
	item.seen = json[JSON_MESSAGE_ITEM_SEEN].toInt();
	item.status = json[JSON_MESSAGE_ITEM_STATUS].toInt();
	item.threadId = json[JSON_MESSAGE_ITEM_THREADID].toInt();
	item.type = json[JSON_MESSAGE_ITEM_TYPE].toInt();
	item.personId = json[JSON_MESSAGE_PERSON_ID].toString();

	if (!json[JSON_MESSAGE_USER_IMAGE].isNull())
	{
		QImage image;
		image.loadFromData(QByteArray::fromBase64(json[JSON_MESSAGE_USER_IMAGE].toByteArray()), "jpg");
		item.user_image = image;
	}
	return item;
}

void ClientHandler::sendMessage( COURIER::MESSAGE_ITEM item)
{
	QVariantMap map;
	map[JSON_TYPE] = JSON_TYPE_REPLY_MSG;
	map[JSON_MESSAGE_ITEM_ADDRESS] = item.address;
	map[JSON_MESSAGE_ITEM_BODY] = item.body;
	QTextCodec *tc = QTextCodec::codecForName("UTF-8");
	QByteArray data = QtJson::serialize(map);
	sendJson(tc->toUnicode(data));
}
