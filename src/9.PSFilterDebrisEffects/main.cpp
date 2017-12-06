// http://blog.csdn.net/matrix_space/article/details/40477085
// PS滤镜：碎片特效
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

// 碎片特效
void debris_effects(const cv::Mat& src, cv::Mat& dst)
{
	CV_Assert(src.type() == CV_32FC3);

	cv::Mat imageUp = src.clone();
	cv::Mat imageDown = src.clone();
	cv::Mat imageLeft = src.clone();
	cv::Mat imageRight = src.clone();

	const int offset = 5;
	const int h = src.rows;
	const int w = src.cols;

	cv::Mat tempSrc;
	cv::Mat tempDst;

	// down
	tempSrc = src.rowRange(0, h - 1 - offset);
	tempDst = imageDown.rowRange(offset, h - 1);
	tempSrc.copyTo(tempDst);

	// up
	tempSrc = src.rowRange(offset, h - 1);
	tempDst = imageUp.rowRange(0, h - 1 - offset);
	tempSrc.copyTo(tempDst);

	// left
	tempSrc = src.colRange(offset, w - 1);
	tempDst = imageLeft.colRange(0, w - 1 - offset);
	tempSrc.copyTo(tempDst);

	// right
	tempSrc = src.colRange(0, w - 1 - offset);
	tempDst = imageRight.colRange(offset, w - 1);
	tempSrc.copyTo(tempDst);

	dst = 0.25f * (imageUp + imageDown + imageLeft + imageRight);
}

int main()
{
	cv::Mat colorImage = cv::imread("../../dataset/flower.jpg");
	if (colorImage.empty())
	{
		return -1;
	}

	cv::imshow("Color image", colorImage);

	colorImage.convertTo(colorImage, CV_32FC3, 1.0 / 255.0);

	cv::Mat resultImage;
	debris_effects(colorImage, resultImage);
	resultImage.convertTo(resultImage, CV_8UC3, 255.0);
	cv::imshow("Result image", resultImage);

	cv::waitKey();
	return 0;
}
