#include <QObject>
#include "userimageprovider.h"
#include <QSize>
#include <QImage>
#include <QDeclarativeImageProvider>

UserImageProvider * UserImageProvider::sInstance = nullptr;

UserImageProvider::UserImageProvider()
	: QDeclarativeImageProvider(QDeclarativeImageProvider::Image)
{

}

UserImageProvider::~UserImageProvider()
{

}

QImage UserImageProvider::requestImage( const QString &id, QSize *size, const QSize &requestedSize )
{
	return mImageMap.value(id);
}

UserImageProvider * UserImageProvider::getInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new UserImageProvider();
	}
	return sInstance;
}

void UserImageProvider::putImage( QString imageId, QImage image )
{
	mImageMap.insert(imageId, image);
}
