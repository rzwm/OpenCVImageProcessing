// http://blog.csdn.net/matrix_space/article/details/22488467
// http://blog.csdn.net/matrix_space/article/details/37914085
// PS图层混合算法（六）：差值、溶解、排除
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

// 差值
void difference(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst)
{
	CV_Assert(src1.size() == src2.size());
	CV_Assert(src1.type() == CV_32FC3 && src2.type() == CV_32FC3);

	dst = src1 - src2;
	dst = cv::abs(dst);
}

// 排除
void exclusion(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst)
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
				d[i] = s1[i] + s2[i] - 2.0f * s1[i] * s2[i];
				d[i] = std::min(d[i], 1.0f);
			}
		}
	}
}

// 溶解
void dissolve(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst, float alpha)
{
	CV_Assert(src1.size() == src2.size());
	CV_Assert(src1.type() == CV_32FC3 && src2.type() == CV_32FC3);
	CV_Assert(alpha >= 0.0f && alpha <= 1.0f);

	dst = src1.clone();

	cv::Mat randMat(src1.size(), CV_32FC1);
	cv::randu(randMat, 0.0f, 1.0f);

	const int h = src1.rows;
	const int w = src1.cols;
	for (int y = 0; y < h; ++y)
	{
		cv::Vec3f* dstData = dst.ptr<cv::Vec3f>(y);
		const cv::Vec3f* src2Data = src2.ptr<cv::Vec3f>(y);
		const float* randData = randMat.ptr<float>(y);
		for (int x = 0; x < w; ++x)
		{
			cv::Vec3f& d = dstData[x];
			const cv::Vec3f& s2 = src2Data[x];
			if (randData[x] < alpha)
			{
				for (int i = 0; i < 3; ++i)
				{
					d[i] = s2[i];
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

	cv::Mat differenceImage;
	difference(imageUp, imageDown, differenceImage);
	differenceImage.convertTo(differenceImage, CV_8UC3, 255.0);
	cv::imshow("Difference image", differenceImage);

	cv::Mat exclusionImage;
	exclusion(imageUp, imageDown, exclusionImage);
	exclusionImage.convertTo(exclusionImage, CV_8UC3, 255.0);
	cv::imshow("Exclusion image", exclusionImage);

	cv::Mat dissolveImage;
	dissolve(imageUp, imageDown, dissolveImage, 0.75f);
	dissolveImage.convertTo(dissolveImage, CV_8UC3, 255.0);
	cv::imshow("Dissolve image", dissolveImage);

	cv::waitKey();
	return 0;
}
