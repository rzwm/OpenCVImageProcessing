// http://blog.csdn.net/matrix_space/article/details/30283003
// http://blog.csdn.net/matrix_space/article/details/40403411
// PS滤镜算法原理——照亮边缘
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

// 照亮边缘
void lit_up_edges(const cv::Mat& src, cv::Mat& dst)
{
	CV_Assert(src.type() == CV_32FC3);

	const int kernelSize = 3;
	const int p = 3;
	const int q = 0;
	
	const int gx[] = 
	{
		-1, -p, -1,
		 0,  q,  0,
		 1,  p,  1
	};
	const int gy[] =
	{
		-1, 0, 1,
		-p, q, p,
		-1, 0, 1
	};

	cv::Mat gxMat(cv::Size(kernelSize, kernelSize), CV_32SC1, (void*)gx);
	cv::Mat gyMat(cv::Size(kernelSize, kernelSize), CV_32SC1, (void*)gy);

	cv::Mat imageX;
	cv::Mat imageY;
	cv::filter2D(src, imageX, -1, gxMat);
	cv::filter2D(src, imageY, -1, gyMat);

	const float alpha = 0.5f;

	dst = alpha * cv::abs(imageX) + (1.0f - alpha) * cv::abs(imageY);
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
	lit_up_edges(colorImage, resultImage);
	resultImage.convertTo(resultImage, CV_8UC3, 255.0);
	cv::imshow("Result image", resultImage);

	cv::waitKey();
	return 0;
}
