#include<map>
#include<functional>
#include <opencv2/highgui.hpp>
#define EVENT_ENTER 8000
#define EVENT_LEAVE 8001
#define EVENT_KEYBOARD 8002

namespace z {

class Widget : public cv::Rect_<int>
{//base class for all widgets
public:
	Widget(cv::Rect_<int> r);
	bool is_updated();
	//void register_callback(int event, std::function<void(int,int)> f);
	bool focus();
	void focus(bool);
	std::map<int, std::function<void(int, int)>> gui_callback_;//int : event, x, y
	std::map<int, std::function<void(int, int)>> user_callback_;
	cv::Mat3b mat_;//widget shape

protected:
	bool focus_ = false;
};

class Button : public Widget
{
public:
	Button(std::string text, cv::Rect_<int> r);
	void click(std::function<void(int, int)> f);
protected:
	std::string text_;
private:
	void leave(int, int);
	void enter(int, int);
	void ldown(int, int);
	void lup(int, int);
	void label();
	const cv::Vec3b base_color{220, 220, 220};
	const cv::Vec3b hover_color{255, 255, 255};
	const cv::Vec3b click_color{185, 180, 180};
};

class CheckBox : public Widget
{
public:
	CheckBox(std::string text, cv::Rect2i r);
	bool checked();
protected:
	std::string text_;
	bool checked_ = false;
private:
	void click(int, int);
	cv::Rect2i inner_rect_;
};

class Window : public Widget
{
public:
	Window(std::string title, cv::Rect_<int> r);
	void show();
	void operator+=(Widget &w);
	void operator<<(Widget &r);
	int loop();
	std::vector<Widget*>::iterator begin(), end();
	void quit();

protected:
	std::string title_;
	bool first_ = true;
	std::vector<Widget*> widgets_;
};

class Popup : public Window
{
public:
	Popup(std::string title, cv::Rect2i r, std::string content);
	bool open();
protected:
	Button yes_{"Yes", {30, 100, 50, 30}}, no_{"No", {100, 100, 50, 30}};
private:
	bool closed_ = false, result_ = false;
	void click_yes(int, int), click_no(int, int);
};

class Image : public Widget
{
public:
	Image(cv::Rect2i r);
	cv::Mat &operator=(const cv::Mat &r);
};

class TextInput : public Widget
{
public:
	TextInput(cv::Rect2i r);
	std::string value();
protected:
	std::string value_;
};

}
