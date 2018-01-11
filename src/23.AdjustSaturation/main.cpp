// http://blog.csdn.net/maozefa/article/details/1781208
// http://blog.csdn.net/matrix_space/article/details/40652691
// 饱和度调整
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

// percent :［-1.0, 1.0］
void adjust_saturation(const cv::Mat& src, cv::Mat& dst, float percent)
{
	CV_Assert(src.type() == CV_32FC3);
	CV_Assert(percent >= -1.0f && percent <= 1.0f);

	dst.create(src.size(), src.type());

	const int pixelCount = src.cols * src.rows;
	const cv::Vec3f* srcData = src.ptr<cv::Vec3f>(0);
	cv::Vec3f* dstData = dst.ptr<cv::Vec3f>(0);
	for (int i = 0; i < pixelCount; ++i)
	{
		const float b = srcData[i][0];
		const float g = srcData[i][1];
		const float r = srcData[i][2];

		float minVal = std::min(std::min(b, g), r);
		float maxVal = std::max(std::max(b, g), r);
		float delta = (maxVal - minVal) / 255.0f;
		float L = (maxVal + minVal) * 0.5f / 255.0f;
		float S = std::max(0.5f * delta / L, 0.5f * delta / (1.0f - L));

		if (percent > 0.0f)
		{
			float alpha = std::max(S, 1.0f - percent);
			alpha = 1.0f / alpha - 1.0f;
			dstData[i][0] = b + (b - L * 255.0f) * alpha;
			dstData[i][1] = g + (g - L * 255.0f) * alpha;
			dstData[i][2] = r + (r - L * 255.0f) * alpha;
		}
		else
		{
			float alpha = percent;
			dstData[i][0] = L * 255.0f + (b - L * 255.0f) * (1.0f + alpha);
			dstData[i][1] = L * 255.0f + (g - L * 255.0f) * (1.0f + alpha);
			dstData[i][2] = L * 255.0f + (r - L * 255.0f) * (1.0f + alpha);
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

	colorImage.convertTo(colorImage, CV_32FC3);

	cv::Mat resultImage;
	adjust_saturation(colorImage, resultImage, -0.5f);
	resultImage.convertTo(resultImage, CV_8UC3);
	cv::imshow("Result image", resultImage);

	cv::waitKey();
	return 0;
}
