
#include <QPainter>
#include <QDir>
#include <QSvgRenderer>
#include <QImageReader>
#include "thumbnailengine.h"

const QString ThumbnailEngine::DIRECTORY_KEY = ":directory:";

ThumbnailEngine::ThumbnailEngine(QFileInfo fi) {
	filename = fi.absoluteFilePath();
	is_dir = fi.isDir();
	QFileInfo first_image = getFirstImageFromDir(filename);
	if(is_dir && !first_image.exists()) key = DIRECTORY_KEY;
	else key = filename;
}

bool ThumbnailEngine::isLoadable(QFileInfo fi) {
	QImageReader r(fi.absoluteFilePath());
	return fi.isDir() || r.canRead();
}

QFileInfo ThumbnailEngine::getFirstImageFromDir(QString dirname) {
	QDir d(dirname);
	int limit = 10;
	for(QFileInfo fi : d.entryInfoList(QDir::Files, QDir::Name | QDir::IgnoreCase)) {
		if(!fi.isReadable() || fi.size() == 0) continue; // empty or device file
		QString f = fi.absoluteFilePath();
		QImageReader r(f);
		if(r.canRead()) return fi;
		if(--limit < 0) break;
	}
	return QFileInfo();
}

void ThumbnailEngine::paintThumbnail(QPainter & painter, QString filename) {
	if(filename.endsWith(".svg", Qt::CaseInsensitive)) {
		QSvgRenderer r(filename);
		r.render(&painter, QRectF(0, 0, THUMBNAIL_SIZE, THUMBNAIL_SIZE));
	}
	else {
		QPixmap qi(filename);
		if(!qi.isNull()) painter.drawPixmap(QPoint(0, 0), qi.scaled(QSize(THUMBNAIL_SIZE, THUMBNAIL_SIZE), Qt::KeepAspectRatio));
	}
}

QPixmap * ThumbnailEngine::renderThumbnail(QString filename, bool is_dir) {
	QImage d_icon(THUMBNAIL_SIZE, THUMBNAIL_SIZE, QImage::Format_ARGB32);
	d_icon.fill(Qt::transparent);
	QPainter painter(&d_icon);

	if(is_dir) {
		QFileInfo first_image = getFirstImageFromDir(filename);
		if(first_image.exists()) {
			paintThumbnail(painter, first_image.absoluteFilePath());
			FOLDER_RENDERER()->render(&painter, QRectF(THUMBNAIL_SIZE-STHUMB_SIZE,THUMBNAIL_SIZE-STHUMB_SIZE,STHUMB_SIZE,STHUMB_SIZE));
		}
		else {
			FOLDER_RENDERER()->render(&painter, QRectF(0,0,THUMBNAIL_SIZE,THUMBNAIL_SIZE));
		}
	}
	else {
		paintThumbnail(painter, filename);
	}
	return new QPixmap(QPixmap::fromImage(d_icon));
}

void ThumbnailEngine::setCacheLimit(int n) {
	PIXMAP_CACHE()->setMaxCost(1024 * n);
}

int ThumbnailEngine::pixmapCost(QPixmap * pixmap) {
	return pixmap->width() * pixmap->height() * pixmap->depth() / 8;
}

QIconEngine * ThumbnailEngine::clone() const {
	return new ThumbnailEngine(filename);
}

QPixmap * ThumbnailEngine::findOrLoad() {
	QPixmap * pm = PIXMAP_CACHE()->object(key);
	if(pm == nullptr) {
		pm = renderThumbnail(filename, is_dir);
		PIXMAP_CACHE()->insert(key, pm, pixmapCost(pm));
	}
	return pm;
}

void ThumbnailEngine::paint(QPainter *painter, const QRect &rect, QIcon::Mode /*mode*/, QIcon::State /*state*/) {
	QPixmap * pm = findOrLoad();
	if(pm) painter->drawPixmap(rect, *pm);
}

QCache<QString, QPixmap> * ThumbnailEngine::PIXMAP_CACHE() {
	static QCache<QString, QPixmap> * cache = new QCache<QString, QPixmap>(10*1024*1024); // 10MB
	return cache;
}

QSvgRenderer * ThumbnailEngine::FOLDER_RENDERER() {
	static QSvgRenderer * r = new QSvgRenderer(QString(":/folder.svg"));
	return r;
}

