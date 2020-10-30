#include<opencv2/opencv.hpp>
#include"button.h"
using namespace std;
using namespace placeholders;

z::TextInput::TextInput(cv::Rect2i r) : z::Widget{r}
{
	mat_ = white;
	gui_callback_[EVENT_KEYBOARD] = bind(&z::TextInput::key_event, this, _1, _2);
}

void z::TextInput::key_event(int key, int)
{
	if(key == 13) return user_callback_[EVENT_KEYBOARD](0, 0);
	if(key == 8) value_.pop_back();
	else value_ += key;
	cout << key << endl;
	mat_ = white;
	cv::putText(mat_, value_, {10, 20}, 0, .7, {0,0,0}, 2);
}

void z::TextInput::enter(function<void()> f)
{
	user_callback_[EVENT_KEYBOARD] = [f](int, int){f();};
}

string z::TextInput::value()
{
	return value_;
}

