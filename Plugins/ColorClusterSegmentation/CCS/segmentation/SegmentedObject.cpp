/*
 * SegmentededObject.cpp
 *
 *  Created on: Oct 23, 2013
 *      Author: pablo
 */

#include "SegmentedObject.h"

namespace vision {
	namespace segmentation {
		SegmentedObject::SegmentedObject(LineObjRLE ini) {
			upperLeft.x = 9999;
			downRight.x = 0;
			upperLeft.y = 9999;
			downRight.y = 0;
			size = 0;
			color = ini.color;
			addLineObjRLE(ini);

		}

		void SegmentedObject::addLineObjRLE(LineObjRLE aux) {
			obj.push_back(aux);

			if (upperLeft.y > aux.i)
				upperLeft.y = aux.i;
			if (upperLeft.x > aux.js)
				upperLeft.x = aux.js;
			if (downRight.y < aux.i)
				downRight.y = aux.i;
			if (downRight.x < aux.je)
				downRight.x = aux.je;
			size += aux.size + 1;
		}

		void SegmentedObject::addRLEFamily(SegmentedObject& family) {
			for (int k = 0; k < family.getLines(); k++) {
				LineObjRLE aux = family.getRLEObj(k);
				obj.push_back(aux);
				if (upperLeft.y > aux.i)
					upperLeft.y = aux.i;
				if (upperLeft.x > aux.js)
					upperLeft.x = aux.js;
				if (downRight.y < aux.i)
					downRight.y = aux.i;
				if (downRight.x < aux.je)
					downRight.x = aux.je;
				size += aux.size + 1;
			}
		}

		int SegmentedObject::getLines() const {
			return obj.size();
		}

		LineObjRLE SegmentedObject::getRLEObj(int k) const {
			return obj[k];
		}

		cv::Point2d SegmentedObject::getUpperLeft() const {
			return upperLeft;
		}

		cv::Point2d SegmentedObject::getDownRight() const {
			return downRight;
		}

		unsigned int SegmentedObject::getColor() const {
			return color;
		}

		unsigned int SegmentedObject::getSize() const {
			return size;
		}

		unsigned int SegmentedObject::getBBSize() const {
			return int((downRight.x - upperLeft.x) * (downRight.y - upperLeft.y));
		}

		cv::Point2d SegmentedObject::getCentroid() const {
			return cv::Point(int((upperLeft.x + downRight.x) / 2),
					int((upperLeft.y + downRight.y) / 2));
		}

		bool sortFunction(LineObjRLE a, LineObjRLE b) {
			return a.i < b.i || (a.i == b.i && a.je < b.js) ? true : false;
		}

		void SegmentedObject::sortObj() {
			std::sort(obj.begin(), obj.end(), sortFunction);
		}
	} // namespace segmentation
} // namespace vision

