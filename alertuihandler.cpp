#include "alertuihandler.h"

AlertUiHandler::AlertUiHandler(QObject *parent)
	: CourierUiHandler(parent)
{

}

AlertUiHandler::~AlertUiHandler()
{

}

void AlertUiHandler::setUpSignalSlot( QDeclarativeView * showView )
{
	CourierUiHandler::setUpSignalSlot(showView);
	QObject *rootObject = dynamic_cast<QObject*>(CourierUiHandler::mView->rootObject());
}

void AlertUiHandler::cleanUp()
{
	CourierUiHandler::cleanUp();
	QObject *rootObject = dynamic_cast<QObject*>(CourierUiHandler::mView->rootObject());
}

void AlertUiHandler::onClosing()
{

}
