#ifndef MODELS_H
#define MODELS_H

#include <QString>
#include <QHostAddress>
#include <QImage>
#include <QList>

namespace COURIER
{
	struct MESSAGE_ITEM 
	{
		QString address;
		QString body;
		long date;
		long dateSent;
		int deleted;
		int id;
		int protocol;
		int read;
		int seen;
		int status;
		long threadId;
		int type;
		QImage user_image;
		QString personId;
	};

	struct MESSAGE_PACK
	{
		QList<COURIER::MESSAGE_ITEM> messageArray;
		QString senderAddress;
		QMap<QString, QImage> userImages;
		long threadId;
		QString senderId;

		QVariantMap rawJSON;
	};

	enum CLIENT_TYPE
	{
		CLIENT_TYPE_SOCKET,
		CLIENT_TYPE_HTTP
	};

	struct CLIENT_INFO
	{
		CLIENT_TYPE type;
	};

	struct SOCKET_CLIENT_INFO : CLIENT_INFO
	{
		QHostAddress ip;
		quint16 port;
		QString imxi;

		bool operator==(const struct SOCKET_CLIENT_INFO &info) const
		{
			if(ip != info.ip) return false;
			if(port != info.port) return false;
			if(imxi.compare(info.imxi) != 0) return false;
			else return true;
		}
	};
	
	enum CHAT_WINDOW_DATA_TYPE
	{
		MESSAGE,
		MESSAGE_PACKAGE,
		NODATA
	};
}

Q_DECLARE_METATYPE(COURIER::MESSAGE_ITEM);
Q_DECLARE_METATYPE(COURIER::MESSAGE_PACK);
Q_DECLARE_METATYPE(COURIER::SOCKET_CLIENT_INFO);
Q_DECLARE_METATYPE(COURIER::CLIENT_INFO);
Q_DECLARE_METATYPE(COURIER::CHAT_WINDOW_DATA_TYPE);

#endif