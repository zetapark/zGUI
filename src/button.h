#include<mutex>
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
	const cv::Vec3b background_color_ = cv::Vec3b{200, 200, 200};
};

class Label : public Widget
{
public:
	Label(std::string text, cv::Rect2i r);
	void text(std::string s);
	std::string text();
protected:
	std::string text_;
};

class Button : public Widget
{
public:
	Button(std::string text, cv::Rect_<int> r);
	void click(std::function<void()> f);
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
	void on_change(std::function<void(bool)> f);
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
	Window &operator+(Widget &w);
	Window &operator<<(Widget &r);
	int loop();
	std::vector<Widget*>::iterator begin(), end();
	void quit();

protected:
	std::string title_;
	bool first_ = true;
	std::vector<Widget*> widgets_;
	std::mutex mtx_;
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
	void click_yes(), click_no();
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
	void enter(std::function<void(std::string)> f);
protected:
	std::string value_;
private:
	void key_event(int key, int);
	const cv::Vec3b white = cv::Vec3b{255, 255, 255};
};

class Slider : public Widget
{
public:
	Slider(cv::Rect2i r, int start, int stop, int step);
	int value();
	void value(int);
	void on_change(std::function<void(int)> f);//front int = value, second int = discard
protected:
	int value_, start_, end_, step_, logical_length_, physical_length_;
	bool hold_ = false;
private:
	std::function<void(int)> on_change_;
	void key_event(int key, int);
	void move(int x, int y);
	void ldown(int x, int y);
	void lup(int x, int y);
	void draw();
	int to_pos(int val), to_val(int pos);
	bool user_hold_ = false;
};

}
