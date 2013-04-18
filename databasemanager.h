#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "idatabase.h"

class DatabaseManager : public IDatabase
{
	Q_OBJECT

public:
	static DatabaseManager * getInstance();

	virtual QList<QMap<QString, QString>*>* query( QString table, QString rule, QString limitation );
	virtual void update( QString table, QString condition, QMap<QString, QString> updateValues );
	virtual int remove( QString table, QString condition );
	virtual int insert( QString table, QMap<QString, QString> values );

private:
	DatabaseManager(QObject *parent);
	static const QString SQLITE_FILE_PATH;
	static DatabaseManager * sInstance;
	QString getEncodePassword();
	void initDBConnection();
};

#endif // DATABASEMANAGER_H
