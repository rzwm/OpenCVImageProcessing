// http://blog.csdn.net/matrix_space/article/details/40898103
// 高斯模糊与毛玻璃特效
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

void gauss_blur(const cv::Mat& src, cv::Mat& dst)
{
	CV_Assert(src.type() == CV_32FC3);

	dst = src.clone();

	cv::Mat temp = dst.rowRange(100, 300);

	cv::GaussianBlur(temp, temp, cv::Size(21, 21), 0.0);
	cv::GaussianBlur(temp, temp, cv::Size(21, 21), 0.0);
}

int main()
{
	cv::Mat colorImage = cv::imread("../../dataset/girl.jpg");
	if (colorImage.empty())
	{
		return -1;
	}

	cv::imshow("Color image", colorImage);

	colorImage.convertTo(colorImage, CV_32FC3);

	cv::Mat resultImage;
	gauss_blur(colorImage, resultImage);
	resultImage.convertTo(resultImage, CV_8UC3);
	cv::imshow("Result image", resultImage);

	cv::waitKey();
	return 0;
}
