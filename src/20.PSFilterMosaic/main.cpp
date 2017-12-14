// http://blog.csdn.net/matrix_space/article/details/30469217
// http://blog.csdn.net/matrix_space/article/details/40622159
// PS滤镜：马赛克
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

void mosaic_effects(const cv::Mat& src, cv::Mat& dst)
{
	CV_Assert(src.type() == CV_32FC3);

	dst = src.clone();

	const int P_size = 9;
	float k1 = 0.0f;
	float k2 = 0.0f;
	float m = 0.0f;
	float n = 0.0f;
	int row = 0;
	int col = 0;
	
	const int h = src.rows;
	const int w = src.cols;

	cv::Mat region;
	srand((unsigned int)cv::getTickCount());
	for (int i = P_size; i < h - P_size - 1; i += P_size)
	{
		for (int j = P_size; j < w - P_size - 1; j += P_size)
		{
			k1 = (rand() % 100) / 100.0f - 0.5f;
			k2 = (rand() % 100) / 100.0f - 0.5f;

			m = (k1 * (P_size * 2.0f - 1.0f));
			n = (k2 * (P_size * 2.0f - 1.0f));

			row = (int)(i + m) % h;
			col = (int)(j + n) % w;

			region = dst(cv::Rect(j - P_size, i - P_size, P_size * 2, P_size * 2));
			region.setTo(cv::Scalar(src.at<cv::Vec3f>(row, col)));
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

	colorImage.convertTo(colorImage, CV_32FC3, 1.0 / 255.0);

	cv::Mat resultImage;
	mosaic_effects(colorImage, resultImage);
	resultImage.convertTo(resultImage, CV_8UC3, 255.0);
	cv::imshow("Result image", resultImage);

	cv::waitKey();
	return 0;
}
