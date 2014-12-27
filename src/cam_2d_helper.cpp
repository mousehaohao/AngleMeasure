#include "cam_2d_helper.h"

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

bool CAM2DHelper::GetAnotherPointInCircleTangent(cv::Point tang_point,
		cv::Point circle_center_point,
		int circle_radius,cv::Point& another_point) {
	//
	cv::Point local_p1 = tang_point - circle_center_point;
	cv::Point local_p2;
	local_p2.x = local_p1.x + 100;
	local_p2.y = (circle_radius*circle_radius - (local_p1.x * local_p2.x))/local_p1.y;

	another_point = circle_center_point + local_p2;

	if(another_point.y > tang_point.y) {
		cv::Point local_p2_another;
		local_p2_another.x = local_p1.x - 100;
		local_p2_another.y = (circle_radius*circle_radius - (local_p1.x * local_p2_another.x))/local_p1.y;
		another_point = circle_center_point + local_p2_another;
	}
	return true;
}


double CAM2DHelper::CalcAngle(const QVector<cv::Point>& points) {
	if(points.size() < 3) {
		return 0;
	}
	//
	double x1,y1,x2,y2,x4,y4;
	x1 = points[0].x;
	y1 = points[0].y;
	//
	x2 = points[1].x;
	y2 = points[1].y;
	//
	x4 = points[2].x;
	y4 = points[2].y;
	//
	double ma_x = x1 - x2;  
	double ma_y = y1 - y2;  
	double mb_x = x4 - x2;  
	double mb_y = y4 - y2;  
	double v1 = (ma_x * mb_x) + (ma_y * mb_y);  
	double ma_val = sqrt(ma_x*ma_x + ma_y*ma_y);  
	double mb_val = sqrt(mb_x*mb_x + mb_y*mb_y);  
	double cosM = v1 / (ma_val*mb_val);  
	double angleAMB = acos(cosM) * 180 / M_PI;
	return angleAMB;
}



bool CAM2DHelper::GetCollisionPointsBetweenLineAndCircle(cv::Point line_point_1,
		cv::Point line_point_2, 
		cv::Point circle_center_point,
		int circle_radius,
		QVector<cv::Point>& collision_points) {
	cv::Point local_p1 = line_point_1 - circle_center_point;
	cv::Point local_p2 = line_point_2 - circle_center_point;
	//
	cv::Point line_delta_point = local_p2 - local_p1;
	double a = line_delta_point.x*line_delta_point.x + line_delta_point.y*line_delta_point.y;
	double b = 2*((line_delta_point.x*local_p1.x) + (line_delta_point.y * local_p1.y));
	double c = local_p1.x * local_p1.x + local_p1.y*local_p1.y - circle_radius*circle_radius;
	double delta = b*b - 4*a*c;
	if(delta < 0) {
		return false;
	} else if(delta == 0) {
		double u = -b / (2 * a);
		cv::Point col_point = line_point_1 +  (u * line_delta_point);
		collision_points.push_back(col_point);
	} else {
		double sqrt_delta = sqrt(delta);
		double u1 = (-b + sqrt_delta) / (2 * a);
		double u2 = (-b - sqrt_delta) / (2 * a);
		cv::Point col_point1 = line_point_1 + u1*line_delta_point;
		cv::Point col_point2 = line_point_1 + u2*line_delta_point;
		collision_points.push_back(col_point1);
		collision_points.push_back(col_point2);
	}
	return true;
}