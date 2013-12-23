#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QWidget>
#include "qmlapplicationviewer.h"
#include "courierqmlviewer.h"
#include <QDeclarativeContext>
#include <QDebug>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeEngine>
#include "courieruihandler.h"
#include <QGraphicsObject>
#include "clienthandler.h"
#include "models.h"

class CourierQmlViewer;

class UIManager : public QWidget
{
	Q_OBJECT

public:
	UIManager(QWidget *parent = 0);
	static UIManager * getInstance();
	static void initUiManager();
	void showChatWindow();
	void showAuthenticateCodeInputWindow();
	~UIManager();

private:
	//Ui::UIManager ui;
	static UIManager * sInstance;
	CourierQmlViewer * mAuthWindowViewer;

public slots:
	Q_INVOKABLE void OnAuthenticateCodeRequired();
	Q_INVOKABLE void OnNewMessage(QString,COURIER::MESSAGE_ITEM);
	Q_INVOKABLE void OnNewMessagePack(QString,COURIER::MESSAGE_PACK);
	Q_INVOKABLE void onReconnected(COURIER::SOCKET_CLIENT_INFO);
};

#endif // UIMANAGER_H
