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
	mTimer.setInterval(10000);
	connect(&mTimer, SIGNAL(timeout()), this, SLOT(tick()));
	mTimer.start();
}

void CourierService::init()
{
	// ��ʱ����Ҫ�����߳���ɹ�����������ղ���Ҫ�߳�ִ�У���Service���ΪQObject
	this->start();
	mDoorKeeper = DoorKeeper::getInstance();
	mDoorKeeper->start();
}

void CourierService::tick()
{

}
