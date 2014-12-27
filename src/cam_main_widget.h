#ifndef CAM_MAIN_WIDGET_H
#define CAM_MAIN_WIDGET_H

#include <QtWidgets/qwidget.h>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QLineEdit>
#include <QMap>

#include "cam_image_label.h"

class CAMImageLabel;
class CAMMainWidget : public QWidget
{
public:
	CAMMainWidget(QWidget *parent);
	~CAMMainWidget();

public:
	void OnSIGSelectImageFileComplete(const QString& file_path);
	void OnSIGSetPointComplete(SetPointType set_point_type, const QQueue<cv::Point>& points);
	void OnSIGCalcAngleComplete(double angle);

private:
	void OnSIGSetPointButtonClicked();
	void OnSIGClearPointButtonClicked();
	void OnSIGClearAllPointsButtonClicked();
	
private:
	void CreateImageGroupBox();
	void CreateSettingGroupBox();

private:
	QMap<SetPointType,QLineEdit*> set_point_infos_;

private:
	QHBoxLayout* main_lay_out_;
	//
	QGroupBox* gb_image_;
	CAMImageLabel* label_image_;
	//
	QGroupBox* gb_setting_;
	//
	//
	QLineEdit* le_line_point_1_;
	QLineEdit* le_line_point_2_;
	QLineEdit* le_circle_point_1_;
	QLineEdit* le_circle_point_2_;
	QLineEdit* le_circle_point_3_;
	//
	QComboBox* combo_box_select_point_;
	QPushButton* btn_set_point_;
	QPushButton* btn_clear_point_;
	QPushButton* btn_clear_all_point_;
	//
	QLabel* label_angle_result_;
	QLineEdit* le_angle_result_;
	//
	QComboBox* combo_box_tc_item_1_;
	QLineEdit* le_tension_item_1_;
	QLineEdit* le_tc_angle_1_;
	//
	QComboBox* combo_box_tc_item_2_;
	QLineEdit* le_tension_item_2_;
	QLineEdit* le_tc_angle_2_;
	//
	QPushButton* btn_calc_tension_;
	QLineEdit* le_calc_tension_result_;
};

#endif // CAM_MAIN_WIDGET_H
