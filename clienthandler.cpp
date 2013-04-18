#include "clienthandler.h"
#include <qca.h>
#include "json.h"
#include "doorkeeper.h"

ClientHandler::ClientHandler(QString id, SOCKET_CLIENT_INFO clientInfo, QString authCode, QObject *parent)
	: QTcpSocket(parent)
{
	mClientInfo = clientInfo;
	mID = id;
	mAuthCode = authCode;
	isEnableEncrypt = false;

	// for test purpose
	//mClientInfo.ip = QHostAddress("192.168.0.169");
}

ClientHandler::~ClientHandler()
{

}

void ClientHandler::init()
{
	mClientState = Connecting;
	connect(this, SIGNAL(readyRead()), this, SLOT(processReadyRead()));
	connect(this, SIGNAL(error(QAbstractSocket::SocketError)), ConnectionManager::getInstance(), SLOT(connectionError(QAbstractSocket::SocketError)));
	connect(this, SIGNAL(disconnected()), ConnectionManager::getInstance(), SLOT(OnClientExit()));

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

SOCKET_CLIENT_INFO ClientHandler::info() const
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
	QString type = json["type"].toString();
	if (type.compare("NeedEncryptionOption"))
		return NeedEncryptionOption;
	if (type.compare("NewTextMessagePack"))
		return NewTextMessagePack;
	if (type.compare("ClientExit"))
		return ClientExit;
	return UnknowType;
}

QVariantMap ClientHandler::getJsonFromRawData( QByteArray raw )
{
	QByteArray deBase64 = QByteArray::fromBase64(raw);
	QString line = QString(deBase64);
	
	qDebug()<<"Qt: deBase64 result: "<<line;

	QString encryptLine = line.mid(QString(BASE64_MAGIC_STRING).length());
	QString resultLine = encryptLine;
	if (isEnableEncrypt)
	{
		QCA::Initializer init;
		QCA::SecureArray encryptArray = encryptLine.toLocal8Bit();
		if(!QCA::isSupported("pkey") || !QCA::PKey::supportedIOTypes().contains(QCA::PKey::RSA))
			qDebug() << "RSA not supported!\n";
		else 
		{
			QCA::SecureArray decrypt;
			if(0 == mPrivateKey.decrypt(encryptArray, &decrypt, QCA::EME_PKCS1_OAEP)) {
				qDebug() << "Error decrypting.\n";
			}
			qDebug()<<decrypt.data();
			resultLine = QString::fromLocal8Bit(decrypt.data());
		}
	}
	bool isParserSuccess;
	QVariantMap result = QtJson::parse(resultLine, isParserSuccess).toMap();
	if (!isParserSuccess)
		qDebug()<<"Qt: Parser JSON failed.";
	return result;
}

void ClientHandler::processData( DataType dataType, QVariantMap json )
{
	qDebug()<<"Qt: Processing Received Data: "<<dataType<<" "<<json;
	switch(dataType)
	{
		case NeedEncryptionOption:
			sendPublicKey();
			break;
		case ClientExit:
			break;
		case NewTextMessagePack:
			break;
		default:
			break;
	}
}

void ClientHandler::sendPublicKey()
{
	int tryTime = 0;
	QCA::Initializer init;
	while ((mPrivateKey.isNull() || !mPublicKey.canEncrypt()) && tryTime < MAX_KEY_GENERATOR_TIME)
	{
		mPrivateKey = QCA::KeyGenerator().createRSA(PRIVATE_KEY_LENGTH);
		mPublicKey = mPrivateKey.toPublicKey();
		tryTime++;
	}
	if (tryTime == MAX_KEY_GENERATOR_TIME)
	{
		qDebug()<<"Qt: RSA Key generating failed.";
	}
	sendString("publicKey", mPublicKey.toPEM());
}

void ClientHandler::sendString( QString k, QString v )
{
	QVariantMap map;
	map[k] = v;
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
	json += BASE64_MAGIC_STRING;
	// TODO JSON签名处理
	QByteArray msg = json.toLocal8Bit().toBase64();
	qDebug()<<"Qt: Send Bytes: "<<msg;
	return write(msg) == msg.size();
}
