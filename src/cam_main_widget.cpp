#include "cam_main_widget.h"
#include <QPushButton>

#include <QHBoxLayout>



CAMMainWidget::CAMMainWidget(QWidget *parent)
	: QWidget(parent) {
	

	CreateImageGroupBox();
	CreateSettingGroupBox();

	main_lay_out_ = new QHBoxLayout;
	main_lay_out_->addWidget(gb_image_);
	main_lay_out_->addWidget(gb_setting_);
	
	this->setLayout(main_lay_out_);

}

CAMMainWidget::~CAMMainWidget() {

}

void CAMMainWidget::OnSIGSelectImageFileComplete(const QString& file_path) {
	label_image_->LoadImage(file_path);
}

void CAMMainWidget::CreateImageGroupBox() {
	QHBoxLayout* gb_layout = new QHBoxLayout();
	//
	gb_image_ = new QGroupBox(QStringLiteral("处理图像"));
	//
	label_image_ = new CAMImageLabel;
	label_image_->setFixedSize(800,600);

	gb_layout->addWidget(label_image_);
	QObject::connect(label_image_,&CAMImageLabel::SIGSetPointComplete,
		this,&CAMMainWidget::OnSIGSetPointComplete);
	QObject::connect(label_image_,&CAMImageLabel::SIGCalcAngleComplete,
		this,&CAMMainWidget::OnSIGCalcAngleComplete);
	//
	gb_image_->setLayout(gb_layout);
	
}

