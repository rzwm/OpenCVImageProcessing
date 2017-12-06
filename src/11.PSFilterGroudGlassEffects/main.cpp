// http://blog.csdn.net/matrix_space/article/details/40271843
// PS滤镜：毛玻璃特效
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

// 毛玻璃特效
void ground_glass_effects(const cv::Mat& src, cv::Mat& dst)
{
	CV_Assert(src.type() == CV_32FC3);

	dst.create(src.size(), src.type());

	cv::Mat blur;
	cv::GaussianBlur(src, blur, cv::Size(5, 5), 0.0);

	const int P_size = 3;
	const int h = src.rows;
	const int w = src.cols;

	srand((unsigned int)cv::getTickCount());

	for (int y = P_size; y < h - P_size; ++y)
	{
		for (int x = P_size; x < w - P_size; ++x)
		{
			float k1 = (rand() % 100) / 100.0f - 0.5f;
			float k2 = (rand() % 100) / 100.0f - 0.5f;

			float m = k1 * (P_size * 2 - 1);
			float n = k2 * (P_size * 2 - 1);

			int row = static_cast<int>(y + m) % h;
			int col = static_cast<int>(x + n) % w;

			dst.at<cv::Vec3f>(y, x) = blur.at<cv::Vec3f>(row, col);
		}
	}
}

// 毛玻璃特效-无黑边
void ground_glass_effects_no_border(const cv::Mat& src, cv::Mat& dst)
{
	CV_Assert(src.type() == CV_32FC3);

	dst.create(src.size(), src.type());

	const int P_size = 3;

	cv::Mat extend;
	cv::copyMakeBorder(src, extend, P_size, P_size, P_size, P_size, cv::BORDER_DEFAULT);

	cv::Mat blur;
	cv::GaussianBlur(extend, blur, cv::Size(5, 5), 0.0);

	const int h = src.rows;
	const int w = src.cols;
	const int eh = extend.rows;
	const int ew = extend.cols;

	srand((unsigned int)cv::getTickCount());

	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			float k1 = (rand() % 100) / 100.0f - 0.5f;
			float k2 = (rand() % 100) / 100.0f - 0.5f;

			float m = k1 * (P_size * 2 - 1);
			float n = k2 * (P_size * 2 - 1);

			int row = static_cast<int>(y + m) % eh;
			int col = static_cast<int>(x + n) % ew;

			dst.at<cv::Vec3f>(y, x) = blur.at<cv::Vec3f>(row + P_size, col + P_size);
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
	ground_glass_effects(colorImage, resultImage);
	resultImage.convertTo(resultImage, CV_8UC3, 255.0);
	cv::imshow("Result image", resultImage);

	cv::Mat noBorderImage;
	ground_glass_effects_no_border(colorImage, noBorderImage);
	noBorderImage.convertTo(noBorderImage, CV_8UC3, 255.0);
	cv::imshow("Result no border image", noBorderImage);

	cv::waitKey();
	return 0;
}
