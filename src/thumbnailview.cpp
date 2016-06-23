
#include <QApplication>
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

	currentDir = "";

	tmodel = new ThumbnailModel(this);
	setModel(tmodel);
}

void ThumbnailView::loadDir(QDir dir) {
	tmodel->clear();
	currentDir = dir;
	QDir this_dir(dir);

	for(QFileInfo fi : currentDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot, QDir::Name | QDir::IgnoreCase | QDir::DirsFirst)) {
		if(this_dir != currentDir) break; // dir changed, stop loading

		if(ThumbnailEngine::isLoadable(fi)) {
			tmodel->addThumbnail(fi);
			if(tmodel->rowCount() % 5 == 0) QApplication::processEvents();
		}
	}
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