void CAMMainWidget::CreateSettingGroupBox() {
	QGroupBox* gb_curr_status_ = new QGroupBox(QStringLiteral("目前设置数据"));
	QVBoxLayout* gb_curr_status_layout = new QVBoxLayout;
	//
	QHBoxLayout*  gb_line_point_1_layout = new QHBoxLayout;
	QLabel* lable_line_point_1 = new QLabel(QStringLiteral("水平点1 坐标: "));
	le_line_point_1_ = new QLineEdit;
	le_line_point_1_->setReadOnly(true);
	le_line_point_1_->setText("X: 0, Y: 0");
	gb_line_point_1_layout->addWidget(lable_line_point_1);
	gb_line_point_1_layout->addWidget(le_line_point_1_);
	//
	QHBoxLayout*  gb_line_point_2_layout = new QHBoxLayout;
	QLabel* lable_line_point_2 = new QLabel(QStringLiteral("水平点2 坐标: "));
	le_line_point_2_ = new QLineEdit;
	le_line_point_2_->setReadOnly(true);
	le_line_point_2_->setText("X: 0, Y: 0");
	gb_line_point_2_layout->addWidget(lable_line_point_2);
	gb_line_point_2_layout->addWidget(le_line_point_2_);
	//
	QHBoxLayout*  gb_circle_point_1_layout = new QHBoxLayout;
	QLabel* lable_circle_point_1 = new QLabel(QStringLiteral("液滴点1 坐标: "));
	le_circle_point_1_ = new QLineEdit;
	le_circle_point_1_->setReadOnly(true);
	le_circle_point_1_->setText("X: 0, Y: 0");
	gb_circle_point_1_layout->addWidget(lable_circle_point_1);
	gb_circle_point_1_layout->addWidget(le_circle_point_1_);
	//
	QHBoxLayout*  gb_circle_point_2_layout = new QHBoxLayout;
	QLabel* lable_circle_point_2 = new QLabel(QStringLiteral("液滴点2 坐标: "));
	le_circle_point_2_ = new QLineEdit;
	le_circle_point_2_->setReadOnly(true);
	le_circle_point_2_->setText("X: 0, Y: 0");
	gb_circle_point_2_layout->addWidget(lable_circle_point_2);
	gb_circle_point_2_layout->addWidget(le_circle_point_2_);
	//
	QHBoxLayout*  gb_circle_point_3_layout = new QHBoxLayout;
	QLabel* lable_circle_point_3 = new QLabel(QStringLiteral("液滴点3 坐标: "));
	le_circle_point_3_ = new QLineEdit;
	le_circle_point_3_->setReadOnly(true);
	le_circle_point_3_->setText("X: 0, Y: 0");
	gb_circle_point_3_layout->addWidget(lable_circle_point_3);
	gb_circle_point_3_layout->addWidget(le_circle_point_3_);
	//
	gb_curr_status_layout->addLayout(gb_line_point_1_layout);
	gb_curr_status_layout->addLayout(gb_line_point_2_layout);
	gb_curr_status_layout->addLayout(gb_circle_point_1_layout);
	gb_curr_status_layout->addLayout(gb_circle_point_2_layout);
	gb_curr_status_layout->addLayout(gb_circle_point_3_layout);
	gb_curr_status_layout->setSizeConstraint(QLayout::SizeConstraint::SetMinAndMaxSize);
	gb_curr_status_->setLayout(gb_curr_status_layout);
	//
	QGroupBox* gb_action_ = new QGroupBox(QStringLiteral("设置水平线与液滴轮廓"));
	QVBoxLayout* gb_action_layout = new QVBoxLayout;
	//
	QHBoxLayout* set_point_layout = new QHBoxLayout;
	combo_box_select_point_ = new QComboBox;
	combo_box_select_point_->addItem(QStringLiteral("水平点"));
	combo_box_select_point_->addItem(QStringLiteral("液滴点"));
	//
	btn_set_point_ = new QPushButton(QStringLiteral("设置"));
	btn_clear_point_ = new QPushButton(QStringLiteral("重置"));
	btn_clear_all_point_ = new QPushButton(QStringLiteral("全部重置"));
	//
	set_point_layout->addWidget(combo_box_select_point_);
	set_point_layout->addWidget(btn_set_point_);
	set_point_layout->addWidget(btn_clear_point_);
	set_point_layout->addWidget(btn_clear_all_point_);
	QObject::connect(btn_set_point_,&QPushButton::clicked,
		this,&CAMMainWidget::OnSIGSetPointButtonClicked);
	QObject::connect(btn_clear_point_,&QPushButton::clicked,
		this,&CAMMainWidget::OnSIGClearPointButtonClicked);
	QObject::connect(btn_clear_all_point_,&QPushButton::clicked,
		this,&CAMMainWidget::OnSIGClearAllPointsButtonClicked);
 	//
	gb_action_layout->setSizeConstraint(QLayout::SizeConstraint::SetMinAndMaxSize);
 	gb_action_layout->addLayout(set_point_layout);
 	gb_action_->setLayout(gb_action_layout);
	//

	QGroupBox* gb_calc = new QGroupBox(QStringLiteral("角度测量 "));
	QHBoxLayout* calc_layout = new QHBoxLayout;
	label_angle_result_ = new QLabel;
	label_angle_result_->setText(QStringLiteral("角度计算结果: "));
	le_angle_result_ = new QLineEdit;
	le_angle_result_->setText(tr("0.0"));
	le_angle_result_->setReadOnly(true);
	calc_layout->addWidget(label_angle_result_);
	calc_layout->addWidget(le_angle_result_);
	calc_layout->setSizeConstraint(QLayout::SizeConstraint::SetMinAndMaxSize);
	gb_calc->setLayout(calc_layout);
	//--------------------------------计算张力 -----------------------------
	QGroupBox* gb_tension_calc = new QGroupBox(QStringLiteral("计算张力"));
	
	QVBoxLayout* gb_tension_calc_layout = new QVBoxLayout;
	QHBoxLayout* tc_item_1_layout = new QHBoxLayout;
	QLabel* label_liquid_type_1 = new QLabel(QStringLiteral("液体种类1: "));
	combo_box_tc_item_1_ = new QComboBox;
	combo_box_tc_item_1_->addItem(QStringLiteral("自定义"));
	combo_box_tc_item_1_->addItem(QStringLiteral("水"));
	QLabel* label_liquid_tension_1 = new QLabel(QStringLiteral("表面张力: "));
	le_tension_item_1_ = new QLineEdit;
	QLabel* label_liquid_angle_1 = new QLabel(QStringLiteral("接触角: "));
	le_tc_angle_1_ = new QLineEdit;

	tc_item_1_layout->addWidget(label_liquid_type_1);
	tc_item_1_layout->addWidget(combo_box_tc_item_1_);
	tc_item_1_layout->addWidget(label_liquid_tension_1);
	tc_item_1_layout->addWidget(le_tension_item_1_);
	tc_item_1_layout->addWidget(label_liquid_angle_1);
	tc_item_1_layout->addWidget(le_tc_angle_1_);
	//--
	QHBoxLayout* tc_item_2_layout = new QHBoxLayout;
	QLabel* label_liquid_type_2 = new QLabel(QStringLiteral("液体种类2: "));
	combo_box_tc_item_2_ = new QComboBox;
	combo_box_tc_item_2_->addItem(QStringLiteral("自定义"));
	combo_box_tc_item_2_->addItem(QStringLiteral("水"));
	QLabel* label_liquid_tension_2 = new QLabel(QStringLiteral("表面张力: "));
	le_tension_item_2_ = new QLineEdit;
	QLabel* label_liquid_angle_2 = new QLabel(QStringLiteral("接触角: "));
	le_tc_angle_2_ = new QLineEdit;

	tc_item_2_layout->addWidget(label_liquid_type_2);
	tc_item_2_layout->addWidget(combo_box_tc_item_2_);
	tc_item_2_layout->addWidget(label_liquid_tension_2);
	tc_item_2_layout->addWidget(le_tension_item_2_);
	tc_item_2_layout->addWidget(label_liquid_angle_2);
	tc_item_2_layout->addWidget(le_tc_angle_2_);

	QHBoxLayout* tc_calc_layout = new QHBoxLayout;
	btn_calc_tension_ = new QPushButton(QStringLiteral("计算张力"));
	QLabel* label_calc_tension_result = new QLabel(QStringLiteral("计算结果: "));
	le_calc_tension_result_ = new QLineEdit;
	le_calc_tension_result_->setReadOnly(true);
	tc_calc_layout->addWidget(btn_calc_tension_);
	tc_calc_layout->addWidget(label_calc_tension_result);
	tc_calc_layout->addWidget(le_calc_tension_result_);

	gb_tension_calc_layout->addLayout(tc_item_1_layout);
	gb_tension_calc_layout->addLayout(tc_item_2_layout);
	gb_tension_calc_layout->addLayout(tc_calc_layout);
	gb_tension_calc_layout->setSizeConstraint(QLayout::SizeConstraint::SetMinAndMaxSize);
	gb_tension_calc->setLayout(gb_tension_calc_layout);
	//



	//
	QGroupBox* gb_debug = new QGroupBox(QStringLiteral("测试信息"));

	gb_setting_ = new QGroupBox;
	QVBoxLayout* gb_setting_layout = new QVBoxLayout;
	gb_setting_layout->addWidget(gb_curr_status_);
	gb_setting_layout->addWidget(gb_action_);
	gb_setting_layout->addWidget(gb_calc);
	gb_setting_layout->addWidget(gb_tension_calc);
	gb_setting_layout->addWidget(gb_debug);
	//gb_setting_layout->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);
	gb_setting_->setLayout(gb_setting_layout);
	//
// 	set_point_infos_[kPTLinePoint1] = le_line_point_1_;
// 	set_point_infos_[kPTLinePoint2] = le_line_point_2_;
// 	set_point_infos_[kPTCirclePoint1] = le_circle_point_1_;
// 	set_point_infos_[kPTCirclePoint2] = le_circle_point_2_;
// 	set_point_infos_[kPTCirclePoint3] = le_circle_point_3_;
}

