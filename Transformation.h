#pragma once

#include <QtWidgets>
#include "ui_Transformation.h"
#include "ViewerWidget.h"


class Transformation : public QMainWindow
{
	Q_OBJECT

public:
	Transformation(QWidget *parent = Q_NULLPTR);
	bool clockwise,zoomIn,shearplus;

private:
	Ui::TransformationClass ui;
	QMessageBox msgBox;
	int algo = 0;
	

	//ViewerWidget functions
	ViewerWidget* getCurrentViewerWidget();

	//Event filters
	bool eventFilter(QObject* obj, QEvent* event);

	//ViewerWidget Events
	bool ViewerWidgetEventFilter(QObject* obj, QEvent* event);
	bool ViewerWidgetMouseButtonPress(ViewerWidget* w, QEvent* event);
	bool ViewerWidgetWheel(ViewerWidget* w, QEvent* event);

	//ImageViewer Events
	void closeEvent(QCloseEvent* event);

	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

private slots:
	void on_clear_clicked();
	void on_color_clicked();
	void on_draw_clicked();
	void on_rotateRight_clicked();
	void on_rotateLeft_clicked();
	void on_zoomIn_clicked();
	void on_zoomOut_clicked();
	void on_shearplus_clicked();
	void on_shearminus_clicked();
	void on_flip_clicked();
	void radio2_is_checked();
};
