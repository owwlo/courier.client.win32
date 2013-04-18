#include "courierservice.h"
#include "qmlapplicationviewer.h"
#include <QDeclarativeContext>
#include <QDebug>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeEngine>
#include "courieruihandler.h"
#include <QGraphicsObject>

CourierService::CourierService(UIManager * uiManager, QObject *parent) :
QThread(parent)
{
	mUiManager = uiManager;
}

void CourierService::run()
{
	
}

void CourierService::init()
{
	// 暂时不需要启动线程完成工作，如果最终不需要线程执行，将Service类改为QObject
	//this->start();
	mDoorKeeper = DoorKeeper::getInstance();
	mDoorKeeper->start();
}
