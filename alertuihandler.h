#ifndef ALERTUIHANDLER_H
#define ALERTUIHANDLER_H

#include "courieruihandler.h"

class AlertUiHandler : public CourierUiHandler
{
	Q_OBJECT

public:
	AlertUiHandler(QObject *parent);
	~AlertUiHandler();
	virtual void setUpSignalSlot( QDeclarativeView * showView );
	virtual void cleanUp();

private:

public slots:
	Q_INVOKABLE void onClosing();

signals:
};

#endif // ALERTUIHANDLER_H
