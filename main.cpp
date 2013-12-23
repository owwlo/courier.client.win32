#include "base64.h"
#include <QApplication>
#include "qmlapplicationviewer.h"
#include <QDeclarativeContext>
#include <QDebug>
#include "courierservice.h"
#include <QtDeclarative>
#include <QGraphicsObject>
#include "uimanager.h"
#include <QMetaType>

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

	qmlRegisterType<QGraphicsBlurEffect>("Effects",1,0,"Blur");
	qmlRegisterType<QGraphicsColorizeEffect>("Effects",1,0,"Colorize");
	qmlRegisterType<QGraphicsDropShadowEffect>("Effects",1,0,"DropShadow");
	qmlRegisterType<QGraphicsOpacityEffect>("Effects",1,0,"OpacityEffect");

    QScopedPointer<QApplication> app(createApplication(argc, argv));
	UIManager::initUiManager();
    QScopedPointer<UIManager> uiManager(UIManager::getInstance());
	QScopedPointer<CourierService> courierService(new CourierService(uiManager.data()));
	courierService->init();
	uiManager->showChatWindow();

    return app->exec();
}
