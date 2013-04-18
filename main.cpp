#include <QApplication>
#include "qmlapplicationviewer.h"
#include <QDeclarativeContext>
#include <QDebug>
#include "courierservice.h"
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeEngine>
#include "courieruihandler.h"
#include <QGraphicsObject>
#include "uimanager.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    //Handle arguments
    bool isInstall = false;
    if(argc>1)
    {
        for(int i = 0 ; i<argc ; i++)
        {
            QString arguments = QString::fromAscii(*(argv+i)).toLower();
            if(arguments.indexOf("-install") != -1)
            {
                isInstall = true;
            }
        }
    }

    QScopedPointer<QApplication> app(createApplication(argc, argv));
	UIManager::initUiManager();
    QScopedPointer<UIManager> uiManager(UIManager::getInstance());
	QScopedPointer<CourierService> courierService(new CourierService(uiManager.data()));
	courierService->init();
	uiManager->showChatWindow();

    return app->exec();
}
