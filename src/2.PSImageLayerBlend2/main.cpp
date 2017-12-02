// http://blog.csdn.net/matrix_space/article/details/22425209
// http://blog.csdn.net/matrix_space/article/details/37760609
// PS图层混合算法（二）：线性加深、线性减淡、变亮、变暗
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

// 线性加深
void linear_burn(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst)
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
				d[i] = s1[i] + s2[i] - 1.0f;
				d[i] = std::max(d[i], 0.0f);
			}
		}
	}
}

// 线性减淡
void linear_dodge(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst)
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
				d[i] = s1[i] + s2[i];
				d[i] = std::min(d[i], 1.0f);
			}
		}
	}
}

// 变亮
void lighten(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst)
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
				if (s2[i] <= s1[i])
				{
					d[i] = s1[i];
				}
				else
				{
					d[i] = s2[i];
				}
			}
		}
	}
}

// 变暗
void darken(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst)
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
				if (s2[i] <= s1[i])
				{
					d[i] = s2[i];
				}
				else
				{
					d[i] = s1[i];
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

	cv::Mat linearBurnImage;
	linear_burn(imageUp, imageDown, linearBurnImage);
	linearBurnImage.convertTo(linearBurnImage, CV_8UC3, 255.0);
	cv::imshow("Linear burn image", linearBurnImage);

	cv::Mat linearDodgeImage;
	linear_dodge(imageUp, imageDown, linearDodgeImage);
	linearDodgeImage.convertTo(linearDodgeImage, CV_8UC3, 255.0);
	cv::imshow("Linear dodge image", linearDodgeImage);

	cv::Mat lightenImage;
	lighten(imageUp, imageDown, lightenImage);
	lightenImage.convertTo(lightenImage, CV_8UC3, 255.0);
	cv::imshow("Lighten image", lightenImage);

	cv::Mat darkenImage;
	darken(imageUp, imageDown, darkenImage);
	darkenImage.convertTo(darkenImage, CV_8UC3, 255.0);
	cv::imshow("Darken image", darkenImage);

	cv::waitKey();
	return 0;
}
