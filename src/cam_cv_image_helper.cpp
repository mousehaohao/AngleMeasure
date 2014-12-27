#include "cam_cv_image_helper.h"

#include "cam_2d_helper.h"

#include <QMessageBox>

CamCVImageHelper::CamCVImageHelper() {
	is_line_ok_ = false;
	is_circle_ok_ = false;
	curr_angle_ = 0.0;
}

CamCVImageHelper::~CamCVImageHelper() {

}

bool CamCVImageHelper::LoadImage(const QString& image_path) {
	image_path_ = image_path;
	QByteArray ba = image_path.toLatin1();
	const char* c_image_path = ba.data();
	//
	cv_origin_image_ = cv::imread(c_image_path, CV_LOAD_IMAGE_COLOR);
	if(!cv_origin_image_.data) {
		return false;
	}
	//
	if(cv_origin_image_.cols > 800) {
		pyrDown(cv_origin_image_,cv_adjust_image_,cv::Size(cv_adjust_image_.cols/2,cv_adjust_image_.rows/2));
	} else {
		cv_adjust_image_ = cv_origin_image_.clone();
	}
	return true;
}

QImage CamCVImageHelper::GetOriginImage() {
	return ConvertMatToQImage(cv_origin_image_);
}

QImage CamCVImageHelper::GetAdjustImage() {
	return ConvertMatToQImage(cv_adjust_image_);
}

QImage CamCVImageHelper::GetProcessedImage() {
	cv::Mat processed_image = GetImageContours(cv_adjust_image_);
	return ConvertMatToQImage(processed_image);
}

