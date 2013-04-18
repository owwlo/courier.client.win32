#ifndef IDATABASE_H
#define IDATABASE_H

#include <QObject>
#include <QMap>

class IDatabase : public QObject
{
	Q_OBJECT

public:
	IDatabase(QObject *parent):QObject(parent){};
	virtual QList<QMap<QString, QString>*>* query(QString table, QString rule, QString limitation) = 0;
	virtual void update(QString table, QString condition, QMap<QString, QString> updateValues) = 0;
	virtual int remove(QString table, QString condition) = 0;
	virtual int insert(QString table, QMap<QString, QString> values) = 0;
private:
};

#endif // IDATABASE_H
