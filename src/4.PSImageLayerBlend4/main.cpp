// http://blog.csdn.net/matrix_space/article/details/22427285
// http://blog.csdn.net/matrix_space/article/details/37914011
// PS图层混合算法（四）：亮光、点光、线性光、实色混合
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

// 亮光
void vivid_ligten(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst)
{
	CV_Assert(src1.size() == src2.size());
	CV_Assert(src1.type() == CV_32FC3 && src2.type() == CV_32FC3);

	dst.create(src1.size(), src1.type());
	const float threshold = 1e-6f;

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
					if (2.0f * s1[i] > threshold)
					{
						d[i] = 1.0f - (1.0f - s2[i]) / (2.0f * s1[i]);
						d[i] = std::max(d[i], 0.0f);
					}
					else
					{
						d[i] = 0.0f;
					}
				}
				else
				{
					if (1.0f - s1[i] > threshold)
					{
						d[i] = s2[i] / (2.0f * (1.0f - s1[i]));
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
}

// 点光
void pin_lighten(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst)
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
				if (s1[i] > 0.5f)
				{
					d[i] = std::max(2.0f * (s1[i] - 0.5f), s2[i]);
				}
				else
				{
					d[i] = std::min(2.0f * s1[i], s2[i]);
				}
			}
		}
	}
}

// 线性光
void linear_lighten(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst)
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
				d[i] = s2[i] + 2.0f * s1[i] - 1.0f;
				d[i] = std::max(d[i], 0.0f);
				d[i] = std::min(d[i], 1.0f);
			}
		}
	}
}

// 实色混合
void hard_mix(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst)
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
				if (s1[i] <= 1.0f - s2[i])
				{
					d[i] = 0.0f;
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

	cv::Mat vividLightenImage;
	vivid_ligten(imageUp, imageDown, vividLightenImage);
	vividLightenImage.convertTo(vividLightenImage, CV_8UC3, 255.0);
	cv::imshow("Vivid lighten image", vividLightenImage);

	cv::Mat pinLightenImage;
	pin_lighten(imageUp, imageDown, pinLightenImage);
	pinLightenImage.convertTo(pinLightenImage, CV_8UC3, 255.0);
	cv::imshow("Pin lighten image", pinLightenImage);

	cv::Mat linearLightenImage;
	linear_lighten(imageUp, imageDown, linearLightenImage);
	linearLightenImage.convertTo(linearLightenImage, CV_8UC3, 255.0);
	cv::imshow("Linear lighten image", linearLightenImage);

	cv::Mat hardMixImage;
	hard_mix(imageUp, imageDown, hardMixImage);
	hardMixImage.convertTo(hardMixImage, CV_8UC3, 255.0);
	cv::imshow("Hard mix image", hardMixImage);

	cv::waitKey();
	return 0;
}
