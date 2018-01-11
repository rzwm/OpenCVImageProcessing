// http://blog.csdn.net/matrix_space/article/details/25493345
// http://blog.csdn.net/matrix_space/article/details/40627847
// PS图像特效：百叶窗
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

void window_shutter(const cv::Mat& src, cv::Mat& dst)
{
	CV_Assert(src.type() == CV_32FC3);

	dst.create(src.size(), src.type());

	cv::Mat map(src.size(), src.type());

	double val;
	cv::Mat temp;
	for (int i = 0; i < map.rows; ++i)
	{
		val = i / 255.0;
		temp = map.row(i);
		temp.setTo(cv::Scalar(val, val, val));
	}

	const int H_shade = 8;
	const int Inter = 5;
	int num = map.rows / (H_shade + Inter);

	cv::Mat mask(src.size(), src.type(), cv::Scalar(1.0, 1.0, 1.0));

	int row_begin = 0;

	for (int i = 0; i <= num; ++i)
	{
		if (i < num)
		{
			row_begin = i * (H_shade + Inter);
			temp = mask.rowRange(row_begin, row_begin + H_shade - 1);
			temp.setTo(cv::Scalar(0.0, 0.0, 0.0));
		}
		else
		{
			row_begin = i*(H_shade + Inter);
			temp = mask.rowRange(row_begin, mask.rows - 1);
			temp.setTo(cv::Scalar(0.0, 0.0, 0.0));
		}
	}

	cv::Mat m0;
	cv::Mat m1;
	cv::Mat m2;
	cv::subtract(cv::Scalar(1.0, 1.0, 1.0), mask, m0);
	cv::multiply(src, m0, m1);
	cv::multiply(map, mask, m2);

	dst = m1 + m2;
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
	window_shutter(colorImage, resultImage);
	resultImage.convertTo(resultImage, CV_8UC3, 255.0);
	cv::imshow("Result image", resultImage);

	cv::waitKey();
	return 0;
}
