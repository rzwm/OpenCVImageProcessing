// http://blog.csdn.net/matrix_space/article/details/42361595
// PS滤镜算法之平面坐标到极坐标的变换
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define PI 3.1415926f

void plane_2_polar(const cv::Mat& src, cv::Mat& dst)
{
	CV_Assert(src.type() == CV_32FC3);

	dst.create(src.size(), src.type());

	const int height = src.rows;
	const int width = src.cols;

	float R = height / 2.0f;
	float e = (float)width / (float)height;

	cv::Point center(width / 2, height / 2);

	float R1 = 0.0f;
	float new_x = 0.0f;
	float new_y = 0;
	float p = 0.0f;
	float q = 0.0f;
	float x1 = 0.0f;
	float y1 = 0.0f;
	float theta = 0.0f;

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			theta = std::atan2((float)((center.y - y) * e), (float)(x - center.x));

			if (center.y - y <= 0.0f)
				theta = theta + 2 * PI;

			R1 = (center.y - y) / std::sin(theta);
			new_y = R1 * height / R;
			new_x = theta * width / (2 * PI);

			if (new_x < 0.0f)
				new_x = 0.0f;
			if (new_x > width - 1)
				new_x = (float)(width - 2);
			if (new_y < 0.0f)
				new_y = 0.0f;
			if (new_y > height - 1)
				new_y = (float)(height - 2);

			x1 = new_x;
			y1 = new_y;

			p = new_x - x1;
			q = new_y - y1;

			for (int k = 0; k < 3; ++k)
			{
				dst.at<cv::Vec3f>(y, x)[k] =
					(1.0f - p) * (1.0f - q) * src.at<cv::Vec3f>((int)y1, (int)x1)[k] +
					(p) * (1.0f - 1) * src.at<cv::Vec3f>((int)y1, (int)x1 + 1)[k] +
					(1.0f - p) * (q)* src.at<cv::Vec3f>((int)y1 + 1, (int)x1)[k] +
					(p) * (q)* src.at<cv::Vec3f>((int)y1 + 1, (int)x1 + 1)[k];
			}
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
	plane_2_polar(colorImage, resultImage);
	resultImage.convertTo(resultImage, CV_8UC3);
	cv::imshow("Result image", resultImage);

	cv::waitKey();
	return 0;
}
