#include "ViewerWidget.h"
#include<QtWidgets>


ViewerWidget::ViewerWidget(QString viewerName, QSize imgSize, QWidget* parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_StaticContents);
	setMouseTracking(true);
	if (imgSize != QSize(0, 0)) {
		img = new QImage(imgSize, QImage::Format_ARGB32);
		img->fill(Qt::white);
		resizeWidget(img->size());
		setPainter();
	}
}
ViewerWidget::~ViewerWidget()
{
	delete painter;
	delete data;
	delete img;
}
void ViewerWidget::resizeWidget(QSize size)
{
	this->resize(size);
	this->setMinimumSize(size);
	this->setMaximumSize(size);
}

//Image functions
bool ViewerWidget::setImage(const QImage& inputImg)
{
	if (img != nullptr)
	{
		delete img;
	}
	img = new QImage(inputImg);
	if (!img)
	{
		return false;
	}
	resizeWidget(img->size());
	setPainter();
	return true;
}

//Draw functions
void ViewerWidget::addPoint(QPoint point, int algo)
{
	points.append(point);
	drawPoints(algo);
}

void ViewerWidget::drawPoints(int algo)
{
	QPen pen;
	pen.setColor(color);
	pen.setCapStyle(Qt::RoundCap);
	painter->setRenderHint(QPainter::Antialiasing);
	QPoint point1, point2;
	point1 = points[0];
	for each(QPoint point in points)
	{
		pen.setWidth(8);
		painter->setPen(pen);
		painter->drawPoint(point);
		point2 = point;

		if (algo == 0)
		{
			DDAalgo(point1, point2);
		}
		else
		{
			Bresenhamalgo(point1, point2);
		}
		point1 = point2;
	}
}

void ViewerWidget::connectPolygon(int algo)
{
	if (algo == 0)
	{
		DDAalgo(points[0], points[points.size()-1]);
	}
	else
	{
		Bresenhamalgo(points[0], points[points.size()-1]);
	}
}

void ViewerWidget::DDAalgo(QPoint point1, QPoint point2)
{ 
	double x, y;
	int dx, dy;
	double xInc, yInc;
	int steps;

	dx = point2.x() - point1.x();
	dy = point2.y() - point1.y();
	x = point1.x();
	y = point1.y();

	steps = abs(dx) >= abs(dy) ? abs(dx) : abs(dy);

	xInc = static_cast<double>(dx) / steps;
	yInc = static_cast<double>(dy) / steps;

	int xMax = img->size().width();
	int yMax = img->size().height();

	for (int i = 0; i <= steps; i++)
	{
		img->setPixel(x, y, color.rgb());
		x += xInc;
		y += yInc;
	}
}
void ViewerWidget::Bresenhamalgo(QPoint point1, QPoint point2)
{
	int x, y, xi, yi, dx, dy, d, dDecrement, dIncrement, xMax, yMax;
	x = point1.x();
	y = point1.y();

	dx = abs(point2.x() - point1.x());
	dy = abs(point2.y() - point1.y());
	xi = point2.x() - point1.x() >= 0 ? 1 : -1;
	yi = point2.y() - point1.y() >= 0 ? 1 : -1;

	bool xAxis = dx > dy;
	d = xAxis ? 2 * dy - dx : 2 * dx - dy;
	dDecrement = xAxis ? 2 * dx : 2 * dy;
	dIncrement = xAxis ? 2 * dy : 2 * dx;

	xMax = img->size().width();
	yMax = img->size().height();

	while (x != point2.x() || y != point2.y())
	{
		if (x > 0 && x < xMax && y > 0 && y < yMax)
			img->setPixel(x, y, color.rgba());

		if (d > 0)
		{
			d -= dDecrement;
			x += xi;
			y += yi;
		}
		else
		{
			if (xAxis)
			{
				x += xi;
			}
			else
			{
				y += yi;
			}
		}
		d += dIncrement;
	}
}
void ViewerWidget::posun(QPoint point)
{
	img->fill(Qt::white);
	QPoint center = points[0];
	point.setX(point.x() - center.x());
	point.setY(point.y() - center.y());
	for (int i = 0; i < points.size(); i++)
	{
		points[i].setX(points[i].x() + point.x());
		points[i].setY(points[i].y() + point.y());
	}
}
//Slots
void ViewerWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QRect area = event->rect();
	painter.drawImage(area, *img, area);
}

