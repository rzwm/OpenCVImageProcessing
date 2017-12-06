// http://blog.csdn.net/matrix_space/article/details/38122673
// 彩色图像转灰度图像
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

// 方案一：I = 0.299 * R + 0.587 * G + 0.144 * B
void rgb_2_gray1(const cv::Mat& color, cv::Mat& gray)
{
	CV_Assert(color.type() == CV_8UC3);

	gray.create(color.size(), CV_8UC1);

	const int h = color.rows;
	const int w = color.cols;
	for (int y = 0; y < h; ++y)
	{
		uchar* grayData = gray.ptr<uchar>(y);
		const cv::Vec3b* colorData = color.ptr<cv::Vec3b>(y);
		for (int x = 0; x < w; ++x)
		{
			const cv::Vec3b& c = colorData[x];
			grayData[x] = cv::saturate_cast<uchar>(
				0.299f * c[2] + 0.587f * c[1] + 0.114f * c[0]);
		}
	}
}

// 方案二：I = (R + G + B) / 3
void rgb_2_gray2(const cv::Mat& color, cv::Mat& gray)
{
	CV_Assert(color.type() == CV_8UC3);

	gray.create(color.size(), CV_8UC1);

	const int h = color.rows;
	const int w = color.cols;
	for (int y = 0; y < h; ++y)
	{
		uchar* grayData = gray.ptr<uchar>(y);
		const cv::Vec3b* colorData = color.ptr<cv::Vec3b>(y);
		for (int x = 0; x < w; ++x)
		{
			const cv::Vec3b& c = colorData[x];
			grayData[x] = cv::saturate_cast<uchar>(
				((float)c[0] + (float)c[1] + (float)c[2]) / 3.0f + 0.5f);
		}
	}
}

// 方案三：I = max(R, G, B)
void rgb_2_gray3(const cv::Mat& color, cv::Mat& gray)
{
	CV_Assert(color.type() == CV_8UC3);

	gray.create(color.size(), CV_8UC1);

	const int h = color.rows;
	const int w = color.cols;
	for (int y = 0; y < h; ++y)
	{
		uchar* grayData = gray.ptr<uchar>(y);
		const cv::Vec3b* colorData = color.ptr<cv::Vec3b>(y);
		for (int x = 0; x < w; ++x)
		{
			const cv::Vec3b& c = colorData[x];
			grayData[x] = std::max(c[0], std::max(c[1], c[2]));
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

	cv::Mat grayImage1;
	rgb_2_gray1(colorImage, grayImage1);
	cv::imshow("Gray Image 1", grayImage1);

	cv::Mat grayImage2;
	rgb_2_gray2(colorImage, grayImage2);
	cv::imshow("Gray image 2", grayImage2);

	cv::Mat grayImage3;
	rgb_2_gray3(colorImage, grayImage3);
	cv::imshow("Gray image 3", grayImage3);

	cv::waitKey();
	return 0;
}
