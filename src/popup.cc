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

int z::Popup::open()
{
	start();
	while(!closed_) cv::waitKey(10);
	closed_ = false;
	return result_;
}

void z::Popup::click_yes()
{
	result_ = 1;
	closed_ = true;
	quit();
}

void z::Popup::click_no()
{
	result_ = 0; 
	closed_ = true;//closed first to avoid mutex
	quit();
}

