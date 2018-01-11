// http://blog.csdn.net/matrix_space/article/details/38687609
// http://blog.csdn.net/matrix_space/article/details/40623017
// PS图像特效：非线性滤波器
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

void nolinear_filter(const cv::Mat& src, cv::Mat& dst)
{
	CV_Assert(src.type() == CV_32FC3);

	dst = src.clone();

	cv::Mat B;
	cv::Mat G;
	cv::Mat R;
	cv::Mat bgr[] = { B, G, R };
	cv::split(src, bgr);

	const int P_size = 3;
	cv::Mat subMat;
	double minVal;

	for (int y = P_size; y < src.rows - P_size - 1; ++y)
	{
		for (int x = P_size; x < src.cols - P_size - 1; ++x)
		{
			for (int k = 0; k < 3; ++k)
			{
				subMat = bgr[k](cv::Range(y - P_size, y + P_size), cv::Range(x - P_size, x + P_size));
				cv::minMaxLoc(subMat, &minVal);
				dst.at<cv::Vec3f>(y, x)[k] = static_cast<float>(minVal);
			}
		}
	}
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
	nolinear_filter(colorImage, resultImage);
	resultImage.convertTo(resultImage, CV_8UC3, 255.0);
	cv::imshow("Result image", resultImage);

	cv::waitKey();
	return 0;
}
