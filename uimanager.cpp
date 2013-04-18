#include "uimanager.h"
#include "chatwindowuihandler.h"
#include "courierqmlviewer.h"
#include "authinputwindowuihandler.h"
#include <QDesktopWidget>
#include <QApplication>

UIManager * UIManager::sInstance = nullptr;

UIManager::UIManager(QWidget *parent)
	: QWidget(parent)
{
	mAuthWindowViewer = nullptr;
}

UIManager::~UIManager()
{

}

UIManager * UIManager::getInstance()
{
	return sInstance;
}

void UIManager::initUiManager()
{
	sInstance = new UIManager;
}

void UIManager::OnAuthenticateCodeRequired()
{
	if (mAuthWindowViewer == nullptr || !mAuthWindowViewer->isVisible())
	{
		showAuthenticateCodeInputWindow();
	}
}

void UIManager::showChatWindow()
{
	ChatWindowUiHandler * handler = new ChatWindowUiHandler(0);
	CourierQmlViewer * viewer = new CourierQmlViewer(0);
	viewer->setUpUiHandler(handler);
	viewer->setTopMost(true);
	viewer->setQMLFile("qml/Courier/main.qml");

	QDesktopWidget *desktop = QApplication::desktop();
	QRect deskRect = desktop->screenGeometry(desktop->primaryScreen());
	int width = deskRect.width();
	viewer->move(width - viewer->width(), 0);

	viewer->showWindow();
}

void UIManager::showAuthenticateCodeInputWindow()
{
	AuthInputWindowUiHandler * handler = new AuthInputWindowUiHandler(0);
	mAuthWindowViewer = new CourierQmlViewer(0);
	mAuthWindowViewer->setUpUiHandler(handler);
	mAuthWindowViewer->setTopMost(true);
	mAuthWindowViewer->setQMLFile("qml/Courier/AuthenticateCodeInputWindow.qml");
	mAuthWindowViewer->showWindow();
}
