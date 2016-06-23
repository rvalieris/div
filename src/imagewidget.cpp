
#include <QApplication>
#include <QMovie>
#include <QMouseEvent>
#include "imagewidget.h"

ImageWidget::ImageWidget(QWidget * parent)
	: QScrollArea(parent) {

	scaleFactor = 1;
	setStyleSheet("background-color:black;color:white");
	setFrameShape(QFrame::NoFrame);
	setAlignment(Qt::AlignCenter);
	setMouseTracking(true);

	imageLabel = new QLabel(this);
	imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	imageLabel->setScaledContents(true);
	imageLabel->setMouseTracking(true);
	QFont f = imageLabel->font();
	f.setPointSize(18);
	imageLabel->setFont(f);

	svgWidget = new QSvgWidget(this);
	svgWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	svgWidget->setMouseTracking(true);

	cursorTimer = new QTimer(this);
	cursorTimer->setInterval(5*1000); // 5 seconds
	connect(cursorTimer, &QTimer::timeout, this, [this]{
		setCursor(Qt::BlankCursor);
	});

	setWidget(imageLabel);
	is_svg_visible = false;
}

// set widget without destroying the previous
void ImageWidget::swapWidget(QWidget * widget) {
	takeWidget();
	setWidget(widget);
}

void ImageWidget::setImage(QString filename) {
	QPixmap pixmap(filename);
	if(pixmap.isNull()) {
		imageLabel->setText(tr("cannot load %1").arg(filename));
		imageLabel->adjustSize();
		frameSize = imageLabel->size();
		swapWidget(imageLabel);
		is_svg_visible = false;
		return;
	}
	if(filename.endsWith(".gif", Qt::CaseInsensitive)) {
		QMovie * movie = new QMovie(filename, QByteArray(), imageLabel);
		imageLabel->setMovie(movie);
		movie->start();
		frameSize = movie->currentPixmap().size();
		swapWidget(imageLabel);
		is_svg_visible = false;
	}
	else if(filename.endsWith(".svg", Qt::CaseInsensitive)) {
		svgWidget->load(filename);
		frameSize = svgWidget->sizeHint();
		swapWidget(svgWidget);
		is_svg_visible = true;
	}
	else {
		imageLabel->setPixmap(pixmap);
		frameSize = pixmap.size();
		swapWidget(imageLabel);
		is_svg_visible = false;
	}
	// need to let the gui update before calculating the factor
	QApplication::processEvents();
	optimalFactor();
}

void ImageWidget::optimalFactor() {
	normalScale();

	double wF = (double)width() / widget()->width();
	double hF = (double)height() / widget()->height();

	// only svg can be upscaled
	if(!is_svg_visible && wF >= 1 && hF >= 1) return;

	double oF = std::min(wF, hF);
	scaleImage(oF);
}

void ImageWidget::scaleImage(double factor) {
	if(frameSize.isValid()) {
		scaleFactor *= factor;
		widget()->resize(scaleFactor * frameSize);
		adjustScrollBar(horizontalScrollBar(), factor);
		adjustScrollBar(verticalScrollBar(), factor);
	}
}

void ImageWidget::normalScale() {
	widget()->adjustSize();
	scaleFactor = 1.0;
}

void ImageWidget::adjustScrollBar(QScrollBar *scrollBar, double factor) {
	scrollBar->setValue(int(factor * scrollBar->value() + ((factor - 1) * scrollBar->pageStep()/2)));
}

void ImageWidget::mouseMoveEvent(QMouseEvent * /*event*/) {
	setCursor(Qt::ArrowCursor);
	cursorTimer->start();
}
