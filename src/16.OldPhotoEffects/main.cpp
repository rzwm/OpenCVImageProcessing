// http://blog.csdn.net/matrix_space/article/details/40432125
// 老照片效果
#include <vector>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

void old_photo_effects(const cv::Mat& src, cv::Mat& dst)
{
	CV_Assert(src.type() == CV_32FC3);

	std::vector<cv::Mat> vecBGR;
	cv::split(src, vecBGR);
	cv::Mat B = vecBGR[0];
	cv::Mat G = vecBGR[1];
	cv::Mat R = vecBGR[2];

	cv::Mat dstB = 0.272 * R + 0.523 * G + 0.132 * B;
	cv::Mat dstG = 0.349 * R + 0.686 * G + 0.168 * B;
	cv::Mat dstR = 0.393 * R + 0.769 * G + 0.189 * B;

	std::vector<cv::Mat> vecDstBGR;
	vecDstBGR.push_back(dstB);
	vecDstBGR.push_back(dstG);
	vecDstBGR.push_back(dstR);
	cv::merge(vecDstBGR, dst);
}

int main()
{
	cv::Mat colorImage = cv::imread("../../dataset/girl.jpg");
	if (colorImage.empty())
	{
		return -1;
	}

	cv::imshow("Color image", colorImage);

	colorImage.convertTo(colorImage, CV_32FC3, 1.0 / 255.0);

	cv::Mat resultImage;
	old_photo_effects(colorImage, resultImage);
	resultImage.convertTo(resultImage, CV_8UC3, 255.0);
	cv::imshow("Result image", resultImage);

	cv::waitKey();
	return 0;
}
