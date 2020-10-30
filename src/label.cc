#include<opencv2/opencv.hpp>
#include"button.h"
using namespace std;

z::Label::Label(string s, cv::Rect2i r) : z::Widget{r}
{
	text(s);
}

string z::Label::text()
{
	return text_;
}

void z::Label::text(string s)
{
	text_ = s;
	mat_ = background_color_;
	cv::putText(mat_, text_, {0, height-10}, 0, 0.7, {0, 0, 0});
}
