// http://www.linuxidc.com/Linux/2012-09/70006.htm
// http://blog.csdn.net/matrix_space/article/details/40893795
// 生成关于某点的颜色径向均匀渐变图像
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

void color_uniform_gradient(int width, int height, cv::Mat& dst)
{
	CV_Assert(width > 0 && height > 0);

	dst.create(cv::Size(width, height), CV_32FC3);

	cv::Scalar a(255.0, 255.0, 255.0);
	cv::Scalar b(0.0, 0.0, 0.0);

	dst.setTo(a);

	cv::Point2f origin(0.0f, 0.0f);
	cv::Point2f center(width / 2.0f, height / 2.0f);

	float distance = 0.0f;

	if (origin.x <= center.x && origin.y <= center.y)
	{
		distance = std::sqrt((width - 1 - origin.x) * (width - 1 - origin.x) 
			+ (height - 1 - origin.y) * (height - 1 - origin.y));
	}
	else if (origin.x <= center.x && origin.y > center.y)
	{
		distance = std::sqrt((width - 1 - origin.x) * (width - 1 - origin.x)
			+ origin.y * origin.y);
	}
	else if (origin.x > center.x && origin.y > center.y)
	{
		distance = std::sqrt(origin.x * origin.x + origin.y * origin.y);
	}
	else
	{
		distance = std::sqrt(origin.x * origin.x 
			+ (height - 1 - origin.y) * (height - 1 - origin.y));
	}

	float weightB = (float)(b[0] - a[0]) / distance;
	float weightG = (float)(b[1] - a[1]) / distance;
	float weightR = (float)(b[2] - a[2]) / distance;

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			float dis = std::sqrt((y - origin.y) * (y - origin.y)
				+ (x - origin.x) * (x - origin.x));
			dst.at<cv::Vec3f>(y, x)[0] = dst.at<cv::Vec3f>(y, x)[0] + weightB * dis;
			dst.at<cv::Vec3f>(y, x)[1] = dst.at<cv::Vec3f>(y, x)[1] + weightG * dis;
			dst.at<cv::Vec3f>(y, x)[2] = dst.at<cv::Vec3f>(y, x)[2] + weightR * dis;
		}
	}
}

int main()
{
	cv::Mat resultImage;
	color_uniform_gradient(800, 600, resultImage);
	resultImage.convertTo(resultImage, CV_8UC3);
	cv::imshow("Result image", resultImage);

	cv::waitKey();
	return 0;
}
