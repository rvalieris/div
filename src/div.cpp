
#include <QApplication>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QtDebug>
#include "div.h"
#include "thumbnailengine.h"
#include "thumbnailmodel.h"

Div::Div() {
	ThumbnailEngine::setCacheLimit(1024*1024); // 1G

	crumbs = new QLabel(this);
	connect(crumbs, &QLabel::linkActivated, this, [this](const QString &link){ setCurrentFile(link); });

	imageWidget = new ImageWidget(this);

	thumbnail_view = new ThumbnailView(this);

	QWidget * main = new QWidget(this);
	main->setContentsMargins(0, 0, 0, 0);
	QVBoxLayout * v_layout = new QVBoxLayout(main);
	v_layout->setSpacing(0);
	v_layout->setContentsMargins(0, 0, 0, 0);
	v_layout->addWidget(crumbs);
	v_layout->addWidget(imageWidget);
	v_layout->addWidget(thumbnail_view);

	main->setLayout(v_layout);
	setCentralWidget(main);

	createActions();

	setWindowTitle(tr("div"));
	resize(500, 400);
}

void Div::setBreadCrumbs(QString absPath) {
	crumbs->setStyleSheet("background-color:black;color:white");
	QStringList crumbs_list(QStringList(absPath.split("/")));
	QStringList cf("");
	QStringList out(
	"<style>"
	".slash {font-size:18pt;font-weight:900;color:white;}"
	".bc_button { font-size:10pt;text-decoration:none;color:white}"
	"</style>"
	);
	QString slash(" <span class='slash'>&#47;</span> ");
	out.append("<a href='/' class='bc_button'>root</a>");
	
	for(QString s : crumbs_list) {
		if(s.isEmpty()) continue;
		cf.append(s);
		out.append("<a href='"+cf.join("/")+"' class='bc_button'>"+s+"</a>");
	}
	crumbs->setText(out.join(slash));
}

void Div::setCurrentFile(QString filename) {
	//qDebug() << "div::setCurrentFile: " << filename;
	QFileInfo fi(filename);

	setBreadCrumbs(fi.absoluteFilePath());

	imageWidget->setVisible(!fi.isDir());
	thumbnail_view->setVisible(fi.isDir());
	prevImageAct->setEnabled(!fi.isDir());
	nextImageAct->setEnabled(!fi.isDir());
	zoomInAct->setEnabled(!fi.isDir());
	zoomOutAct->setEnabled(!fi.isDir());
	normalSizeAct->setEnabled(!fi.isDir());

	if(fi.isDir()) {
		QDir d(fi.absoluteFilePath());
		if(thumbnail_view->currentDir != d) {
			thumbnail_view->loadDir(d);
		}
	}
	else {
		thumbnail_view->setCurrentFile(fi);
		imageWidget->setImage(fi.absoluteFilePath());
	}
}

