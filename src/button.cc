#include<opencv2/opencv.hpp>
#include"button.h"
using namespace std;
using namespace placeholders;

const cv::Vec3b z::Widget::background_color_ = cv::Vec3b{200, 200, 200};

z::Widget::Widget(cv::Rect_<int> r)
	: Rect_<int>{r}
	, mat_(r.height, r.width, background_color_)
{ }

bool z::Widget::focus() {
	return focus_;
}
void z::Widget::focus(bool tf) {
	focus_ = tf;
}

z::Button::Button(string text, cv::Rect_<int> r) : z::Widget{r}
{
	text_ = text;
	mat_ = base_color;
	//mat_(cv::Rect2i{4, 4, this->width - 8, this->height - 8}) = base_color;
	label();
	gui_callback_[EVENT_ENTER] = bind(&Button::enter, this, _1, _2);
	gui_callback_[EVENT_LEAVE] = bind(&Button::leave, this, _1, _2);
	gui_callback_[cv::EVENT_LBUTTONDOWN] = bind(&z::Button::ldown, this, _1, _2);
	gui_callback_[cv::EVENT_LBUTTONUP] = bind(&z::Button::lup, this, _1, _2);
}

void z::Button::label()
{
	cv::putText(mat_, text_, {10, 20}, 0, .7, {0,0,0}, 2);
}
void z::Button::leave(int, int)
{
	mat_ = base_color;
	label();
}
void z::Button::enter(int, int)
{
	mat_ = hover_color;
	label();
}
void z::Button::ldown(int, int)
{
	mat_ = click_color;
	label();
}
void z::Button::lup(int x, int y)
{
	mat_ = base_color;
	label();
}
void z::Button::click(function<void(int, int)> f) 
{
	user_callback_[cv::EVENT_LBUTTONUP] = f;
}

void mouse_callback(int event, int x, int y, int flags, void *ptr)
{
	z::Window *p = (z::Window*)ptr;
	z::Widget *pw = nullptr;
	for(auto w : *p) {
		if(w->contains({x, y})) { pw = w; }
		else if(w->focus()) {
			if(w->gui_callback_.find(EVENT_LEAVE) != w->gui_callback_.end()) {
				w->gui_callback_[EVENT_LEAVE](x, y);
				*p << *w;
				p->show();
			}
			if(w->user_callback_.find(EVENT_LEAVE) != w->user_callback_.end())
				w->user_callback_[EVENT_LEAVE](x, y);
			w->focus(false);
		}
	}
	if(!pw) return;

	if(event == cv::EVENT_MOUSEMOVE) {
		if(!pw->focus()) {
			pw->focus(true);
			if(pw->gui_callback_.find(EVENT_ENTER) != pw->gui_callback_.end()) {
				pw->gui_callback_[EVENT_ENTER](x, y);
				*p << *pw;
				p->show();
			} 
			if(pw->user_callback_.find(EVENT_ENTER) != pw->user_callback_.end())
				pw->user_callback_[EVENT_ENTER](x, y);
		} else {
			if(pw->gui_callback_.find(event) != pw->gui_callback_.end()) {
				pw->gui_callback_[event](x, y);
				*p << *pw;
				p->show();
			}
		}
	} else {
		if(pw->gui_callback_.find(event) != pw->gui_callback_.end()) {
			pw->gui_callback_[event](x, y);
			*p << *pw;
			p->show();
		}
		if(pw->user_callback_.find(event) != pw->user_callback_.end())
			pw->user_callback_[event](x, y);
	}
}

z::Window::Window(string title, cv::Rect2i r) : z::Widget{r}
{
	title_ = title;
}

vector<z::Widget*>::iterator z::Window::begin() {
	return widgets_.begin();
}
vector<z::Widget*>::iterator z::Window::end() {
	return widgets_.end();
}

void z::Window::operator+=(z::Widget &w)
{
	widgets_.push_back(&w);
	*this << w;
}

void z::Window::operator<<(z::Widget &r)
{
	r.mat_.copyTo(mat_(r));
}

void z::Window::show()
{
	if(first_) {
		cv::namedWindow(title_, 0);
		cv::setMouseCallback(title_, mouse_callback, this);
		first_ = false;
	}
	cv::imshow(title_, mat_);
}

void z::Window::quit()
{
	cv::destroyWindow(title_);
}

int z::Window::loop()
{
	for(int key; (key = cv::waitKey()) != -1;) {//destroy window make waitkey return -1
		for(z::Widget* p : *this)
			if(p->focus() && p->gui_callback_.find(EVENT_KEYBOARD) != p->gui_callback_.end()) {
				p->gui_callback_[EVENT_KEYBOARD](key, 0);
				*this << *p;
				show();
			}
	}
	return 0;
}

