// http://blog.csdn.net/matrix_space/article/details/40897935
// 旋转模糊（二）
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define PI 3.1415926f

void rotate_blur(const cv::Mat& src, cv::Mat& dst)
{
	CV_Assert(src.type() == CV_32FC3);

	dst.create(src.size(), src.type());

	const int height = src.rows;
	const int width = src.cols;

	int n_point = 1;
	float angle = 30.0f;
	float w = angle * PI / 180.0f;
	float w_2 = w * w;
	int num = 3;
	int num2 = num * num;

	cv::Point center(width / 2, height / 2);
	float t1 = 0.0f;
	float t2 = 0.0f;
	float t3 = 0.0f;
	int x1 = 0;
	int y1 = 0;
	int x2 = 0;
	int y2 = 0;
	int new_x = 0;
	int new_y = 0;

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			n_point = 1;
			x1 = x - center.x;
			y1 = center.y - y;

			t1 = src.at<cv::Vec3f>(y, x)[0];
			t2 = src.at<cv::Vec3f>(y, x)[1];
			t3 = src.at<cv::Vec3f>(y, x)[2];

			x2 = x1;
			y2 = y1;

			for (int mm = 0; mm < num; ++mm)
			{
				x1 = x2;
				y1 = y2;

				// anticlockwise
				x2 = static_cast<int>(x1 - w * y1 / num - w_2 * x1 / num2);
				y2 = static_cast<int>(y1 + w * x1 / num - w_2 * y1 / num2);

				new_x = x2 + center.x;
				new_y = center.y - y2;

				if (new_x > 0 && new_x < width - 1 && new_y > 0 && new_y < height - 1)
				{
					t1 = t1 + src.at<cv::Vec3f>(new_y, new_x)[0];
					t2 = t2 + src.at<cv::Vec3f>(new_y, new_x)[1];
					t3 = t3 + src.at<cv::Vec3f>(new_y, new_x)[2];
					++n_point;
				}
			}

			dst.at<cv::Vec3f>(y, x)[0] = t1 / n_point;
			dst.at<cv::Vec3f>(y, x)[1] = t2 / n_point;
			dst.at<cv::Vec3f>(y, x)[2] = t3 / n_point;
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
	rotate_blur(colorImage, resultImage);
	resultImage.convertTo(resultImage, CV_8UC3);
	cv::imshow("Result image", resultImage);

	cv::waitKey();
	return 0;
}
