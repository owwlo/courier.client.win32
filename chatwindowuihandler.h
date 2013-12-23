#ifndef CHATWINDOWUIHANDLER_H
#define CHATWINDOWUIHANDLER_H

#include "courieruihandler.h"
#include "models.h"

class ChatWindowUiHandler : public CourierUiHandler
{
	Q_OBJECT

public:
	ChatWindowUiHandler(QObject *parent);
	~ChatWindowUiHandler();
	virtual void setUpSignalSlot( QDeclarativeView * showView );
	virtual void cleanUp();
	void setMessage( COURIER::MESSAGE_ITEM item );
	void setMessagePack( COURIER::MESSAGE_PACK pack );
	void setClientID( QString clientId );

private:
	COURIER::CHAT_WINDOW_DATA_TYPE DATA_TYPE;
	COURIER::MESSAGE_ITEM msgItem;
	COURIER::MESSAGE_PACK msgPack;
	QString mClientId;
	QDeclarativeView * mView;

public slots:
	Q_INVOKABLE void onQMLReplyMessageAccepted(QString);

signals:
	void SIGNAL_MESSAGE_SEND_RECALL(QVariant);
	void UPDATE_QML_DATA_MESSAGE(QVariantMap);
	void SIGNAL_MESSAGE_REPLY_SUBMITTED(QString clientId,COURIER::MESSAGE_ITEM);
};

#endif // CHATWINDOWUIHANDLER_H
