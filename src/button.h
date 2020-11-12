#include<mutex>
#include<map>
#include<functional>
#include<opencv2/highgui.hpp>
#include<opencv2/freetype.hpp>
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
	static cv::Ptr<cv::freetype::FreeType2> ft2_;
	const cv::Vec3b background_color_{200, 200, 200};
	const cv::Vec3b widget_color_{220, 220, 220};
	const cv::Vec3b highlight_color_{255, 255, 255};
	const cv::Vec3b click_color_{180, 180, 180};
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
};

class CheckBox : public Widget
{
public:
	CheckBox(cv::Rect2i r);
	bool checked();
	void checked(bool);
	void on_change(std::function<void(bool)> f);
protected:
	bool checked_ = false;
private:
	void click(int, int);
	int r_;
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
	void close();
	void start(int flag = cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO);
	void keyboard_callback(int key);

protected:
	std::string title_;
	std::vector<Widget*> widgets_;
	//std::mutex mtx_;
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

class Progress : public Widget
{
public:
	Progress(cv::Rect2i r);
	void value(int val);
	int value();
protected:
	int value_ = 0;
};

class AsciiWindow : public Window
{
public:
	AsciiWindow(const char *asciiart, int unit_width = 10, int unit_height = 15, int margin = 1);
protected:
	std::vector<std::shared_ptr<Slider>> S;
	std::vector<std::shared_ptr<Button>> B;
	std::vector<std::shared_ptr<TextInput>> T;
	std::vector<std::shared_ptr<CheckBox>> C;
	std::vector<std::shared_ptr<Label>> L;
	std::vector<std::shared_ptr<Image>> I;
	std::vector<std::shared_ptr<Progress>> P;
private:
	int get_size(char c);
	bool parse_widget_area(int y, int x);
	void parse_art();
	int uw_, uh_, margin_;
	std::vector<std::string> art_, parsed_;
};

class PopupInterface {
public:
	PopupInterface(Window *p);
	int open(int flag = cv::WINDOW_AUTOSIZE);
	void quit(int r);
protected:
	bool closed_ = false;
	int result_ = 0;
private:
	Window *window_ptr_ = nullptr;
};

struct Popup : Window, PopupInterface
{
	Popup(std::string title, cv::Rect2i r);
};

struct AsciiPopup : AsciiWindow, PopupInterface
{
	AsciiPopup(const char *p, int uw = 10, int uh = 15, int margin = 1);
};

}
