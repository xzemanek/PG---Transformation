#include "Transformation.h"
#include "ViewerWidget.h"

Transformation::Transformation(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ViewerWidget * vW = new ViewerWidget("polygon",ui.scrollArea->size());
	ui.scrollArea->setWidget(vW);
	ui.scrollArea->setBackgroundRole(QPalette::Dark);
	ui.scrollArea->installEventFilter(this);
	ui.radio1->setChecked(true);
}

//ViewerWidget functions
ViewerWidget* Transformation::getCurrentViewerWidget()
{
	auto vW = static_cast<ViewerWidget*>(ui.scrollArea->widget());
	if (vW) {	
		return vW;
	}
	return nullptr;
}

// Event filters
bool Transformation::eventFilter(QObject* obj, QEvent* event)
{
	if (obj->objectName() == "scrollArea") {
		return ViewerWidgetEventFilter(obj, event);
	}
	return false;
}

//ViewerWidget Events
bool Transformation::ViewerWidgetEventFilter(QObject* obj, QEvent* event)
{
	ViewerWidget* w = static_cast<ViewerWidget*>(static_cast<QScrollArea*>(obj)->widget());

	if (!w)
	{
		return false;
	}
	if (event->type() == QEvent::MouseButtonPress)
	{
		return ViewerWidgetMouseButtonPress(w, event);
	} 
	else if (event->type() == QEvent::Wheel)
	{
		return ViewerWidgetWheel(w, event);
	}

	return false; //QObject::eventFilter(obj, event);
}
bool Transformation::ViewerWidgetMouseButtonPress(ViewerWidget* w, QEvent* event)
{
	QMouseEvent* e = static_cast<QMouseEvent*>(event);
	if (e->button() == Qt::LeftButton)
	{
		if (ui.draw->isEnabled()==false)
		{
			return false;
		}
		w->addPoint(e->pos(),algo);
		w->update();
		return true;
	}
	if (e->button() == Qt::RightButton)
		if(ui.draw->isEnabled() == true)
			on_draw_clicked();
	
	return false;
}

bool Transformation::ViewerWidgetWheel(ViewerWidget* w, QEvent* event)
{
	QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
	return false;
}

//ImageViewer Events
void Transformation::closeEvent(QCloseEvent* event)
{
	if (QMessageBox::Yes == QMessageBox::question(this, "Close Confirmation", "Are you sure you want to exit?", QMessageBox::Yes | QMessageBox::No))
	{
		event->accept();
	}
	else
	{
		event->ignore();
	}
}

void Transformation::mouseMoveEvent(QMouseEvent* event)
{
	ViewerWidget* w = getCurrentViewerWidget();
	if (event->buttons() & Qt::RightButton)
	{
		QPoint point;
		point = mapFromGlobal(event->globalPos());
		qDebug() << point.x() << point.y();
		point.setX(point.x() - 260);
		point.setY(point.y() - 40);
		w->posun(point);
		w->drawPoints(algo);
		w->connectPolygon(algo);
		w->update();
	}
}

void Transformation::mouseReleaseEvent(QMouseEvent* event)
{
	ViewerWidget* w = getCurrentViewerWidget();
	if (event->buttons() & Qt::RightButton)
	{
		QPoint pos;
		pos.setX(event->x() - ui.scrollArea->width());
		pos.setY(event->y() - ui.scrollArea->height());
		w->posun(pos);
		w->drawPoints(algo);
		w->connectPolygon(algo);
		w->update();
	}
}


void Transformation::on_clear_clicked()
{
	ViewerWidget* w = getCurrentViewerWidget();
	w->clear();
	w->update();
	ui.draw->setEnabled(true);
	ui.clear->setEnabled(false);
	ui.groupBox_2->setEnabled(false);
}

void Transformation::on_color_clicked()
{
	ViewerWidget* w = getCurrentViewerWidget();
	QColor color = QColorDialog::getColor(Qt::white, this, "Vyber farbu");
	if (color.isValid())
	{
		w->setPenColor(color);
		if (w->points.isEmpty() == true)
			return;
		w->addPoint(w->points.last(),algo);
		if(ui.draw->isEnabled()==false)
			w->connectPolygon(algo);
		w->update();
	}
}

void Transformation::on_draw_clicked()
{
	ViewerWidget* w = getCurrentViewerWidget();
	if (w->getCount() < 3)
	{
		QMessageBox msgBox;
		msgBox.setText("Treba minimalne 3 body.");
		msgBox.exec();
		return;
	}
		
	w->connectPolygon(algo);
	w->update();
	ui.draw->setEnabled(false);
	ui.clear->setEnabled(true);
	ui.groupBox_2->setEnabled(true);
}

void Transformation::on_rotateRight_clicked()
{
	ViewerWidget* w = getCurrentViewerWidget();
	clockwise = true;
	w->rotate(clockwise,algo);
	w->update();
}

void Transformation::on_rotateLeft_clicked()
{
	ViewerWidget* w = getCurrentViewerWidget();
	clockwise = false;
	w->rotate(clockwise,algo);
	w->update();
}

void Transformation::on_zoomIn_clicked()
{
	ViewerWidget* w = getCurrentViewerWidget();
	zoomIn = true;
	w->scale(zoomIn,algo);
	w->update();
}

void Transformation::on_zoomOut_clicked()
{
	ViewerWidget* w = getCurrentViewerWidget();
	zoomIn = false;
	w->scale(zoomIn,algo);
	w->update();
}

void Transformation::on_shearplus_clicked()
{
	ViewerWidget* w = getCurrentViewerWidget();
	shearplus = true;
	w->shear(shearplus,algo);
	w->update();
}

void Transformation::on_shearminus_clicked()
{
	ViewerWidget* w = getCurrentViewerWidget();
	shearplus = false;
	w->shear(shearplus,algo);
	w->update();
}

void Transformation::on_flip_clicked()
{
	ViewerWidget* w = getCurrentViewerWidget();
	zoomIn = false;
	w->flip(algo);
	w->update();
}

void Transformation::radio2_is_checked()
{
	algo = 1;
}
