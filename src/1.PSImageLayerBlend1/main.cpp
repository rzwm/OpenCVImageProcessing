// http://blog.csdn.net/matrix_space/article/details/22416241
// http://blog.csdn.net/matrix_space/article/details/37696015
// PS图层混合算法（一）：不透明度、正片叠底、颜色加深、颜色减淡
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

// 不透明度
void transparent(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst, float alpha)
{
	CV_Assert(src1.size() == src2.size());
	CV_Assert(src1.type() == CV_32FC3 && src2.type() == CV_32FC3);
	CV_Assert(alpha >= 0.0f && alpha <= 1.0f);

	dst = alpha * src1 + (1.0f - alpha) * src2;
}

// 正片叠底
void multiply(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst)
{
	CV_Assert(src1.size() == src2.size());
	CV_Assert(src1.type() == CV_32FC3 && src2.type() == CV_32FC3);

	cv::multiply(src1, src2, dst);
}

// 颜色加深
void color_burn(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst)
{
	CV_Assert(src1.size() == src2.size());
	CV_Assert(src1.type() == CV_32FC3 && src2.type() == CV_32FC3);

	dst.create(src1.size(), src1.type());
	const float threshold = 1e-6f;

	const int h = src1.rows;
	const int w = src2.cols;
	for (int y = 0; y < h; ++y)
	{
		cv::Vec3f* dstData = dst.ptr<cv::Vec3f>(y);
		const cv::Vec3f* src1Data = src1.ptr<cv::Vec3f>(y);
		const cv::Vec3f* src2Data = src2.ptr<cv::Vec3f>(y);
		for (int x = 0; x < w; ++x)
		{
			cv::Vec3f& d = dstData[x];
			const cv::Vec3f& s1 = src1Data[x];
			const cv::Vec3f& s2 = src2Data[x];
			for (int i = 0; i < 3; ++i)
			{
				if (s1[i] > threshold)
				{
					d[i] = 1.0f - (1.0f - s2[i]) / s1[i];
					d[i] = std::max(d[i], 0.0f);
				}
				else
				{
					d[i] = 0.0f;
				}
			}
		}
	}
}

// 颜色减淡
void color_dodge(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst)
{
	CV_Assert(src1.size() == src2.size());
	CV_Assert(src1.type() == CV_32FC3 && src2.type() == CV_32FC3);

	dst.create(src1.size(), src1.type());
	const float threshold = 1.0f - 1e-6f;

	const int h = src1.rows;
	const int w = src2.cols;
	for (int y = 0; y < h; ++y)
	{
		cv::Vec3f* dstData = dst.ptr<cv::Vec3f>(y);
		const cv::Vec3f* src1Data = src1.ptr<cv::Vec3f>(y);
		const cv::Vec3f* src2Data = src2.ptr<cv::Vec3f>(y);
		for (int x = 0; x < w; ++x)
		{
			cv::Vec3f& d = dstData[x];
			const cv::Vec3f& s1 = src1Data[x];
			const cv::Vec3f& s2 = src2Data[x];
			for (int i = 0; i < 3; ++i)
			{
				if (s1[i] < threshold)
				{
					d[i] = s2[i] / (1.0f - s1[i]);
					d[i] = std::min(d[i], 1.0f);
				}
				else
				{
					d[i] = 1.0f;
				}
			}
		}
	}
}

int main()
{
	cv::Mat imageUp = cv::imread("../../dataset/trees.png");
	cv::Mat imageDown = cv::imread("../../dataset/mountains.png");
	if (imageUp.empty() || imageDown.empty()
		|| imageUp.size() != imageDown.size())
	{
		return -1;
	}

	cv::imshow("Original up image", imageUp);
	cv::imshow("Original down image", imageDown);

	imageUp.convertTo(imageUp, CV_32FC3, 1.0 / 255.0);
	imageDown.convertTo(imageDown, CV_32FC3, 1.0 / 255.0);

	cv::Mat transparentImage;
	transparent(imageUp, imageDown, transparentImage, 0.5f);
	transparentImage.convertTo(transparentImage, CV_8UC3, 255.0);
	cv::imshow("Transparent image", transparentImage);

	cv::Mat multiplyImage;
	multiply(imageUp, imageDown, multiplyImage);
	multiplyImage.convertTo(multiplyImage, CV_8UC3, 255.0);
	cv::imshow("Multiply image", multiplyImage);

	cv::Mat colorBurnImage;
	color_burn(imageUp, imageDown, colorBurnImage);
	colorBurnImage.convertTo(colorBurnImage, CV_8UC3, 255.0);
	cv::imshow("Color burn image", colorBurnImage);

	cv::Mat colorDodgeImage;
	color_dodge(imageUp, imageDown, colorDodgeImage);
	colorDodgeImage.convertTo(colorDodgeImage, CV_8UC3, 255.0);
	cv::imshow("Color dodge image", colorDodgeImage);

	cv::waitKey();
	return 0;
}