z::Popup::Popup(string title, cv::Rect2i r, string content) : z::Window{title, r}
{
	title_ = title;
	*this += yes_;
	*this += no_;
	cv::putText(mat_, content, {10, 30}, 0, 0.7, {0, 0, 0});
	yes_.click(bind(&Popup::click_yes, this, _1, _2));
	no_.click(bind(&Popup::click_no, this, _1, _2));
}

bool z::Popup::open()
{
	show();
	while(!closed_) cv::waitKey(10);
	closed_ = false;
	return result_;
}

void z::Popup::click_yes(int, int)
{
	closed_ = first_ = result_ = true;
	quit();
}

void z::Popup::click_no(int, int)
{
	result_ = false; 
	closed_ = first_ = true;//closed first to avoid mutex
	quit();
}

z::CheckBox::CheckBox(string text, cv::Rect2i r) : z::Widget{r}
{
	mat_ = cv::Vec3b{100, 100, 100};
	inner_rect_ = cv::Rect2i{cv::Point2i{5, 5}, cv::Point2i{width-5, height-5}};
	cv::rectangle(mat_, inner_rect_, cv::Scalar{200, 200, 200}, -1);
	gui_callback_[cv::EVENT_LBUTTONUP] = bind(&CheckBox::click, this, _1, _2);
}

void z::CheckBox::click(int, int) 
{
	cv::rectangle(mat_, inner_rect_, checked_ ? cv::Scalar{200, 200, 200} : cv::Scalar{0, 255, 0}, -1);
	checked_ = !checked_;
}

bool z::CheckBox::checked()
{
	return checked_;
}

z::Image::Image(cv::Rect2i r) : Widget{r}
{ }

cv::Mat &z::Image::operator=(const cv::Mat &r)
{
	mat_ = r({0, 0, min(r.cols, width), min(r.rows, height)});
	return mat_;
}

z::TextInput::TextInput(cv::Rect2i r) : z::Widget{r}
{
	mat_ = white;
	gui_callback_[EVENT_KEYBOARD] = bind(&z::TextInput::key_event, this, _1, _2);
}

void z::TextInput::key_event(int key, int)
{
	if(key == 8) value_.pop_back();
	else value_ += key;
	cout << key << endl;
	mat_ = white;
	cv::putText(mat_, value_, {10, 20}, 0, .7, {0,0,0}, 2);
}

string z::TextInput::value()
{
	return value_;
}

z::Slider::Slider(cv::Rect2i r, int start, int end, int step) : z::Widget{r}
{
	start_ = start; end_ = end; step_ = step;
	value_ = (end - start) / 2;
	logical_length_ = end_ - start_;
	physical_length_ = width - 20;
	draw();
	gui_callback_[cv::EVENT_LBUTTONUP] = bind(&z::Slider::lup, this, _1, _2);
	gui_callback_[cv::EVENT_LBUTTONDOWN] = bind(&z::Slider::ldown, this, _1, _2);
	gui_callback_[cv::EVENT_MOUSEMOVE] = bind(&z::Slider::move, this, _1, _2);
	gui_callback_[EVENT_KEYBOARD] = bind(&z::Slider::key_event, this, _1, _2);
}

void z::Slider::draw()
{
	mat_ = background_color_;
	cv::line(mat_, {10, height / 2}, {width - 11, height / 2}, {100, 100, 100});
	cv::circle(mat_, {to_pos(value_), height / 2}, 10, {230, 230, 230}, -1);
}

int z::Slider::to_pos(int val)
{
	return (val - start_) * physical_length_ / logical_length_ + 10;
}
int z::Slider::to_val(int pos)
{
	return (pos - 10) * logical_length_ / physical_length_ + start_;
}

void z::Slider::value(int v)
{
	value_ = v;
}

int z::Slider::value()
{
	return value_;
}

void z::Slider::ldown(int x, int y)
{
	int pos = to_pos(value_);
	if(pos - 10 < x - this->x && x - this->x < pos + 10) hold_ = true;
}

void z::Slider::move(int x, int y)
{
	if(hold_) {
		value(to_val(x - this->x));
		draw();
	}
}

void z::Slider::lup(int x, int y)
{
	if(hold_) hold_ = false;
	else {
		value(to_val(x - this->x));
		draw();
	}
}

void z::Slider::key_event(int key, int)
{
	if(key == 81) value_ -= step_;//left arrow key
	else if(key == 83) value_ += step_;//right arrow key
	draw();
}
