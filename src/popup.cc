#include<opencv2/opencv.hpp>
#include"button.h"
using namespace std;

z::Popup::Popup(string title, cv::Rect2i r, string content) : z::Window{title, r}
{
	title_ = title;
	*this + yes_ + no_;
	cv::putText(mat_, content, {10, 30}, 0, 0.7, {0, 0, 0});
	yes_.click(bind(&Popup::click_yes, this));
	no_.click(bind(&Popup::click_no, this));
}

bool z::Popup::open()
{
	show();
	while(!closed_) cv::waitKey(10);
	closed_ = false;
	return result_;
}

void z::Popup::click_yes()
{
	closed_ = first_ = result_ = true;
	quit();
}

void z::Popup::click_no()
{
	result_ = false; 
	closed_ = first_ = true;//closed first to avoid mutex
	quit();
}

