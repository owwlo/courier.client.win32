#include "courierqmlviewer.h"
#include <QDeclarativeContext>
#include <QDebug>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeEngine>
#include "courieruihandler.h"
#include <QGraphicsObject>
#include <QDesktopWidget>
#include <QApplication>

CourierQmlViewer::CourierQmlViewer(QWidget *parent)
	: QmlApplicationViewer(parent)
{
	mUiHandler = new CourierUiHandler;
}

CourierQmlViewer::~CourierQmlViewer()
{

}

void CourierQmlViewer::setUpUiHandler( CourierUiHandler * uiHandler )
{
	mUiHandler = uiHandler;
}

void CourierQmlViewer::showWindow()
{
	setOrientation(ScreenOrientationAuto);
	setAttribute(Qt::WA_TranslucentBackground);
	engine()->rootContext()->setContextObject(mUiHandler);
	setStyleSheet("background:transparent;");
	QDeclarativeContext *window = rootContext();

	// Connect signals and slots for QML
	mUiHandler->setUpSignalSlot(this);

	// 让QML控制窗口移动
	window->setContextProperty("mainWindow", this);

	showExpanded();
}

void CourierQmlViewer::setQMLFile( QString path )
{
	mQMLPath = path;
	setMainQmlFile(mQMLPath);
}

void CourierQmlViewer::closeEvent( QCloseEvent * )
{
	qDebug()<<"QML: Window closing.";
}

void CourierQmlViewer::setTopMost( bool topMost)
{
	if(topMost)
	{
		// 靠setWindowFlags会清空原有flags
		// 如果flag多的话用windowFlags()逻辑运算后setWindowFlags会比较好
		setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	}
	else
	{
		setWindowFlags(Qt::FramelessWindowHint);
	}
}