/* by default cv::Mat are in the Blue Green Red (BGR) */
cv::Mat CamCVImageHelper::GetImageContours(const cv::Mat& mat) {
	//
	cv::Mat gray_mat;
	cv::cvtColor(mat, gray_mat, CV_BGR2GRAY);
	//
	cv::Mat threshold_mat;
	cv::threshold(gray_mat,threshold_mat,100,255,cv::THRESH_BINARY);
	//
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(threshold_mat, 
		contours, // a vector of contours 
		CV_RETR_EXTERNAL, // retrieve the external contours
		CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours
	//
	cv::Mat result(threshold_mat.size(),CV_8U,cv::Scalar(255));
	cv::drawContours(result,contours,
		-1, // draw all contours
		cv::Scalar(0), // in black
		2); // with a thickness of 2

	int cmin= 500;  // minimum contour length
	int cmax= 4000; // maximum contour length
	std::vector<std::vector<cv::Point>>::const_iterator itc= contours.begin();
	while (itc!=contours.end()) {

		if (itc->size() < cmin || itc->size() > cmax)
			itc= contours.erase(itc);
		else 
			++itc;
	}

	cv::Mat mat_with_contours;
	cv_adjust_image_.copyTo(mat_with_contours);
	cv::drawContours(mat_with_contours,contours,
		-1, // draw all contours
		cv::Scalar(255,0,0), // in white
		2); // with a thickness of 2


	return mat_with_contours;
}

bool CamCVImageHelper::IsPointValid(const cv::Point& pt) {
	if(pt.x == 0 && pt.y == 0) {
		return false;
	}
	return true;
}

/* BGR */
QImage CamCVImageHelper::DrawUserSettedPoint(const QQueue<cv::Point>& line_points,
		const QQueue<cv::Point>& circle_points) {

	curr_angle_ = 0;

	cv::Mat processed_image = cv_adjust_image_.clone();
	//
	int point_r = 5;
	is_line_ok_ = true;

	for(int index=0; index<line_points.size(); index++) {
		if(IsPointValid(line_points[index])) {
			const cv::Point& curr_point = line_points[index];
			//cv::circle(processed_image,line_points[index],point_r,cv::Scalar(127,0,255),2);
			cv::line(processed_image,cv::Point(curr_point.x+point_r,curr_point.y+point_r),
				cv::Point(curr_point.x-point_r,curr_point.y-point_r),cv::Scalar(127,0,255),2);
			cv::line(processed_image,cv::Point(curr_point.x-point_r,curr_point.y+point_r),
				cv::Point(curr_point.x+point_r,curr_point.y-point_r),cv::Scalar(127,0,255),2);
		}
	}
	if(line_points.size() < 2) {
		is_line_ok_ = false;
	}
	//
	is_circle_ok_ = true;
	for(int index=0; index<circle_points.size(); index++) {
		if(IsPointValid(circle_points[index])) {
			const cv::Point& curr_point = circle_points[index];
			//cv::circle(processed_image,circle_points[index],point_r,cv::Scalar(0,204,102),2);
			cv::line(processed_image,cv::Point(curr_point.x+point_r,curr_point.y+point_r),
				cv::Point(curr_point.x-point_r,curr_point.y-point_r),cv::Scalar(0,204,102),2);
			cv::line(processed_image,cv::Point(curr_point.x-point_r,curr_point.y+point_r),
				cv::Point(curr_point.x+point_r,curr_point.y-point_r),cv::Scalar(0,204,102),2);
		}
	}
	if(circle_points.size() < 3) {
		is_circle_ok_ = false;
	}
	//
	if(is_line_ok_) {
		cv::line(processed_image,line_points[0],line_points[1],cv::Scalar(127,0,255),2);
	}
	CircleParams circle_params;
	if(is_circle_ok_) {
		if(CalcLeastSquaresFitting(circle_points,circle_params)) {
			cv::circle(processed_image,circle_params.center_point,circle_params.radius,cv::Scalar(255,255,0),2);
		}
	}

	if(is_line_ok_ && is_circle_ok_) {
		do {
			QVector<cv::Point> coll_points;
			bool is_get_coll_points_succ = CAM2DHelper::GetCollisionPointsBetweenLineAndCircle(line_points[0],
				line_points[1],
				circle_params.center_point,
				circle_params.radius,coll_points);
			if(is_get_coll_points_succ) {
				for(int index=0; index<coll_points.size();index++) {
					const cv::Point& curr_point = coll_points[index];
					//cv::circle(processed_image,coll_points[index],point_r,cv::Scalar(0,128,255),2);
					cv::line(processed_image,cv::Point(curr_point.x+point_r,curr_point.y+point_r),
						cv::Point(curr_point.x-point_r,curr_point.y-point_r),cv::Scalar(0,128,255),2);
					cv::line(processed_image,cv::Point(curr_point.x-point_r,curr_point.y+point_r),
						cv::Point(curr_point.x+point_r,curr_point.y-point_r),cv::Scalar(0,128,255),2);

				}
			} else {
				QMessageBox::about(NULL, QStringLiteral("接触角测量"), 
					QStringLiteral("水平面与液滴不相交,请重新设置水平面与液滴面"));
				break;
			}
			//
			cv::Point calc_tangent_point;
			if(coll_points.size() == 1) {
				calc_tangent_point = coll_points[0];
			} else {
				calc_tangent_point = coll_points[0].x < coll_points[1].x ? coll_points[0] : coll_points[1];
			}
			cv::Point another_point;
			CAM2DHelper::GetAnotherPointInCircleTangent(calc_tangent_point,
				circle_params.center_point,
				circle_params.radius,another_point);
			//
			cv::circle(processed_image,another_point,point_r,cv::Scalar(0,255,255),2);
			cv::line(processed_image,calc_tangent_point,another_point,cv::Scalar(0,255,255),2);
			//
			QVector<cv::Point> angle_points;
			angle_points.push_back(coll_points[0]);
			angle_points.push_back(coll_points[1]);
			angle_points.push_back(another_point);
			curr_angle_ = CAM2DHelper::CalcAngle(angle_points);
		} while(0);
	}

	return ConvertMatToQImage(processed_image);
}

QImage CamCVImageHelper::ConvertMatToQImage(const cv::Mat& mat) {
	QImage* image = NULL;
	if(!mat.data) {
		return QImage();
	}
	//
	if(mat.type() == CV_8UC1) {
		static QVector<QRgb> sColorTable;
		// only create our color table once
		if ( sColorTable.isEmpty() ) {
			for ( int i = 0; i < 256; ++i ) {
				sColorTable.push_back( qRgb( i, i, i ) );
			}
		}
		QImage image( mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8 );
		image.setColorTable( sColorTable );
		return image;
	} else if (mat.type() == CV_8UC3) {
		// Copy input Mat
		QImage image( mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888 );
		return image.rgbSwapped();
	} else if (mat.type() == CV_8UC4){
		QImage image( mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB32 );
		return image;
	}

	return QImage();

}

/* http://www.cnblogs.com/dotLive/archive/2006/10/09/524633.html */
bool CamCVImageHelper::CalcLeastSquaresFitting(const QQueue<cv::Point>& circle_points, CircleParams& circle_params) {
	if(circle_points.size() < 3) {
		return false;
	}
	double X1=0;
	double Y1=0;
	double X2=0;
	double Y2=0;
	double X3=0;
	double Y3=0;
	double X1Y1=0;
	double X1Y2=0;
	double X2Y1=0;
	for (int index=0; index<circle_points.size(); index++) {
		const cv::Point& po = circle_points[index];
		X1 = X1 + po.x;
		Y1 = Y1 + po.y;
		X2 = X2 + po.x*po.x;
		Y2 = Y2 + po.y*po.y;
		X3 = X3 + po.x*po.x*po.x;
		Y3 = Y3 + po.y*po.y*po.y;
		X1Y1 = X1Y1 +po.x*po.y;
		X1Y2 = X1Y2 + po.x*po.y*po.y;
		X2Y1 = X2Y1 + po.x*po.x*po.y;
	}
	//
	double C,D,E,G,H,N;
	double a,b,c;
	N = circle_points.size();
	C = N*X2 - X1*X1;
	D = N*X1Y1 - X1*Y1;
	E = N*X3 + N*X1Y2 - (X2+Y2)*X1;
	G = N*Y2 - Y1*Y1;
	H = N*X2Y1 + N*Y3 - (X2+Y2)*Y1;
	a = (H*D-E*G)/(C*G-D*D);
	b = (H*C-E*D)/(D*D-G*C);
	c = -(a*X1 + b*Y1 + X2 + Y2)/N;

	double A,B,R;
	A = a/(-2);
	B = b/(-2);
	R = sqrt(a*a+b*b-4*c)/2;

	circle_params.center_point.x = A;
	circle_params.center_point.y = B;
	circle_params.radius = R;
	
	return true;
}

