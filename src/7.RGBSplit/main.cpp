// http://blog.csdn.net/matrix_space/article/details/39896567
// RGB三通道分离
#include <vector>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

int main()
{
	cv::Mat colorImage = cv::imread("../../dataset/flower.jpg");
	if (colorImage.empty())
	{
		return -1;
	}

	cv::imshow("Color image", colorImage);

	std::vector<cv::Mat> vecBGR;
	cv::split(colorImage, vecBGR);
	cv::Mat R = vecBGR[2];
	cv::Mat G = vecBGR[1];
	cv::Mat B = vecBGR[0];
	cv::imshow("R channel", R);
	cv::imshow("G channel", G);
	cv::imshow("B channel", B);

	cv::waitKey();
	return 0;
}
