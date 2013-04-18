#ifndef CHATWINDOWUIHANDLER_H
#define CHATWINDOWUIHANDLER_H

#include "courieruihandler.h"

class ChatWindowUiHandler : public CourierUiHandler
{
	Q_OBJECT

public:
	ChatWindowUiHandler(QObject *parent);
	~ChatWindowUiHandler();
	virtual void setUpSignalSlot( QDeclarativeView * showView );
	virtual void cleanUp();

private:

public slots:
	Q_INVOKABLE void onQMLReplyMessageAccepted(QString);

signals:
	void SIGNAL_MESSAGE_SEND_RECALL(QVariant);
};

#endif // CHATWINDOWUIHANDLER_H
