#include<map>
#include<functional>
#include <opencv2/highgui.hpp>
#define EVENT_ENTER 8000
#define EVENT_LEAVE 8001

namespace zeta {

class Widget : public cv::Rect_<int>
{//base class for all widgets
public:
	Widget(cv::Rect_<int> r);
	bool is_updated();
	void register_callback(int event, std::function<void(int,int)> f);
	void operator>>(Widget &r);//copy widget to window
	bool focus();
	void focus(bool);
	std::map<int, std::function<void(int, int)>> gui_callback_;//int : event, x, y
	std::map<int, std::function<void(int, int)>> user_callback_;

protected:
	cv::Mat3b mat_;//widget shape
	bool focus_ = false;
};

class Button : public Widget
{
public:
	Button(cv::Rect_<int> r);
	void click(auto func);
};

class Window : public Widget
{
public:
	Window(std::string title, cv::Rect_<int> r);
	void show();
	void add(Widget &w);
	std::vector<Widget*>::iterator begin(), end();

protected:
	std::string title_;
	std::vector<Widget*> widgets_;
};

}
