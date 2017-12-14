// http://blog.csdn.net/matrix_space/article/details/40584537
// 色调映射
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

void tone_mapping(const cv::Mat& src, cv::Mat& dst)
{
	CV_Assert(src.type() == CV_32FC3);

	dst.create(src.size(), src.type());

	const int size = src.rows * src.cols;
	const cv::Vec3f* data = src.ptr<cv::Vec3f>(0);
	cv::Vec3f* dstData = dst.ptr<cv::Vec3f>(0);
	for (int i = 0; i < size; ++i)
	{
		const cv::Vec3f& s = data[i];
		cv::Vec3f& d = dstData[i];
		double val = s[0] * 0.144 + s[1] * 0.587 + s[2] * 0.299;
		//val /= 255.0; // 注意这里与原文的不同
		d[0] = static_cast<float>(3 * val);
		d[1] = static_cast<float>(3 * val - 1.0);
		d[2] = static_cast<float>(3 * val - 2.0);
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
	tone_mapping(colorImage, resultImage);
	resultImage.convertTo(resultImage, CV_8UC3, 255.0);
	cv::imshow("Result image", resultImage);

	cv::waitKey();
	return 0;
}
