////////////////////////////////////////////////////////////////////////////////
//	Visual Tracking UI
//		Author: Pablo Ramón Soria
//		Date: 2013/10/23
////////////////////////////////////////////////////////////////////////////////
// Color cluster segmentation algorithm.

#ifndef _VISUAL_TRACKING_UI_SEGMENTATEIMAGE_H_
#define _VISUAL_TRACKING_UI_SEGMENTATEIMAGE_H_

#include "ColorClusterSpace.h"
#include <Types/SimpleObject.h>

#include <opencv/cv.h>
#include <string>


namespace vision {
	namespace segmentation {
		// Unique frame color cluster segmentation.
		// 666 TODO: implement 1 camera algorithm
		//int ColorClusterImageSegmentation(cv::Mat& frame, ColorClusterSpace& CS, unsigned int threshold, std::vector<SimpleObject>& objects);
		
		// Pair frames color cluster segmentation. Better that execute a couple times the previous algorithm, because reuse loops for both frames.
		int ColorClusterImageSegmentation(cv::Mat& frame1, cv::Mat& frame2, ColorClusterSpace& CS, const unsigned int threshold, std::vector<SimpleObject>& objects1, std::vector<SimpleObject>& objects2);
		
	} // namespace segmentation
} // namespace vision


#endif // _VISUAL_TRACKING_UI_SEGMENTATEIMAGE_H_
