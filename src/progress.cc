#include<opencv2/opencv.hpp>
#include"button.h"
using namespace std;

z::Progress::Progress(cv::Rect2i r) : z::Widget{r}
{
	cv::rectangle(mat_, {0, height / 3, width, height / 3}, click_color_, -1);
}

void z::Progress::value(int val)
{
	value_ = val;
	cv::rectangle(mat_, {0, height / 3, width, height / 3}, click_color_, -1);
	cv::rectangle(mat_, {0, height / 3, val * width / 100 , height / 3}, widget_color_, -1);
}

int z::Progress::value()
{
	return value_;
}
