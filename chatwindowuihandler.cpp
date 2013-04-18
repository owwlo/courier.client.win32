#include "chatwindowuihandler.h"
#include <QDebug>
#include <QDeclarativeView>
#include <QGraphicsObject>

ChatWindowUiHandler::ChatWindowUiHandler(QObject *parent)
	: CourierUiHandler(parent)
{

}

ChatWindowUiHandler::~ChatWindowUiHandler()
{

}

void ChatWindowUiHandler::setUpSignalSlot( QDeclarativeView * showView )
{
	CourierUiHandler::setUpSignalSlot(showView);
	QObject *rootObject = dynamic_cast<QObject*>(CourierUiHandler::mView->rootObject());
	connect(this, SIGNAL(SIGNAL_MESSAGE_SEND_RECALL(QVariant)), rootObject, SLOT(messageSendRecall(QVariant)));
}

void ChatWindowUiHandler::cleanUp()
{
	CourierUiHandler::cleanUp();
	QObject *rootObject = dynamic_cast<QObject*>(CourierUiHandler::mView->rootObject());
	disconnect(this, SIGNAL(SIGNAL_MESSAGE_SEND_RECALL(QVariant)), rootObject, SLOT(messageSendRecall(QVariant)));
}

void ChatWindowUiHandler::onQMLReplyMessageAccepted(QString msg)
{
	qDebug()<<"Message Accepted from QT: " + msg;
	emit SIGNAL_MESSAGE_SEND_RECALL("ok");
}