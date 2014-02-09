////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Color Cluster Segmentation Stereo Tracking
//
//		Author: Pablo Ramón Soria
//		Date: 2013/10/22
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Color Cluster Space
//		Here is defined the limits of every color and functions to truncate colors.

#include "ColorSpaceHSV8.h"

namespace vision {
	namespace segmentation {
		int bin2dec(std::string bin) {
			const char *cstr = bin.c_str();
			int len, dec = 0, i, exp;

			len = strlen(cstr);
			exp = len - 1;

			for (i = 0; i < len; i++, exp--)
				dec += cstr[i] == '1' ? int(pow(2, exp)) : 0;
			return dec;
		}

		ColorClusterSpace *CreateHSVCS_8c(int MaskH, int MaskS, int MaskV) {

			uint8_t HClass[36];
			uint8_t SClass[36];
			uint8_t VClass[36];

			for (int i = 0; i < 36; i++) {
				HClass[i] = bin2dec(HClassStr8[i]) & MaskH;
				SClass[i] = bin2dec(SClassStr8[i]) & MaskS;
				VClass[i] = bin2dec(VClassStr8[i]) & MaskV;
			}

			return new ColorClusterSpace(36, HClass, SClass, VClass, colorsHSV8);

		}
		void imageBGR2HSV(cv::Mat& img) {
			cv::cvtColor(img, img, cv::COLOR_BGR2HSV);
		}
		void imageHSV2BGR(cv::Mat& img) {
			cv::cvtColor(img, img, cv::COLOR_HSV2BGR);
		}
	} // namespace segmentation
} // namespace vision
