#include<opencv2/opencv.hpp>
#include"button.h"
using namespace std;
using namespace placeholders;

z::Widget::Widget(cv::Rect_<int> r)
	: Rect_<int>{r}
	, mat_(r.height, r.width, cv::Vec3b{200, 200, 200})
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
		if(!pw->focus() && pw->gui_callback_.find(EVENT_ENTER) != pw->gui_callback_.end()) {
			pw->gui_callback_[EVENT_ENTER](x, y);
			*p << *pw;
			p->show();
			pw->focus(true);
		} 
		if(!pw->focus() && pw->user_callback_.find(EVENT_ENTER) != pw->user_callback_.end()) {
			pw->user_callback_[EVENT_ENTER](x, y);
			pw->focus(true);
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
			if(p->focus() && p->gui_callback_.find(EVENT_KEYBOARD) != p->gui_callback_.end())
				p->gui_callback_[EVENT_KEYBOARD](key, 0);
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
	mat_ = cv::Vec3b{255, 255, 255};
}
