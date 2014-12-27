#ifndef _CAM_2D_Helper_
#define _CAM_2D_Helper_

#include "opencv.hpp"
#include <QVector>


class CAM2DHelper {
public:
	static bool GetCollisionPointsBetweenLineAndCircle(cv::Point line_point_1,
		cv::Point line_point_2, 
		cv::Point circle_center_point,
		int circle_radius,
		QVector<cv::Point>& collision_points);

	static bool GetAnotherPointInCircleTangent(cv::Point tang_point,
		cv::Point circle_center_point,
		int circle_radius,cv::Point& another_point);

	static double CalcAngle(const QVector<cv::Point>& points);

	//static bool GetTangentPointsAtCircle(cv::Point )
};









#endif