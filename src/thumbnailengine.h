
#ifndef THUMBNAILENGINE_H
#define THUMBNAILENGINE_H

#include <QFileInfo>
#include <QIconEngine>
#include <QCache>
#include <QSvgRenderer>

class ThumbnailEngine : public QIconEngine {

public:
	ThumbnailEngine(QFileInfo f);
	QIconEngine * clone() const override;
	void paint(QPainter * painter, const QRect & rect, QIcon::Mode mode, QIcon::State state) override;

	static bool isLoadable(QFileInfo fi);
	static void setCacheLimit(int n);
	static const int THUMBNAIL_SIZE = 200;

private:
	QPixmap * findOrLoad();
	QString filename;
	QString key;
	bool is_dir;

	static void paintThumbnail(QPainter & painter, QString filename);
	static QPixmap * renderThumbnail(QString filename, bool is_dir);
	static QFileInfo getFirstImageFromDir(QString dirname);
	static int pixmapCost(QPixmap * pixmap);
	static QCache<QString, QPixmap> * PIXMAP_CACHE();
	static QSvgRenderer * FOLDER_RENDERER();
	static const QString DIRECTORY_KEY;
	static const int STHUMB_SIZE = 50;
};

#endif
