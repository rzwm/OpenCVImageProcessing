// http://blog.csdn.net/matrix_space/article/details/22426633
// http://blog.csdn.net/matrix_space/article/details/37913937
// PS图层混合算法（三）：滤色、叠加、柔光、强光
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

// 滤色
void screen(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst)
{
	CV_Assert(src1.size() == src2.size());
	CV_Assert(src1.type() == CV_32FC3 && src2.type() == CV_32FC3);

	dst.create(src1.size(), src1.type());

	const int h = src1.rows;
	const int w = src1.cols;
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
				d[i] = 1.0f - (1.0f - s1[i]) * (1.0f - s2[i]);
			}
		}
	}
}

// 叠加
void add_color(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst)
{
	CV_Assert(src1.size() == src2.size());
	CV_Assert(src1.type() == CV_32FC3 && src2.type() == CV_32FC3);

	dst.create(src1.size(), src1.type());

	const int h = src1.rows;
	const int w = src1.cols;
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
				if (s2[i] <= 0.5f)
				{
					d[i] = 2.0f * s1[i] * s2[i];
				}
				else
				{
					d[i] = 1.0f - 2.0f * (1.0f - s1[i]) * (1.0f - s2[i]);
				}
			}
		}
	}
}

// 柔光
void soft_lighten(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst)
{
	CV_Assert(src1.size() == src2.size());
	CV_Assert(src1.type() == CV_32FC3 && src2.type() == CV_32FC3);

	dst.create(src1.size(), src1.type());

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
				if (s1[i] <= 0.5f)
				{
					d[i] = (2 * s1[i] - 1.0f) * (s2[i] - s2[i] * s2[i]) + s2[i];
				}
				else
				{
					d[i] = (2 * s1[i] - 1.0f) * (std::sqrt(s2[i]) - s2[i]) + s2[i];
					d[i] = std::min(d[i], 1.0f);
				}
			}
		}
	}
}

// 强光
void strong_lighten(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst)
{
	CV_Assert(src1.size() == src2.size());
	CV_Assert(src1.type() == CV_32FC3 && src2.type() == CV_32FC3);

	dst.create(src1.size(), src1.type());

	const int h = src1.rows;
	const int w = src1.cols;
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
				if (s1[i] <= 0.5f)
				{
					d[i] = 2.0f * s1[i] * s2[i];
				}
				else
				{
					d[i] = 1.0f - 2.0f * (1.0f - s1[i]) * (1.0f - s2[i]);
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

	cv::Mat screenImage;
	screen(imageUp, imageDown, screenImage);
	screenImage.convertTo(screenImage, CV_8UC3, 255.0);
	cv::imshow("Screen image", screenImage);

	cv::Mat addColorImage;
	add_color(imageUp, imageDown, addColorImage);
	addColorImage.convertTo(addColorImage, CV_8UC3, 255.0);
	cv::imshow("Add color image", addColorImage);

	cv::Mat softLigtenImage;
	soft_lighten(imageUp, imageDown, softLigtenImage);
	softLigtenImage.convertTo(softLigtenImage, CV_8UC3, 255.0);
	cv::imshow("Soft lighten image", softLigtenImage);

	cv::Mat strongLightenImage;
	strong_lighten(imageUp, imageDown, strongLightenImage);
	strongLightenImage.convertTo(strongLightenImage, CV_8UC3, 255.0);
	cv::imshow("Strong lighten image", strongLightenImage);

	cv::waitKey();
	return 0;
}
