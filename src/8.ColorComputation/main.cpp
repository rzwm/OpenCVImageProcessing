// http://blog.csdn.net/matrix_space/article/details/40263809
// 颜色运算
#include <vector>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

void color_computation(const cv::Mat& src, cv::Mat& dst)
{
	CV_Assert(src.type() == CV_32FC3);

	std::vector<cv::Mat> vecBGR;
	cv::split(src, vecBGR);
	cv::Mat B = vecBGR[0];
	cv::Mat G = vecBGR[1];
	cv::Mat R = vecBGR[2];

	const float alpha = 0.15f;

	cv::Mat dstB = B * alpha / (R + G + 0.001f);
	cv::Mat dstG = G * alpha / (R + B + 0.001f);
	cv::Mat dstR = R * alpha / (B + G + 0.001f);

	std::vector<cv::Mat> vecDstBGR;
	vecDstBGR.push_back(dstB);
	vecDstBGR.push_back(dstG);
	vecDstBGR.push_back(dstR);
	cv::merge(vecDstBGR, dst);
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
	color_computation(colorImage, resultImage);
	resultImage.convertTo(resultImage, CV_8UC3, 255.0);
	cv::imshow("Result image", resultImage);

	cv::waitKey();
	return 0;
}
