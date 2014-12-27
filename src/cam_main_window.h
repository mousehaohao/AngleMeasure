#ifndef CAM_MAIN_WINDOW_H
#define CAM_MAIN_WINDOW_H

#include <QtWidgets/QMainWindow>
#include <QLabel>
#include "ui_cam_main_window.h"
#include "opencv.hpp"
#include "cam_cv_image_helper.h"

class CAMMainWindow : public QMainWindow {
	Q_OBJECT

public:
	CAMMainWindow(QWidget *parent = 0);
	~CAMMainWindow();

signals:
	void SIGSelectImageFileComplete(const QString& file_path);

private:
	void OpenImageFile();
	void SaveImageFile();

private:
	Ui::CAMMainWindowClass ui;
	QAction* open_image_action_;
	QAction* save_image_action_;
};

#endif // CAM_MAIN_WINDOW_H