void ViewerWidget::rotate(bool clockwise, int algo)
{
	int a = 45;
	QPoint point0 = QPoint(points[0].x(), points[0].y());
	QMatrix TranM(1, 0, 0, 1, -points[0].x(), -points[0].y());
	for (int i = 0 ; i < points.length() ; i++)
	{
		points[i].setX(points[i].x() + TranM.dx());
		points[i].setY(points[i].y() + TranM.dy());
	}
	if (clockwise == true)
	{
		RotM.setMatrix(cos(a), sin(a), -sin(a), cos(a), 0, 0);
	}
	else
	{
		RotM.setMatrix(cos(a), -sin(a), sin(a), cos(a), 0, 0);
	}
	for (int i = 0; i < points.length(); i++)
	{
		double p = points[i].x();
		points[i].setX(points[i].x()*RotM.m11() + points[i].y()*RotM.m12());
		points[i].setY(p * RotM.m21() + points[i].y() * RotM.m22());
	}
	QMatrix TranM2(1, 0, 0, 1, point0.x(), point0.y());
	for (int i = 0; i < points.length(); i++)
	{
		points[i].setX(points[i].x() + TranM2.dx());
		points[i].setY(points[i].y() + TranM2.dy());
	}
	
	img->fill(Qt::white);
	drawPoints(algo);
	connectPolygon(algo);
}

void ViewerWidget::scale(bool zoomIn, int algo)
{
	QMatrix SkalM;
	QPoint point0 = QPoint(points[0].x(), points[0].y());
	QMatrix TranM(1, 0, 0, 1, -points[0].x(), -points[0].y());
	for (int i = 0; i < points.length(); i++)
	{
		points[i].setX(points[i].x() + TranM.dx());
		points[i].setY(points[i].y() + TranM.dy());
	}
	if (zoomIn == true)
	{
		SkalM.setMatrix(1.5, 0, 0, 1.5, 0, 0);
	}
	else
	{
		SkalM.setMatrix(0.75, 0, 0, 0.75, 0, 0);
	}
	for (int i = 0; i < points.length(); i++)
	{
		double p = points[i].x();
		points[i].setX(points[i].x() * SkalM.m11());
		points[i].setY(points[i].y() * SkalM.m22());
	}
	QMatrix TranM2(1, 0, 0, 1, point0.x(), point0.y());
	for (int i = 0; i < points.length(); i++)
	{
		points[i].setX(points[i].x() + TranM2.dx());
		points[i].setY(points[i].y() + TranM2.dy());
	}

	img->fill(Qt::white);
	drawPoints(algo);
	connectPolygon(algo);
}

void ViewerWidget::shear(bool shearplus, int algo) {
	QMatrix SkosM;
	QPoint point0 = QPoint(points[0].x(), points[0].y());
	QMatrix TranM(1, 0, 0, 1, -points[0].x(), -points[0].y());
	for (int i = 0; i < points.length(); i++)
	{
		points[i].setX(points[i].x() + TranM.dx());
		points[i].setY(points[i].y() + TranM.dy());
	}
	if (shearplus == true)
	{
		SkosM.setMatrix(1, 0, 0.1, 1, 0, 0);
	}
	else
	{
		SkosM.setMatrix(1, 0, -0.1, 1, 0, 0);
	}
	for (int i = 0; i < points.length(); i++)
	{
		double p = points[i].x();
		points[i].setX(SkosM.m12()*points[i].y() + points[i].x());
		points[i].setY(SkosM.m21()*points[i].x() + points[i].y());
	}
	QMatrix TranM2(1, 0, 0, 1, point0.x(), point0.y());
	for (int i = 0; i < points.length(); i++)
	{
		points[i].setX(points[i].x() + TranM2.dx());
		points[i].setY(points[i].y() + TranM2.dy());
	}

	img->fill(Qt::white);
	drawPoints(algo);
	connectPolygon(algo);
}

void ViewerWidget::flip(int algo)
{
	qreal tan = (points[1].y() - points[0].y()) / (points[1].x()-points[0].x());
	transform.translate(points[0].x(), points[0].y());
	transform.rotateRadians(qAtan(tan));
	transform.scale(-1,1);
	transform.rotateRadians(qAtan(tan));
	transform.translate(-points[0].x(), -points[0].y());
	painter->setTransform(transform);
	img->fill(Qt::white);
	drawPoints(algo);
	connectPolygon(algo);
}