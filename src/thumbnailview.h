
#ifndef THUMBNAILVIEW_H
#define THUMBNAILVIEW_H

#include <QListView>
#include <QDir>
#include "thumbnailmodel.h"

class ThumbnailView : public QListView {
	Q_OBJECT

public:
	ThumbnailView(QWidget * parent = nullptr);
	void loadDir(QDir dir);
	QDir currentDir;
	QString nextFile();
	QString prevFile();

private:
	ThumbnailModel * tmodel;
};

#endif
