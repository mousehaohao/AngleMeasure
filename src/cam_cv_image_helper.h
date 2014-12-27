#ifndef _CAM_CV_IMAGE_HELPER_H_
#define _CAM_CV_IMAGE_HELPER_H_

#include <QString>
#include <QImage>
#include <QVector>
#include "opencv.hpp"
#include <QQueue>

typedef struct {
	cv::Point center_point;
	int radius;
} CircleParams;

class CamCVImageHelper {
public:
	CamCVImageHelper();
	~CamCVImageHelper();
public:
	bool LoadImage(const QString& image_path);
	QImage GetOriginImage();
	QImage GetAdjustImage();
	QImage GetProcessedImage();
	
	QImage DrawUserSettedPoint(const QQueue<cv::Point>& line_points,
		const QQueue<cv::Point>& circle_points);

	double GetCurrAngle() {
		return curr_angle_;
	}

private:
	QImage ConvertMatToQImage(const cv::Mat& mat);
	cv::Mat GetImageContours(const cv::Mat& mat);

	bool CalcLeastSquaresFitting(const QQueue<cv::Point>& circle_points, CircleParams& circle_params);
	bool IsPointValid(const cv::Point& pt);

private:
	cv::Mat cv_origin_image_;
	cv::Mat cv_adjust_image_;
	QString image_path_;

	double curr_angle_;

	bool is_line_ok_;
	bool is_circle_ok_;


};




#endif