#include<opencv2/opencv.hpp>
#include"button.h"
using namespace std;

z::PopupInterface::PopupInterface(z::Window *p)
{
	window_ptr_ = p;
}

int z::PopupInterface::open(int flag)
{
	window_ptr_->start(flag);
	while(!closed_) if(int key = cv::waitKey(10); key != -1) window_ptr_->keyboard_callback(key);
	closed_ = false;
	return result_;
}

void z::PopupInterface::quit(int r)
{
	result_ = r;
	closed_ = true;
	window_ptr_->close();
}

z::Popup::Popup(string title, cv::Rect2i r) : z::Window{title, r}, PopupInterface{this}
{ }

z::AsciiPopup::AsciiPopup(const char *p, int unit_width, int unit_height, int margin)
	: z::AsciiWindow(p, unit_width, unit_height, margin), PopupInterface{this}
{ }

