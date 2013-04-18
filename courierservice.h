#ifndef COURIERSERVICE_H
#define COURIERSERVICE_H

#include <QObject>
#include <QThread>
#include "uimanager.h"
#include "doorkeeper.h"

class CourierService : public QThread
{
    Q_OBJECT
public:
    explicit CourierService(UIManager * uiManager, QObject *parent = 0);
	virtual void run();
	void init();

private:
	UIManager * mUiManager;
	DoorKeeper * mDoorKeeper;

signals:
    
public slots:
    
};

#endif // COURIERSERVICE_H