void CAMMainWidget::OnSIGCalcAngleComplete(double angle) {
	QString display_info = QString("%1").arg(QString::number(angle));
	le_angle_result_->setText(display_info);
}

void CAMMainWidget::OnSIGSetPointComplete(SetPointType set_point_type, const QQueue<cv::Point>& points) {
	int dis_default_max_size = 0;
	if(set_point_type == kPTLinePoint) {
		dis_default_max_size = 2;
	} else if (set_point_type == kPTCirclePoint) {
		dis_default_max_size = 3;
	} else {
		return;
	}
	
	QVector<QString> display_infos;
	for(int index=0; index<points.size(); index++) {
		QString dis_info = QString("X: %1,   Y: %2").arg(QString::number(points[index].x),QString::number(points[index].y));
		display_infos.push_back(dis_info);
	}

	while(display_infos.size() <= dis_default_max_size) {
		display_infos.push_back(QString("X: 0,   Y: 0"));
	}
	if(set_point_type == kPTLinePoint) {
		le_line_point_1_->setText(display_infos[0]);
		le_line_point_2_->setText(display_infos[1]);
	} else if (set_point_type == kPTCirclePoint) {
		le_circle_point_1_->setText(display_infos[0]);
		le_circle_point_2_->setText(display_infos[1]);
		le_circle_point_3_->setText(display_infos[2]);
	}
}

void CAMMainWidget::OnSIGClearPointButtonClicked() {
	int combo_curr_index = combo_box_select_point_->currentIndex();
	switch (combo_curr_index) {
	case 0:
		label_image_->ClearSinglePoint(kPTLinePoint);
		OnSIGSetPointComplete(kPTLinePoint,QQueue<cv::Point>());
		break;
	case 1:
		label_image_->ClearSinglePoint(kPTCirclePoint);
		OnSIGSetPointComplete(kPTCirclePoint,QQueue<cv::Point>());
		break;
	default:
		label_image_->ClearSinglePoint(kPTUnknown);
		break;
	}
}

void CAMMainWidget::OnSIGClearAllPointsButtonClicked() {
	OnSIGSetPointComplete(kPTLinePoint,QQueue<cv::Point>());
	OnSIGSetPointComplete(kPTCirclePoint,QQueue<cv::Point>());
	label_image_->ClearAllPoints();
}

void CAMMainWidget::OnSIGSetPointButtonClicked() {
	int combo_curr_index = combo_box_select_point_->currentIndex();
	switch (combo_curr_index) {
	case 0:
		label_image_->SetCurrSetPointType(kPTLinePoint);
		break;
	case 1:
		label_image_->SetCurrSetPointType(kPTCirclePoint);
		break;
	default:
		label_image_->SetCurrSetPointType(kPTUnknown);
		break;
	}
}