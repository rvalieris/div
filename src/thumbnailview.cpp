
#include <QtDebug>
#include "thumbnailview.h"
#include "thumbnailengine.h"

ThumbnailView::ThumbnailView(QWidget * parent)
	: QListView(parent) {
	
	setStyleSheet("background-color:black;");
	setViewMode(QListView::IconMode);
	setResizeMode(QListView::Adjust);
	setMovement(QListView::Static);
	setIconSize(QSize(ThumbnailEngine::THUMBNAIL_SIZE,ThumbnailEngine::THUMBNAIL_SIZE));
	setGridSize(QSize(ThumbnailEngine::THUMBNAIL_SIZE+50,ThumbnailEngine::THUMBNAIL_SIZE+50));
	setUniformItemSizes(true);
	setFrameShape(QFrame::NoFrame);
	setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
	setWordWrap(true);
	setTextElideMode(Qt::ElideNone);

	currentDir = "";

	tmodel = new ThumbnailModel(this);
	setModel(tmodel);
}

void ThumbnailView::loadDir(QDir dir) {
	qDebug() << "ThumbnailView::loadDir: " << dir.absolutePath();
	tmodel->clear();
	load_queue.clear();
	currentDir = dir;
	queue_dir = dir;

	for(QFileInfo fi : currentDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot, QDir::Name | QDir::IgnoreCase | QDir::DirsFirst)) {
		load_queue.enqueue(fi);
	}
	processQueue();
}

void ThumbnailView::processQueue() {
	if(queue_dir != currentDir) return; // dir changed, stop loading
	if(load_queue.isEmpty()) return;

	QFileInfo fi = load_queue.dequeue();
	if(ThumbnailEngine::isLoadable(fi)) {
		qDebug() << "ThumbnailModel::addTN: " << fi.filePath();
		tmodel->addThumbnail(fi);
	}
	// queue up the next item
	QMetaObject::invokeMethod(this, "processQueue", Qt::QueuedConnection);
}

void ThumbnailView::setCurrentFile(QFileInfo fi) {
	currentDir = fi.absoluteDir();
	QModelIndex i = tmodel->findIndex(fi);
	setCurrentIndex(i);
}

QString ThumbnailView::prevFile() {
	QModelIndex i = tmodel->previousThumbnail(currentIndex());
	setCurrentIndex(i);
	return i.data(ThumbnailModel::AbsPathRole).toString();
}

QString ThumbnailView::nextFile() {
	QModelIndex i = tmodel->nextThumbnail(currentIndex());
	setCurrentIndex(i);
	return i.data(ThumbnailModel::AbsPathRole).toString();
}

