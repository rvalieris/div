
#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QScrollArea>
#include <QSvgWidget>
#include <QScrollBar>
#include <QLabel>
#include <QTimer>

class ImageWidget : public QScrollArea {
	Q_OBJECT

public:
	ImageWidget(QWidget * parent = nullptr);
	void setImage(QString filename);
	void scaleImage(double factor);
	void normalScale();
	void optimalFactor();

private:
	void mouseMoveEvent(QMouseEvent * event);
	void adjustScrollBar(QScrollBar* scrollBar, double factor);
	void swapWidget(QWidget * widget);
	double scaleFactor;
	QLabel * imageLabel;
	QSvgWidget * svgWidget;
	QTimer * cursorTimer;
	QSize frameSize;
	bool is_svg_visible;
};


#endif
