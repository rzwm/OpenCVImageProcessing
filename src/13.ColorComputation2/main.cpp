// http://blog.csdn.net/matrix_space/article/details/40402373
// 颜色运算（二）
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

	cv::Mat resultB;
	cv::Mat resultG;
	cv::Mat resultR;

	cv::pow(R - G, 2.0, resultB);
	cv::pow(R - B, 2.0, resultG);
	cv::pow(G - B, 2.0, resultR);

	resultB = resultB / 128.0f;
	resultG = resultG / 128.0f;
	resultR = resultR / 128.0f;

	std::vector<cv::Mat> vecResultBGR;
	vecResultBGR.push_back(resultB);
	vecResultBGR.push_back(resultG);
	vecResultBGR.push_back(resultR);
	cv::merge(vecResultBGR, dst);
}

int main()
{
	cv::Mat colorImage = cv::imread("../../dataset/flower.jpg");
	if (colorImage.empty())
	{
		return -1;
	}

	cv::imshow("Color image", colorImage);

	colorImage.convertTo(colorImage, CV_32FC3);

	cv::Mat resultImage;
	color_computation(colorImage, resultImage);
	resultImage.convertTo(resultImage, CV_8UC3);
	cv::imshow("Result image", resultImage);

	cv::waitKey();
	return 0;
}
