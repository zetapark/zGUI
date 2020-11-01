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

