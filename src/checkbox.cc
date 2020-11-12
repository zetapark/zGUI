#include<opencv2/opencv.hpp>
#include"button.h"
using namespace std;
using namespace placeholders;

z::CheckBox::CheckBox(cv::Rect2i r) : z::Widget{r}
{
	r_ = min(width, height) / 2;
	cv::circle(mat_, {width/2, height/2}, r_, widget_color_, -1);
	cv::circle(mat_, {width/2, height/2}, r_ / 2 , background_color_, -1);
	gui_callback_[cv::EVENT_LBUTTONUP] = bind(&CheckBox::click, this, _1, _2);
}

void z::CheckBox::click(int, int) 
{
	cv::circle(mat_, {width/2, height/2}, r_ / 2 , checked_ ? background_color_ : cv::Scalar{0, 255, 0}, -1);
	checked_ = !checked_;
}

bool z::CheckBox::checked()
{
	return checked_;
}

void z::CheckBox::checked(bool t)
{
	checked_ = t;
	cv::circle(mat_, {width/2, height/2}, r_ / 2 , checked_ ? cv::Scalar{0, 255, 0} : background_color_, -1);
}

void z::CheckBox::on_change(function<void(bool)> f)
{
	user_callback_[cv::EVENT_LBUTTONUP] = [this, f](int,int) { f(checked()); };
}

