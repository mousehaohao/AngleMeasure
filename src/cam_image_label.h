#ifndef _CAM_IMAGE_LABEL_H_
#define _CAM_IMAGE_LABEL_H_

#include <QLabel>
#include <QEvent>
#include <QString>
#include <QQueue>

#include "cam_cv_image_helper.h"

typedef enum _tag_set_point_type{
	kPTLinePoint,
	kPTCirclePoint,
	kPTUnknown
} SetPointType;

class CAMImageLabel : public QLabel {
	Q_OBJECT
public:
	CAMImageLabel();
	~CAMImageLabel();

public:
	bool LoadImage(const QString& image_path);
	void SetCurrSetPointType(SetPointType pt);
	void ClearSinglePoint(SetPointType pt);
	void ClearAllPoints();

signals:
	void SIGSetPointComplete(SetPointType set_point_type, const QQueue<cv::Point>& line_points);
	void SIGCalcAngleComplete(double angle);

protected:
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

private:
	CamCVImageHelper* cv_image_helper_;
	SetPointType curr_set_point_type_;
	QQueue<cv::Point> line_points_;
	QQueue<cv::Point> circle_points_;

	/* 目前只取前三个点,多余的点视为无效点 */
	/* 其中第一点和第三点作为水平线的点 */
	QQueue<cv::Point> selected_points_;



};









#endif