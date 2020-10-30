#include<opencv2/opencv.hpp>
#include"button.h"
using namespace std;
using namespace placeholders;

//const cv::Vec3b z::Widget::background_color_ = cv::Vec3b{200, 200, 200};

z::Button::Button(string text, cv::Rect_<int> r) : z::Widget{r}
{
	text_ = text;
	mat_ = base_color;
	//mat_(cv::Rect2i{4, 4, this->width - 8, this->height - 8}) = base_color;
	label();
	gui_callback_[EVENT_ENTER] = bind(&Button::enter, this, _1, _2);
	gui_callback_[EVENT_LEAVE] = bind(&Button::leave, this, _1, _2);
	gui_callback_[cv::EVENT_LBUTTONDOWN] = bind(&z::Button::ldown, this, _1, _2);
	gui_callback_[cv::EVENT_LBUTTONUP] = bind(&z::Button::lup, this, _1, _2);
}

void z::Button::label()
{
	cv::putText(mat_, text_, {10, 20}, 0, .7, {0,0,0}, 2);
}
void z::Button::leave(int, int)
{
	mat_ = base_color;
	label();
}
void z::Button::enter(int, int)
{
	mat_ = hover_color;
	label();
}
void z::Button::ldown(int, int)
{
	mat_ = click_color;
	label();
}
void z::Button::lup(int x, int y)
{
	mat_ = base_color;
	label();
}
void z::Button::click(function<void()> f) 
{
	user_callback_[cv::EVENT_LBUTTONUP] = [f](int, int) {f();};
}
