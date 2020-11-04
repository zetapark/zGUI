#include<opencv2/opencv.hpp>
#include"button.h"
using namespace std;
using namespace placeholders;

z::CheckBox::CheckBox(cv::Rect2i r) : z::Widget{r}
{
	mat_ = cv::Vec3b{100, 100, 100};
	inner_rect_ = cv::Rect2i{cv::Point2i{5, 5}, cv::Point2i{width-5, height-5}};
	cv::rectangle(mat_, inner_rect_, cv::Scalar{200, 200, 200}, -1);
	gui_callback_[cv::EVENT_LBUTTONUP] = bind(&CheckBox::click, this, _1, _2);
}

void z::CheckBox::click(int, int) 
{
	cv::rectangle(mat_, inner_rect_, checked_ ? cv::Scalar{200, 200, 200} : cv::Scalar{0, 255, 0}, -1);
	checked_ = !checked_;
}

bool z::CheckBox::checked()
{
	return checked_;
}

void z::CheckBox::on_change(function<void(bool)> f)
{
	user_callback_[cv::EVENT_LBUTTONUP] = [this, f](int,int) { f(checked()); };
}

