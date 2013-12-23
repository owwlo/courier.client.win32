#ifndef COURIERQMLVIEWER_H
#define COURIERQMLVIEWER_H

#include "qmlapplicationviewer.h"
#include "courieruihandler.h"

class CourierUiHandler;

class CourierQmlViewer : public QmlApplicationViewer
{
	Q_OBJECT

public:
	CourierQmlViewer(QWidget *parent);
	void setUpUiHandler(CourierUiHandler * uiHandler);
	void setQMLFile(QString path);
	void showWindow();
	void setTopMost(bool);
	~CourierQmlViewer();
	virtual void closeEvent( QCloseEvent * );

public slots:
	Q_INVOKABLE void viewerResize(int length, int breadth)
	{
		setGeometry(this->x(),this->y(),length,breadth);
	}

private:
	CourierUiHandler * mUiHandler;
	QString mQMLPath;
};

#endif // COURIERQMLVIEWER_H
