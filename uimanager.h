#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QWidget>
//#include "ui_uimanager.h"
#include "qmlapplicationviewer.h"
#include "courierqmlviewer.h"
#include <QDeclarativeContext>
#include <QDebug>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeEngine>
#include "courieruihandler.h"
#include <QGraphicsObject>

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
};

#endif // UIMANAGER_H
