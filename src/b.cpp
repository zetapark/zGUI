#include<iostream>
#include<opencv2/imgproc.hpp>
#include"button.h"
using namespace std;
using namespace placeholders;

class Win : public z::Window
{
public:
	Win(string title, cv::Rect2i r) : z::Window{title, r} {
		mat2_ = cv::imread("/home/zeta/Pictures/11.jpg");
		//cv::resize(mat, mat2, {200, 200});
		input_.enter([](){cout << "hello" << endl;});
		popup_.click([this]() {cout << (this->pop_win_.open() ? "yes" : "no") << endl;});
		quit_.click([this](){this->quit();});
		sl_.user_callback_[EVENT_KEYBOARD] = ( [&](int, int) { 
				img_ = mat2_(cv::Rect2i{sl_.value(), 0, 200, 200}); 
				*this << img_;
			});
//		sl_.on_change([&](int val) { cout << val << endl;
//				cv::Mat mat3 = cv::imread("/home/zeta/Pictures/11.jpg"), mat2;
//				mat2 = mat3(cv::Rect2i{val, val, 200, 200});
//				img_ = mat2;
//				*this << img_;
//				show();
//				});
		*this + popup_ + quit_ + click_ + chk_ + input_ + img_ + sl_;
	}
protected:
	cv::Mat mat2_;
	z::Popup pop_win_{"yes no", {0, 0, 300, 300}, "What do you want?"};
	z::Button popup_{"Popup", {100, 100, 90, 30}}, quit_{"Quit", {200, 100, 80, 30}},
		click_{"click", {300, 100, 90, 30}};
	z::CheckBox chk_{"option", {100, 200, 20, 20}};
	z::TextInput input_{{100, 300, 140, 30}};
	z::Image img_{{250, 200, 200, 200}};
	z::Slider sl_{{10, 10, 300, 30}, 0, 100, 1};
};

int main()
{
	Win win{"hello", {0, 0, 500, 500}};
	win.show();
	return win.loop();
}

