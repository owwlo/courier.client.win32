#include "authinputwindowuihandler.h"

AuthInputWindowUiHandler::AuthInputWindowUiHandler(QObject *parent)
	: CourierUiHandler(parent)
{

}

AuthInputWindowUiHandler::~AuthInputWindowUiHandler()
{

}

void AuthInputWindowUiHandler::setUpSignalSlot( QDeclarativeView * showView )
{
	CourierUiHandler::setUpSignalSlot(showView);
	QObject *rootObject = dynamic_cast<QObject*>(CourierUiHandler::mView->rootObject());
	connect(this, SIGNAL(SIGNAL_AUTHENTICATE_CODE_SUBMIT(QVariant)), mConnectionManager, SLOT(onAuthenticateCodeEntered(QVariant)));
}

void AuthInputWindowUiHandler::cleanUp()
{
	CourierUiHandler::cleanUp();
	QObject *rootObject = dynamic_cast<QObject*>(CourierUiHandler::mView->rootObject());
	disconnect(this, SIGNAL(SIGNAL_AUTHENTICATE_CODE_SUBMIT(QVariant)), mConnectionManager, SLOT(onAuthenticateCodeEntered(QVariant)));
}

void AuthInputWindowUiHandler::onAuthenticateCodeAccepted(QVariant inputCode)
{
	qDebug()<<"Qt: Got Authenticate Code: "<<inputCode.toString();
	emit SIGNAL_AUTHENTICATE_CODE_SUBMIT(inputCode);
}
