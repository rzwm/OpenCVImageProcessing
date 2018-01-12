// http://blog.csdn.net/matrix_space/article/details/40897645
// 旋转模糊
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

void rotate_blur(const cv::Mat& src, cv::Mat& dst)
{
	CV_Assert(src.type() == CV_32FC3);

	dst.create(src.size(), src.type());

	const int height = src.rows;
	const int width = src.cols;

	float R = 0.0f;
	float angle = 0.0f;

	cv::Point center(width / 2, height / 2);
	float t1 = 0.0f;
	float t2 = 0.0f;
	float t3 = 0.0f;
	int new_x = 0;
	int new_y = 0;
	int num = 30;

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			t1 = 0.0f;
			t2 = 0.0f;
			t3 = 0.0f;
			R = (float)std::sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x));
			angle = std::atan2((float)(y - center.y), (float)(x - center.x));

			for (int mm = 0; mm < num; ++mm)
			{
				angle = angle + 0.01f;

				new_x = static_cast<int>(R * std::cos(angle) + center.x);
				new_y = static_cast<int>(R * std::sin(angle) + center.y);

				if (new_x < 0)
					new_x = 0;
				if (new_x > width - 1)
					new_x = width - 1;
				if (new_y < 0)
					new_y = 0;
				if (new_y > height - 1)
					new_y = height - 1;

				t1 = t1 + src.at<cv::Vec3f>(new_y, new_x)[0];
				t2 = t2 + src.at<cv::Vec3f>(new_y, new_x)[1];
				t3 = t3 + src.at<cv::Vec3f>(new_y, new_x)[2];
			}

			dst.at<cv::Vec3f>(y, x)[0] = t1 / num;
			dst.at<cv::Vec3f>(y, x)[1] = t2 / num;
			dst.at<cv::Vec3f>(y, x)[2] = t3 / num;
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
