////////////////////////////////////////////////////////////////////////////////
//	Visual Tracking UI
//		Author: Pablo Ramón Soria
//		Date: 2013/11/29
////////////////////////////////////////////////////////////////////////////////
// SimpleObject


#include "SimpleObject.h"

using namespace cv;

namespace vision{
	//------------------------------------------------------------------------
	SimpleObject::SimpleObject(){
		upperLeft = Point(0,0);
		downRight = Point(0,0);
		centroid = Point(0,0);

		size = 1;

		color = -1;
	}

	//------------------------------------------------------------------------
	SimpleObject::SimpleObject(cv::Point2d& _upperLeft, cv::Point2d& _downRight, unsigned int _size, int _color){
		upperLeft = _upperLeft;
		downRight = _downRight;
		centroid = Point((_upperLeft.x + _downRight.x)/2, (_upperLeft.y + _downRight.y)/2);

		size = _size;

		color = _color;
	}

	//------------------------------------------------------------------------
	double SimpleObject::distanceTo(Point2d& _p2){
		return sqrt((centroid.x - _p2.x)*(centroid.x - _p2.x) + (centroid.y - _p2.y)*(centroid.y - _p2.y));
	}

} // namespace vision