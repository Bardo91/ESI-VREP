////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Color Cluster Segmentation Stereo Tracking
//
//		Author: Pablo Ramón Soria
//		Date: 2013/10/22
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Color Cluster Space
//		Here is defined the limits of every color and functions to truncate colors.

#include "ColorClusterSpace.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <cmath>

namespace vision {
	namespace segmentation {
		ColorClusterSpace::ColorClusterSpace(int n, uint8_t *AClass, uint8_t *BClass,
				uint8_t *CClass, const c3i *colors) {

			this->AClass = new uint8_t[n];
			this->BClass = new uint8_t[n];
			this->CClass = new uint8_t[n];
			clusters = new c3i[8];
			size = n;

			for (int i = 0; i < n; i++) {
				this->AClass[i] = AClass[i];
				this->BClass[i] = BClass[i];
				this->CClass[i] = CClass[i];
				if (i < 8)
					clusters[i] = colors[i];
			}
		}

		ColorClusterSpace::~ColorClusterSpace() {
			delete[] AClass;
			delete[] BClass;
			delete[] CClass;
			delete[] clusters;

		}
	} // namespace segmentation
} // namespace vision
