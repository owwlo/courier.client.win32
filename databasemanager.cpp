#include "databasemanager.h" 
#include <QCryptographicHash>
#include <QSqlDatabase>
#include <QDebug>
#include <QList>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

const QString DatabaseManager::SQLITE_FILE_PATH = "sessions.db";
DatabaseManager * DatabaseManager::sInstance = nullptr;

DatabaseManager::DatabaseManager(QObject *parent)
	: IDatabase(parent)
{
	initDBConnection();
}

QList<QMap<QString, QString>*>* DatabaseManager::query( QString table, QString rule, QString limitation )
{
	QList<QMap<QString,QString>*>* list = new QList<QMap<QString,QString>*>;
	QSqlQuery query;
	query.exec("select rowid,* from " + table + " where " + rule + " " + limitation + ";");
	while(query.next())
	{
		QMap<QString,QString>* map = new QMap<QString,QString>;
		for (int i = 0;i<query.record().count();i++)
		{
			map->insert(query.record().fieldName(i),query.value(i).toString());
		}
		list->push_back(map);
	}
	return list;
}

void DatabaseManager::update( QString table, QString condition, QMap<QString, QString> updateValues )
{
	throw std::exception("The method or operation is not implemented.");
}

int DatabaseManager::remove( QString table, QString condition )
{
	throw std::exception("The method or operation is not implemented.");
}

QString DatabaseManager::getEncodePassword()
{
	/************************************************************************/
	/*没有找到很好的获取特征码的方法                                        */
	/************************************************************************/
	QCryptographicHash md(QCryptographicHash::Md5);
	QByteArray bytes;
	QString md5;

	bytes.append(QString("I am unique code!!!"));
	md.addData(bytes);
	md5.append(md.result().toHex());

	return md5;
}

void DatabaseManager::initDBConnection()
{
	QSqlDatabase dbconn = QSqlDatabase::addDatabase("QSQLITE"); 
	dbconn.setDatabaseName(SQLITE_FILE_PATH);
	if (!dbconn.open()) 
	{
		qDebug()<<"Database Error"<<dbconn.lastError().text();
	}
}

DatabaseManager * DatabaseManager::getInstance()
{
	if(sInstance == nullptr)
	{
		sInstance = new DatabaseManager(0);
	}
	return sInstance;
}

int DatabaseManager::insert( QString table, QMap<QString, QString> values )
{
	QSqlQuery query;
	QString column;
	QString value;
	for (QMap<QString,QString>::Iterator it = values.begin();it!=values.end();it++)
	{
		column += it.key() + ",";
		value += "'" + it.value() + "',";
	}
	column = column.left(column.length()-1);
	value = value.left(value.length()-1);
	QString sql = "insert into " + table +" (" + column + ") values (" + value + ");";
	query.exec(sql);

	return 0;
}
