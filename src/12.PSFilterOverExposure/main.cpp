// http://blog.csdn.net/matrix_space/article/details/30470303
// http://blog.csdn.net/matrix_space/article/details/40402141
// PS滤镜：曝光过度特效
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

// 曝光过度特效
void over_exposure(const cv::Mat& src, cv::Mat& dst)
{
	CV_Assert(src.type() == CV_32FC3);

	cv::Mat inverse = cv::Scalar(1.0f, 1.0f, 1.0f) - src;
	// 或使用下面的两句代码，效果一样
	//cv::Mat inverse;
	//cv::subtract(1.0f, src, inverse);
	
	cv::min(src, inverse, dst);
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
	over_exposure(colorImage, resultImage);
	resultImage.convertTo(resultImage, CV_8UC3, 255.0);
	cv::imshow("Result image", resultImage);

	cv::waitKey();
	return 0;
}
