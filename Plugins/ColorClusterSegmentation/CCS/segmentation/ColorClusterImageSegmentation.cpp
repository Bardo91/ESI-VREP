////////////////////////////////////////////////////////////////////////////////
//	Visual Tracking UI
//		Author: Pablo Ramón Soria
//		Date: 2013/10/23
////////////////////////////////////////////////////////////////////////////////
// Color cluster segmentation algorithm.


#include "ColocClusterImageSegmentation.h"
#include "ColorSpaceHSV8.h"
#include "SegmentedObject.h"


#include <opencv/highgui.h>

using namespace cv;
using namespace vision::segmentation;

namespace vision {
	namespace segmentation{
		int ColorClusterImageSegmentation(Mat& _frame1, Mat& _frame2, ColorClusterSpace& _CS, const unsigned int _threshold,
				vector<SimpleObject>& _objects1, 
				vector<SimpleObject>& _objects2) {

			imageBGR2HSV(_frame1);
			imageBGR2HSV(_frame2);

			// 666 TODO: use statics variables to save time (allocating, etc...)
			vector<vector<struct LineObjRLE> > aRLE1;
			vector<vector<struct LineObjRLE> > aRLE2;
			vector<SegmentedObject> objs1;
			vector<SegmentedObject> objs2;

			aRLE1.reserve(50000);
			aRLE2.reserve(50000);

			objs1.reserve(5000);
			objs2.reserve(5000);
			

			int n = _frame1.channels(); // Count the number of image's channels to use the pointer

			int color1;
			int color2;
			short int js1 = 0, js2 = 0, colorRLE1 = -1, colorRLE2 = -1; // Variables for RLE encode
			for (int i = 0; i < _frame1.rows; i++) {
				uchar* ptr1 = _frame1.ptr<uchar>(i); // Pointer to i row
				uchar* ptr2 = _frame2.ptr<uchar>(i); // Pointer to i row
				vector<LineObjRLE> temp1;
				vector<LineObjRLE> temp2;
				for (int j = 0; j < _frame1.cols; j++) {
					// Proximate the color to a cluster
					//First Camera ---------------------------------------------------------------
					c3i auxCol1;
					auxCol1.a = ptr1[n * j];
					auxCol1.b = ptr1[n * j + 1];
					auxCol1.c = ptr1[n * j + 2];
					color1 = _CS.whichColor(auxCol1);

					// RLE encoding
					if (j == 0) {
						colorRLE1 = color1;
						js1 = 0;
					} else {
						if (j == _frame1.cols - 1) {
							LineObjRLE aux;
							aux.i = i;
							aux.js = js1;
							aux.je = j;
							aux.size = j - js1;
							aux.color = colorRLE1;
							aux.parent = NULL;
							aux.iObj = -1;

							temp1.push_back(aux);
						} else if (color1 != colorRLE1) {
							LineObjRLE aux;
							aux.i = i;
							aux.js = js1;
							aux.je = j;
							aux.size = j - js1;
							aux.color = colorRLE1;
							aux.parent = NULL;
							aux.iObj = -1;

							temp1.push_back(aux);
							colorRLE1 = color1;
							js1 = j;
						}
					}

					// Change the color (Possible improve assigning directly the BGR color instead of using imageHSV2BGR)

					if (color1 == -1) {
						ptr1[n * j] = _CS.clusters[0].a;
						ptr1[n * j + 1] = _CS.clusters[0].b;
						ptr1[n * j + 2] = _CS.clusters[0].c;
					} else {
						ptr1[n * j] = _CS.clusters[color1].a;
						ptr1[n * j + 1] = _CS.clusters[color1].b;
						ptr1[n * j + 2] = _CS.clusters[color1].c;
					}

					// Second Camera ---------------------------------------------------------------

					c3i auxCol2;
					auxCol2.a = ptr2[n * j];
					auxCol2.b = ptr2[n * j + 1];
					auxCol2.c = ptr2[n * j + 2];
					color2 = _CS.whichColor(auxCol2);

					if (j == 0) {
						colorRLE2 = color2;
						js2 = 0;
					} else {
						if (j == _frame2.cols - 1) {
							LineObjRLE aux;
							aux.i = i;
							aux.js = js2;
							aux.je = j;
							aux.size = j - js2;
							aux.color = colorRLE2;
							aux.parent = NULL;
							aux.iObj = -1;
							temp2.push_back(aux);
						} else if (color2 != colorRLE2) {
							LineObjRLE aux;
							aux.i = i;
							aux.js = js2;
							aux.je = j;
							aux.size = j - js2;
							aux.color = colorRLE2;
							aux.parent = NULL;
							aux.iObj = -1;

							temp2.push_back(aux);
							colorRLE2 = color2;
							js2 = j;
						}
					}

					if (color2 == -1) {
						ptr2[n * j] = _CS.clusters[0].a;
						ptr2[n * j + 1] = _CS.clusters[0].b;
						ptr2[n * j + 2] = _CS.clusters[0].c;
					} else {
						ptr2[n * j] = _CS.clusters[color2].a;
						ptr2[n * j + 1] = _CS.clusters[color2].b;
						ptr2[n * j + 2] = _CS.clusters[color2].c;
					}

				}

				aRLE1.push_back(temp1);
				aRLE2.push_back(temp2);

				if (i) { // Except the first line that can't be child of any object (only parent) start joining objects grouped in LineObjRLE variables
					// First Camera ---------------------------------------------------------------
					unsigned int j = 0, jp = 0; // Pointer to current and previous LineObjRLE
					unsigned int pp = aRLE1[i - 1][jp].size, pc = aRLE1[i][j].size; // Pointer to previous and current col
					while (1) {
						// Connecting
						if (!(aRLE1[i - 1][jp].color == -1 || aRLE1[i][j].color == -1)) {
							if (aRLE1[i - 1][jp].color == aRLE1[i][j].color
									&& aRLE1[i - 1][jp].je >= aRLE1[i][j].js
									&& aRLE1[i - 1][jp].js <= aRLE1[i][j].je) {

								if (aRLE1[i][j].parent == NULL) { // Solve overlap problem checking parent
									if (aRLE1[i - 1][jp].parent != NULL) { // Another parent
										aRLE1[i][j].parent = aRLE1[i - 1][jp].parent;
									} else {
										// Is first parent
										aRLE1[i][j].parent = &(aRLE1[i - 1][jp]);
									}
								} else { //In case of overlap
									if (aRLE1[i - 1][jp].parent != NULL) { // New family
										if (aRLE1[i - 1][jp].parent
												!= aRLE1[i][j].parent) {
											aRLE1[i - 1][jp].parent->parent =
													aRLE1[i][j].parent;
										}

									} else {
										// Orphan
										aRLE1[i - 1][jp].parent = aRLE1[i][j].parent;
									}
								}
							}
						}
						if (j >= aRLE1[i].size() - 1 || jp >= aRLE1[i - 1].size() - 1)
							break;
						if (pp > pc) {
							j++;
							pc += aRLE1[i][j].size;
						} else if (pp <= pc) {
							jp++;
							pp += aRLE1[i - 1][jp].size;
						}
					}
					// Second Camera ---------------------------------------------------------------
					j = 0, jp = 0; // Pointer to current and previous LineObjRLE
					pp = aRLE2[i - 1][jp].size, pc = aRLE2[i][j].size; // Pointer to previous and current col
					while (1) {
						// Connecting
						if (!(aRLE2[i - 1][jp].color == -1 || aRLE2[i][j].color == -1)) {
							if (aRLE2[i - 1][jp].color == aRLE2[i][j].color
									&& aRLE2[i - 1][jp].je >= aRLE2[i][j].js
									&& aRLE2[i - 1][jp].js <= aRLE2[i][j].je) {

								if (aRLE2[i][j].parent == NULL) { // Solve overlap problem checking parent
									if (aRLE2[i - 1][jp].parent != NULL) { // Another parent
										aRLE2[i][j].parent = aRLE2[i - 1][jp].parent;
									} else {
										// Is first parent
										aRLE2[i][j].parent = &(aRLE2[i - 1][jp]);
									}
								} else { //In case of overlap
									if (aRLE2[i - 1][jp].parent != NULL) { // New family
										if (aRLE2[i - 1][jp].parent
												!= aRLE2[i][j].parent) {
											aRLE2[i - 1][jp].parent->parent =
													aRLE2[i][j].parent;
										}

									} else {
										// Orphan
										aRLE2[i - 1][jp].parent = aRLE2[i][j].parent;
									}
								}
							}
						}
						if (j >= aRLE2[i].size() - 1 || jp >= aRLE2[i - 1].size() - 1)
							break;
						if (pp > pc) {
							j++;
							pc += aRLE2[i][j].size;
						} else if (pp <= pc) {
							jp++;
							pp += aRLE2[i - 1][jp].size;
						}
					}
				}

			}
			// First Camera ---------------------------------------------------------------
			//Re-assign parents due to overlap
			for (unsigned int i = 0; i < aRLE1.size(); i++) {
				for (unsigned int j = 0; j < aRLE1[i].size(); j++) {
					if (aRLE1[i][j].parent != NULL) {
						LineObjRLE auxRLE = *aRLE1[i][j].parent;

						int loopAvoider = 0; // Need to be checked if image size is larger than 320x240. If possible check the origin of loops
						while (auxRLE.parent != NULL && loopAvoider < 30) {
							aRLE1[i][j].parent = auxRLE.parent;
							auxRLE = *auxRLE.parent;
							loopAvoider++;
						}

						if (aRLE1[i][j].parent->iObj == -1) {
							aRLE1[i][j].parent->iObj = objs1.size();
							SegmentedObject obj(*aRLE1[i][j].parent);
							objs1.push_back(obj);
						}
					}
				}
			}
			for (unsigned int i = 0; i < aRLE1.size(); i++) {
				for (unsigned int j = 0; j < aRLE1[i].size(); j++) {
					if (aRLE1[i][j].parent != NULL) {
						objs1[aRLE1[i][j].parent->iObj].addLineObjRLE(aRLE1[i][j]);
					}
				}
			}
			// Second Camera ---------------------------------------------------------------
			for (unsigned int i = 0; i < aRLE2.size(); i++) {
				for (unsigned int j = 0; j < aRLE2[i].size(); j++) {
					if (aRLE2[i][j].parent != NULL) {
						LineObjRLE auxRLE = *aRLE2[i][j].parent;

						int loopAvoider = 0; // Need to be checked if image size is larger than 320x240. If possible check the origin of loops
						while (auxRLE.parent != NULL && loopAvoider < 30) {
							aRLE2[i][j].parent = auxRLE.parent;
							auxRLE = *auxRLE.parent;
							loopAvoider++;
						}

						if (aRLE2[i][j].parent->iObj == -1) {
							aRLE2[i][j].parent->iObj = objs2.size();
							SegmentedObject obj(*aRLE2[i][j].parent);
							objs2.push_back(obj);
						}
					}
				}
			}
			for (unsigned int i = 0; i < aRLE2.size(); i++) {
				for (unsigned int j = 0; j < aRLE2[i].size(); j++) {
					if (aRLE2[i][j].parent != NULL) {
						objs2[aRLE2[i][j].parent->iObj].addLineObjRLE(aRLE2[i][j]);
					}
				}
			}

			for(int i = 0; i < objs1.size() ; i ++){
				if(objs1[i].getSize() >= _threshold)
					_objects1.push_back(SimpleObject(objs1[i].getUpperLeft(), objs1[i].getDownRight(), objs1[i].getSize(), objs1[i].getColor()));
			}
			for(int i = 0; i < objs2.size() ; i ++){
				if(objs2[i].getSize() >= _threshold)
					_objects2.push_back(SimpleObject(objs2[i].getUpperLeft(), objs2[i].getDownRight(), objs2[i].getSize(), objs2[i].getColor()));
			}


			imageHSV2BGR(_frame1);
			imageHSV2BGR(_frame2);

			return 0;
		} // int ColorClusterImageSegmentation(...)
	} // namespace segmentation
} // namespace vision
