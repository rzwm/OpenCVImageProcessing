// http://blog.csdn.net/maozefa/article/details/8302198
// http://blog.csdn.net/matrix_space/article/details/40681919
// 黑白调整
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

void adjust_black_and_white(const cv::Mat& src, cv::Mat& dst)
{
	CV_Assert(src.type() == CV_32FC3);

	dst.create(src.size(), CV_32FC1);

	float color_ratio[6] = { 0.4f, 0.6f, 0.4f, 0.6f, 0.2f, 0.8f };
	float ratio_max_mid = 0.0f;
	float ratio_max = 0.0f;
	int index = 0;

	const int pixelCount = src.cols * src.rows; 
	const cv::Vec3f* srcData = src.ptr<cv::Vec3f>(0);
	float* dstData = dst.ptr<float>(0);
	for (int i = 0; i < pixelCount; ++i)
	{
		const float b = srcData[i][0];
		const float g = srcData[i][1];
		const float r = srcData[i][2];

		float minVal = std::min(std::min(b, g), r);
		float maxVal = std::max(std::max(b, g), r);
		float midVal = b + g + r - minVal - maxVal;

		const float eps = 1e-6f;
		if (std::abs(minVal - r) < eps)
		{
			index = 0;
		}
		else if (std::abs(minVal - g) < eps)
		{
			index = 2;
		}
		else
		{
			index = 4;
		}
		ratio_max_mid = color_ratio[2 == index ? 1 : (index + 3) % 6 + 1]; // 注意这里原文的索引会越界

		if (std::abs(maxVal - r) < eps)
		{
			index = 1;
		}
		else if (std::abs(maxVal - g) < eps)
		{
			index = 3;
		}
		else
		{
			index = 5;
		}
		ratio_max = color_ratio[index];

		dstData[i] = (maxVal - midVal) * ratio_max + (midVal - minVal) * ratio_max_mid + minVal;
		if (dstData[i] < eps)
		{
			int n = 0;
		}
	}
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
	adjust_black_and_white(colorImage, resultImage);
	resultImage.convertTo(resultImage, CV_8UC1);
	cv::imshow("Result image", resultImage);

	cv::waitKey();
	return 0;
}
