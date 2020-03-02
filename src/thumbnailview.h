
#ifndef THUMBNAILVIEW_H
#define THUMBNAILVIEW_H

#include <QListView>
#include <QDir>
#include <QQueue>
#include "thumbnailmodel.h"

class ThumbnailView : public QListView {
	Q_OBJECT

public:
	ThumbnailView(QWidget * parent = nullptr);
	void loadDir(QDir dir, bool wait = false);
	QDir currentDir;
	QString nextFile();
	QString prevFile();
	void setCurrentFile(QFileInfo fi);

private slots:
	void processQueue();

private:
	ThumbnailModel * tmodel;
	QQueue<QFileInfo> load_queue;
	QDir queue_dir;
};

#endif
