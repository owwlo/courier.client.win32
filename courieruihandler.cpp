#include "courieruihandler.h"

CourierUiHandler::CourierUiHandler(QObject *parent)
	:QObject(parent)
{
	mDoorKeeper = DoorKeeper::getInstance();
	mConnectionManager = ConnectionManager::getInstance();
}