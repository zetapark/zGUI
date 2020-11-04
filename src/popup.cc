#include<opencv2/opencv.hpp>
#include"button.h"
using namespace std;

z::Popup::Popup(string title, cv::Rect2i r) : z::Window{title, r}
{
	title_ = title;
}

int z::Popup::open()
{
	start();
	while(!closed_) if(int key = cv::waitKey(10); key != -1) keyboard_callback(key);
	closed_ = false;
	return result_;
}

void z::Popup::quit(int r)
{
	result_ = r;
	closed_ = true;
	close();
}

z::AsciiPopup::AsciiPopup(const char *p, int unit_width, int unit_height, int margin)
	: z::AsciiWindow(p, unit_width, unit_height, margin)
{ }

int z::AsciiPopup::open()
{
	start();
	while(!closed_) if(int key = cv::waitKey(10); key != -1) keyboard_callback(key);
	closed_ = false;
	return result_;
}

void z::AsciiPopup::quit(int r)
{
	result_ = r;
	closed_ = true;
	close();
}
