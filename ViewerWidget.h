#pragma once

#include <QObject>
#include <QtWidgets>


class ViewerWidget : public QWidget
{
	Q_OBJECT

public:
	ViewerWidget(QString viewerName, QSize imgSize, QWidget* parent = Q_NULLPTR);
	~ViewerWidget();
	void resizeWidget(QSize size);

	//Image functions
	bool setImage(const QImage& inputImg);

	//Draw functions
	void addPoint(QPoint point,int algo);
	void drawPoints(int algo);
	void connectPolygon(int algo);

	//algo
	void DDAalgo(QPoint point1, QPoint point2);
	void Bresenhamalgo(QPoint point1, QPoint point2);

	void posun(QPoint point);



	//Class inline functions
	inline void setPainter() { painter = new QPainter(img); }
	inline int getCount() { return points.length(); }
	void clear() {
		img->fill(Qt::white);
		points.clear();
		transform.reset();
		painter->setTransform(transform);
	}
	void setPenColor(QColor color) { this->color = color; }
	void rotate(bool clockwise, int algo);
	void scale(bool zoomIn, int algo);
	void shear(bool shearplus, int algo);
	void flip(int algo);

	QList<QPoint> points;
	QPoint nullPoint = QPoint(0, 0);
	QMatrix RotM;

private:
	QSize areaSize = QSize(0, 0);
	QImage* img = nullptr;
	QRgb* data = nullptr;
	QPainter* painter = nullptr;
	QColor color;
	bool clockwise;
	QTransform transform;

public slots:
	void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
};
