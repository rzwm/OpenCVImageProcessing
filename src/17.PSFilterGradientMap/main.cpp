// http://blog.csdn.net/matrix_space/article/details/40477439
// PS滤镜：渐变映射
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

void gradient_mapping(const cv::Mat& src, cv::Mat& dst)
{
	CV_Assert(src.type() == CV_32FC3);

	// build the mapping table
	cv::Mat map(src.size(), src.type());
	const int h = src.rows; // 注意此处与原文的不同
	const float h2 = h / 2.0f;
	for (int i = 0; i < h; ++i)
	{
		float val = 1.0f - std::abs(i / h2 - 1.0f);
		map.row(i).setTo(cv::Scalar(val, val, val));
	}

	cv::multiply(src, map, dst);
}

int main()
{
	cv::Mat colorImage = cv::imread("../../dataset/girl.jpg");
	if (colorImage.empty())
	{
		return -1;
	}

	cv::imshow("Color image", colorImage);
	
	colorImage.convertTo(colorImage, CV_32FC3, 1.0 / 255.0);

	cv::Mat resultImage;
	gradient_mapping(colorImage, resultImage);
	resultImage.convertTo(resultImage, CV_8UC3, 255.0);
	cv::imshow("Result image", resultImage);

	cv::waitKey();
	return 0;
}