void Div::createActions() {

	QAction * exitAct = new QAction(tr("Exit"), this);
	exitAct->setShortcut(Qt::Key_Q);
	connect(exitAct, &QAction::triggered, this, &Div::close);

	zoomInAct = new QAction(tr("Zoom In (25%)"), this);
	zoomInAct->setShortcut(Qt::Key_Plus);
	connect(zoomInAct, &QAction::triggered, this, [this] {
		imageWidget->scaleImage(1.25);
	});

	zoomOutAct = new QAction(tr("Zoom Out (25%)"), this);
	zoomOutAct->setShortcut(Qt::Key_Minus);
	connect(zoomOutAct, &QAction::triggered, this, [this] {
		imageWidget->scaleImage(0.8);
	});

	prevImageAct = new QAction(tr("Previous Image"), this);
	prevImageAct->setShortcut(Qt::Key_Left);
	connect(prevImageAct, &QAction::triggered, this, [this] {
		setCurrentFile(thumbnail_view->prevFile());
	});
	
	nextImageAct = new QAction(tr("Next Image"), this);
	nextImageAct->setShortcut(Qt::Key_Right);
	connect(nextImageAct, &QAction::triggered, this, [this] {
		setCurrentFile(thumbnail_view->nextFile());
	});

	QAction * upDirectoryAct = new QAction(tr("Up directory"), this);
	upDirectoryAct->setShortcut(Qt::Key_Backspace);
	connect(upDirectoryAct, &QAction::triggered, this, [this] {
		QDir up = thumbnail_view->currentDir;
		if(!imageWidget->isVisible()) {
			up.cdUp();
		}
		setCurrentFile(up.absolutePath());
	});

	normalSizeAct = new QAction(tr("Normal Size (100%)"), this);
	normalSizeAct->setShortcut(Qt::Key_0);
	connect(normalSizeAct, &QAction::triggered, this, [this] {
		imageWidget->normalScale();
	});

	toggleFullScreenAct = new QAction(tr("Full Screen"), this);
	toggleFullScreenAct->setShortcut(Qt::Key_F);
	toggleFullScreenAct->setCheckable(true);
	connect(toggleFullScreenAct, &QAction::triggered, this, [this](bool checked) {
		if(checked) {
			setWindowState(Qt::WindowFullScreen);
		}
		else {
			setWindowState(Qt::WindowNoState);
		}
	});

	toggleToolbarAct = new QAction(tr("Breadcrumbs"), this);
	toggleToolbarAct->setShortcut(Qt::Key_B);
	toggleToolbarAct->setCheckable(true);
	toggleToolbarAct->setChecked(true);
	connect(toggleToolbarAct, &QAction::triggered, this, [this](bool checked) {
		crumbs->setVisible(checked);
		imageWidget->optimalFactor();
	});

	QAction * aboutAct = new QAction(tr("About"), this);
	connect(aboutAct, &QAction::triggered, this, [this]{
		QMessageBox::about(this, tr("About div"), tr("div - a dry image viewer\nversion %1").arg(qApp->applicationVersion()));
	});

	connect(thumbnail_view, &QListView::activated, this, [this](const QModelIndex & idx) {
		QString f = idx.data(ThumbnailModel::AbsPathRole).toString();
		setCurrentFile(f);
	});

	setStyleSheet(
	"QScrollBar:vertical { border-width: 0; width: 0.5em; margin: 0; }"
	"QScrollBar:horizontal { border-width: 0; height: 0.5em; margin:0; }"
	"QScrollBar::handle:vertical, QScrollBar::handle:horizontal {"
	"	border-width: 0;"
	"	background: white;"
	"	min-height: 0;"
	"}"
	"QScrollBar::add-line:vertical, QScrollBar::add-line:horizontal { height: 0; }"
	"QScrollBar::sub-line:vertical, QScrollBar::sub-line:horizontal { height: 0; }"
	);

	addAction(exitAct);
	addAction(zoomInAct);
	addAction(zoomOutAct);
	addAction(prevImageAct);
	addAction(nextImageAct);
	addAction(upDirectoryAct);
	addAction(normalSizeAct);
	addAction(toggleFullScreenAct);
	addAction(toggleToolbarAct);

	contextMenu = new QMenu(this);
	contextMenu->addAction(zoomInAct);
	contextMenu->addAction(zoomOutAct);
	contextMenu->addAction(normalSizeAct);
	contextMenu->addAction(prevImageAct);
	contextMenu->addAction(nextImageAct);
	contextMenu->addAction(upDirectoryAct);
	contextMenu->addAction(toggleFullScreenAct);
	contextMenu->addAction(toggleToolbarAct);
	contextMenu->addSeparator();
	contextMenu->addAction(aboutAct);
	contextMenu->addAction(exitAct);
}

void Div::resizeEvent(QResizeEvent * event) {
	QMainWindow::resizeEvent(event);
	imageWidget->optimalFactor();
	if(windowState() == Qt::WindowFullScreen) {
		toggleFullScreenAct->setChecked(true);
	}
	else {
		toggleFullScreenAct->setChecked(false);
	}
}

void Div::contextMenuEvent(QContextMenuEvent * event) {
	contextMenu->exec(event->globalPos());
}

