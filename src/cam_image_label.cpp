#include "cam_image_label.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <QImage>


const int kLinePointNums = 2;
const int kCirclePointNums = 3;

CAMImageLabel::CAMImageLabel() {
	cv_image_helper_ = NULL;
	curr_set_point_type_ = kPTUnknown;
	//
}

CAMImageLabel::~CAMImageLabel() {
	//...
}

void CAMImageLabel::mouseMoveEvent(QMouseEvent* event) {

}
	
void CAMImageLabel::mousePressEvent(QMouseEvent* event) {

}

void CAMImageLabel::mouseReleaseEvent(QMouseEvent* event) {
	int pt_x = event->x();
	int pt_y = event->y();

	if(selected_points_.size() >= 3) {
		return;
	}




// 	switch (curr_set_point_type_) {
// 	case kPTLinePoint:
// 		if(line_points_.size() >= 2) {
// 			line_points_.dequeue();
// 		}
// 		line_points_.enqueue(cv::Point(pt_x,pt_y));
// 		break;
// 	case kPTCirclePoint:
// 		if(circle_points_.size() >= 3) {
// 			circle_points_.dequeue();
// 		}
// 		circle_points_.enqueue(cv::Point(pt_x,pt_y));
// 		break;
// 	default:
// 		break;
// 	}
	//

	QImage drawed_image = cv_image_helper_->DrawUserSettedPoint(line_points_,circle_points_);
	this->setPixmap(QPixmap::fromImage(drawed_image));

	if(curr_set_point_type_ == kPTLinePoint) {
		emit SIGSetPointComplete(curr_set_point_type_,line_points_);
	} else if (curr_set_point_type_ == kPTCirclePoint) {
		emit SIGSetPointComplete(curr_set_point_type_,circle_points_);
	} else {
		//...
	}


	//emit SIGSetPointComplete(curr_set_point_type_,pt_x,pt_y);
	double curr_angle = cv_image_helper_->GetCurrAngle();
	if(curr_angle != 0.0) {
		emit SIGCalcAngleComplete(curr_angle);
	}
	return;
}

void CAMImageLabel::SetCurrSetPointType(SetPointType pt) {
	curr_set_point_type_ = pt;
}

void CAMImageLabel::ClearAllPoints() {
	line_points_.clear();
	circle_points_.clear();

	QImage drawed_image = cv_image_helper_->DrawUserSettedPoint(line_points_,circle_points_);
	this->setPixmap(QPixmap::fromImage(drawed_image));
	return;
}

void CAMImageLabel::ClearSinglePoint(SetPointType pt) {
	switch (pt) {
	case kPTLinePoint:
		line_points_.clear();
		break;
	case kPTCirclePoint:
		circle_points_.clear();
		break;
	default:
		break;
	}

	QImage drawed_image = cv_image_helper_->DrawUserSettedPoint(line_points_,circle_points_);
	this->setPixmap(QPixmap::fromImage(drawed_image));
}

bool CAMImageLabel::LoadImage(const QString& image_path) {

	if(cv_image_helper_ != NULL) {
		//clear old image
		delete cv_image_helper_;
	}

	cv_image_helper_ = new CamCVImageHelper;
	if(!cv_image_helper_->LoadImage(image_path)) {
		return false;
	}

	QImage cv_loaded_image = cv_image_helper_->GetAdjustImage();
	this->setPixmap(QPixmap::fromImage(cv_loaded_image));

	return true;
}