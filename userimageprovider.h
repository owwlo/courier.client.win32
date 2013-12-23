#ifndef USERIMAGEPROVIDER_H
#define USERIMAGEPROVIDER_H

#include <QObject>
#include <QDeclarativeImageProvider>
#include <QImage>
#include <QMap>

class UserImageProvider : public QDeclarativeImageProvider
{
public:
	~UserImageProvider();
	QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
	static UserImageProvider * getInstance();
	void putImage(QString imageId, QImage image);

private:
	static UserImageProvider * sInstance;
	UserImageProvider();
	QMap<QString, QImage> mImageMap;
};

#endif // USERIMAGEPROVIDER_H
