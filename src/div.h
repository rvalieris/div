
#ifndef DIV_H
#define DIV_H

#include <QMenu>
#include <QMainWindow>
#include <QAction>
#include <QContextMenuEvent>
#include <QDir>
#include "imagewidget.h"
#include "thumbnailview.h"

class Div : public QMainWindow {
	Q_OBJECT

public:
	Div();
	void setCurrentFile(QString filename);

private:
	void setBreadCrumbs(QString absPath);
	void resizeEvent(QResizeEvent * event);
	void contextMenuEvent(QContextMenuEvent * event);
	void createActions();

	QLabel * crumbs;

	ThumbnailView * thumbnail_view;

	ImageWidget * imageWidget;

	QMenu * contextMenu;

	QAction *zoomInAct;
	QAction *zoomOutAct;
	QAction *prevImageAct;
	QAction *nextImageAct;
	QAction *normalSizeAct;
	QAction *toggleFullScreenAct;
	QAction *toggleToolbarAct;
};

#endif
