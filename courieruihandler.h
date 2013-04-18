#ifndef COURIERUIHANDLER_H
#define COURIERUIHANDLER_H

#include <QObject>
#include <QDeclarativeView>
#include <QVariant>
#include "doorkeeper.h"
#include "connectionmanager.h"

class DoorKeeper;
class ConnectionManager;

class CourierUiHandler : public QObject
{
    Q_OBJECT
public:
	explicit CourierUiHandler(QObject *parent = 0);
    //static CourierUiHandler * getInstance();
    //static void init();
	virtual void setUpSignalSlot(QDeclarativeView * showView) { mView = showView; };
	virtual void cleanUp() {};

private:
    //static CourierUiHandler * instance;
protected:
	QDeclarativeView * mView;
	DoorKeeper * mDoorKeeper;
	ConnectionManager * mConnectionManager;

public slots:
	Q_INVOKABLE void onAuthenticateCodeRequired() {};
	Q_INVOKABLE void onExit() {};

signals:
};

#endif // COURIERUIHANDLER_H
