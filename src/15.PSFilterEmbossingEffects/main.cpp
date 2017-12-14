// http://blog.csdn.net/matrix_space/article/details/29193433
// http://blog.csdn.net/matrix_space/article/details/40431061
// PS滤镜：浮雕特效
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

void embossing_effects(const cv::Mat& src, cv::Mat& dst)
{
	CV_Assert(src.type() == CV_32FC3);

	const int p = 3;
	const int kernel[] = 
	{
		0, 0, -p,	// 注意此处与原文的不同
		0, 0,  0,
		p, 0,  0
	};
	cv::Mat kernelMat(cv::Size(3, 3), CV_32SC1, (void*)kernel);

	cv::filter2D(src, dst, -1, kernelMat);
	cv::add(dst, cv::Scalar(128.0, 128.0, 128.0), dst);
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
	embossing_effects(colorImage, resultImage);
	resultImage.convertTo(resultImage, CV_8UC3);
	cv::imshow("Result image", resultImage);

	cv::waitKey();
	return 0;
}
