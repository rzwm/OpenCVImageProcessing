// http://blog.csdn.net/matrix_space/article/details/38687427
// http://blog.csdn.net/matrix_space/article/details/40589411
// PS滤镜：素描特效
#include <vector>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

void sketch_effects(const cv::Mat& src, cv::Mat& dst)
{
	CV_Assert(src.type() == CV_32FC3);

	cv::Mat gray;
	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

	cv::Mat invert;
	cv::subtract(cv::Scalar(1.0), gray, invert);

	cv::Mat gauss;
	cv::GaussianBlur(invert, gauss, cv::Size(25, 25), 0.0);

	const float delta = 0.01f;
	gauss = -gauss + 1.0f + delta;

	cv::Mat temp;
	cv::divide(gray, gauss, temp);

	const float alpha = 0.75f;
	cv::Mat B;
	cv::Mat G;
	cv::Mat R;

	B = alpha * temp + (1.0f - alpha) * 105.0f / 255.0f;
	G = alpha * temp + (1.0f - alpha) * 205.0f / 255.0f;
	R = alpha * temp + (1.0f - alpha) * 200.0f / 255.0f;

	std::vector<cv::Mat> vecDstBGR;
	vecDstBGR.push_back(B);
	vecDstBGR.push_back(G);
	vecDstBGR.push_back(R);
	cv::merge(vecDstBGR, dst);
}

int main()
{
	cv::Mat colorImage = cv::imread("../../dataset/flower.jpg");
	if (colorImage.empty())
	{
		return -1;
	}

	cv::imshow("Color image", colorImage);

	colorImage.convertTo(colorImage, CV_32FC3, 1.0 / 255);

	cv::Mat resultImage;
	sketch_effects(colorImage, resultImage);
	resultImage.convertTo(resultImage, CV_8UC3, 255.0);
	cv::imshow("Result image", resultImage);

	cv::waitKey();
	return 0;
}
