#ifndef AUTHINPUTWINDOWUIHANDLER_H
#define AUTHINPUTWINDOWUIHANDLER_H

#include "courieruihandler.h"

class AuthInputWindowUiHandler : public CourierUiHandler
{
	Q_OBJECT

public:
	AuthInputWindowUiHandler(QObject *parent);
	~AuthInputWindowUiHandler();
	virtual void setUpSignalSlot( QDeclarativeView * showView );
	virtual void cleanUp();

private:

public slots:
	Q_INVOKABLE void onAuthenticateCodeAccepted(QVariant);

signals:
	void SIGNAL_AUTHENTICATE_CODE_SUBMIT(QVariant);
};

#endif // AUTHINPUTWINDOWUIHANDLER_H
