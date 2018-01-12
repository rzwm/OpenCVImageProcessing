// http://blog.csdn.net/yangtrees/article/details/9103935
// http://blog.csdn.net/matrix_space/article/details/40896455
// 径向模糊
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

void radial_blur(const cv::Mat& src, cv::Mat& dst)
{
	CV_Assert(src.type() == CV_32FC3);

	dst.create(src.size(), src.type());

	const int h = src.rows;
	const int w = src.cols;

	float R = 0.0f;
	float angle = 0.0f;
	cv::Point center(w / 2, h / 2);
	float t1 = 0.0f;
	float t2 = 0.0f;
	float t3 = 0.0f;
	int new_x = 0;
	int new_y = 0;
	const int Num = 20;

	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			t1 = 0.0f;
			t2 = 0.0f;
			t3 = 0.0f;
			R = (float)std::sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x));
			angle = std::atan2((float)(y - center.y), (float)(x - center.x));
			for (int mm = 0; mm < Num; ++mm)
			{
				float tmR = R - mm > 0.0f ? R - mm : 0.0f;
				new_x = (int)(tmR * std::cos(angle)) + center.x;
				new_y = (int)(tmR * std::sin(angle)) + center.y;

				if (new_x < 0)
					new_x = 0;
				if (new_x > w - 1)
					new_x = w - 1;
				if (new_y < 0)
					new_y = 0;
				if (new_y > h - 1)
					new_y = h - 1;

				t1 = t1 + src.at<cv::Vec3f>(new_y, new_x)[0];
				t2 = t2 + src.at<cv::Vec3f>(new_y, new_x)[1];
				t3 = t3 + src.at<cv::Vec3f>(new_y, new_x)[2];
			}

			dst.at<cv::Vec3f>(y, x)[0] = t1 / Num;
			dst.at<cv::Vec3f>(y, x)[1] = t2 / Num;
			dst.at<cv::Vec3f>(y, x)[2] = t3 / Num;
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
	radial_blur(colorImage, resultImage);
	resultImage.convertTo(resultImage, CV_8UC3);
	cv::imshow("Result image", resultImage);

	cv::waitKey();
	return 0;
}
