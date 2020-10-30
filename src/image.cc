#include<opencv2/opencv.hpp>
#include"button.h"
using namespace std;

z::Image::Image(cv::Rect2i r) : Widget{r}
{ }

cv::Mat &z::Image::operator=(const cv::Mat &r)
{
	mat_ = r;
	return mat_;
}
