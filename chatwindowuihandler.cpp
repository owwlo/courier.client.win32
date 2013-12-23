#include "chatwindowuihandler.h"
#include <QDebug>
#include <QDeclarativeView>
#include <QGraphicsObject>
#include "models.h"
#include "userimageprovider.h"

ChatWindowUiHandler::ChatWindowUiHandler(QObject *parent)
	: CourierUiHandler(parent)
{
	DATA_TYPE = COURIER::NODATA;
	mView = nullptr;

	//将用户回复的信息返回给ConnectionManager
	connect(this, SIGNAL(SIGNAL_MESSAGE_REPLY_SUBMITTED(QString,COURIER::MESSAGE_ITEM)), 
		ConnectionManager::getInstance(), SLOT(onMessagReplySubmittedByUser( QString, COURIER::MESSAGE_ITEM )));

}

ChatWindowUiHandler::~ChatWindowUiHandler()
{
	disconnect(ConnectionManager::getInstance());
}

/************************************************************************/
/* 该函数会在窗口showWindow的时候调用
/************************************************************************/
void ChatWindowUiHandler::setUpSignalSlot( QDeclarativeView * showView )
{
	//仅当使用setUpSignalSlot后才需要removeImageProvider
	mView = showView;
	
	CourierUiHandler::setUpSignalSlot(showView);
	QObject *rootObject = dynamic_cast<QObject*>(CourierUiHandler::mView->rootObject());
	connect(this, SIGNAL(SIGNAL_MESSAGE_SEND_RECALL(QVariant)), rootObject, SLOT(messageSendRecall(QVariant)));
	if (DATA_TYPE != COURIER::NODATA)
	{
		QVariantMap data;
		UserImageProvider * imgProvider = UserImageProvider::getInstance();

		if (DATA_TYPE == COURIER::MESSAGE)
		{
			data["type"] = COURIER::MESSAGE;

			data["address"] = msgItem.address;
			data["body"] = msgItem.body;
			data["date"] = msgItem.date;
			data["dateSent"] = msgItem.dateSent;
			data["userId"] = msgItem.id;
			imgProvider->putImage(QString::number(msgItem.id), msgItem.user_image);
		} else if (DATA_TYPE == COURIER::MESSAGE_PACKAGE)
		{
			data["type"] = COURIER::MESSAGE_PACKAGE;

			data["senderId"] = msgPack.senderId;
			data["senderAddress"] = msgPack.senderAddress;
			data["threadId"] = msgPack.threadId;

			QVariantList messageArray;
			foreach (COURIER::MESSAGE_ITEM item, msgPack.messageArray)
			{
				QVariantMap mapPart;
				mapPart["address"] = item.address;
				mapPart["body"] = item.body;
				mapPart["date"] = item.date;
				mapPart["person"] = item.personId;
				messageArray.push_back(mapPart);
			}
			data["messageArray"] = messageArray;

			//data["innerJson"] = qvariant_cast<QVariant>(msgPack.rawJSON);

			//QVariant aa = QVariant::fromValue(data);
			//QVariantMap bb = aa.toMap();
			//QVariantList cc = bb["messageArray"].toList();
			//QVariantMap dd = cc.at(0).toMap();
			//qDebug()<<dd["address"];

			foreach (QString pId, msgPack.userImages.keys())
			{
				imgProvider->putImage(pId, msgPack.userImages[pId]);
			}
			showView->engine()->addImageProvider("userImage", UserImageProvider::getInstance());
		}
		//connect(this, SIGNAL(UPDATE_QML_DATA_MESSAGE(QVariantMap)), rootObject, SLOT(updateMessage(QVariantMap)));
		//emit UPDATE_QML_DATA_MESSAGE(data);
		QMetaObject::invokeMethod(rootObject, "updateMessage", Q_ARG(QVariant, QVariant::fromValue(data)));
	}
}

void ChatWindowUiHandler::cleanUp()
{
	CourierUiHandler::cleanUp();
	QObject *rootObject = dynamic_cast<QObject*>(CourierUiHandler::mView->rootObject());
	disconnect(this, SIGNAL(SIGNAL_MESSAGE_SEND_RECALL(QVariant)), rootObject, SLOT(messageSendRecall(QVariant)));
	if (mView != nullptr)
	{
		mView->engine()->removeImageProvider("userImage");
	}
}

void ChatWindowUiHandler::onQMLReplyMessageAccepted(QString msg)
{
	qDebug()<<"Message Accepted from QT: " + msg;
	emit SIGNAL_MESSAGE_SEND_RECALL("ok");
	COURIER::MESSAGE_ITEM item;
	item.body = msg;
	if (DATA_TYPE == COURIER::MESSAGE)
	{
		item.address = msgItem.address;
	} 
	else if (DATA_TYPE == COURIER::MESSAGE_PACKAGE)
	{
		item.address = msgPack.senderAddress;
	}
	emit SIGNAL_MESSAGE_REPLY_SUBMITTED(mClientId, item);
}

void ChatWindowUiHandler::setMessage( COURIER::MESSAGE_ITEM item )
{
	msgItem = item;
	DATA_TYPE = COURIER::MESSAGE;
}

void ChatWindowUiHandler::setClientID( QString clientId )
{
	mClientId = clientId;
}

void ChatWindowUiHandler::setMessagePack( COURIER::MESSAGE_PACK pack )
{
	msgPack = pack;
	DATA_TYPE = COURIER::MESSAGE_PACKAGE;
}
