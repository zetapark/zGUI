#include<iostream>
#include"button.h"
using namespace std;

zeta::Widget::Widget(cv::Rect_<int> r)
	: Rect_<int>{r}
	, mat_(r.height, r.width)
{
	mat_ = cv::Vec3b{0xff, 0, 0};
}

void zeta::Widget::operator>>(zeta::Widget &r)
{
	mat_.copyTo(r.mat_(*this));
}

void zeta::Widget::register_callback(int event, function<void(int, int)> f)
{
	user_callback_[event] = f;
}

bool zeta::Widget::focus() {
	return focus_;
}
void zeta::Widget::focus(bool tf) {
	focus_ = tf;
}

zeta::Button::Button(cv::Rect_<int> r) : zeta::Widget{r}
{
	gui_callback_[EVENT_ENTER] = [this](int, int) { this->mat_ = cv::Vec3b{0, 0xff, 0}; }; 
	gui_callback_[EVENT_LEAVE] = [this](int, int) { this->mat_ = cv::Vec3b{0xff, 0xff, 0}; }; 
	gui_callback_[cv::EVENT_LBUTTONDOWN] = [this](int, int) { this->mat_ = cv::Vec3b{0, 0, 0xff}; }; 
	gui_callback_[cv::EVENT_LBUTTONUP] = [this](int, int) { this->mat_ = cv::Vec3b{0xff, 0, 0xff}; }; 
}

void mouse_callback(int event, int x, int y, int flags, void *ptr)
{
	cout << '.' << flush;
	zeta::Window *p = (zeta::Window*)ptr;
	zeta::Widget *pw = nullptr;
	for(auto w : *p) {
		if(w->contains({x, y})) { pw = w; }
		else if(w->focus()) {
			if(w->gui_callback_.find(EVENT_LEAVE) != w->gui_callback_.end()) {
				w->gui_callback_[EVENT_LEAVE](x, y);
				*w >> *p;
				p->show();
			}
			w->focus(false);
		}
	}
	if(!pw) return;

	if(event == cv::EVENT_MOUSEMOVE) {
		if(!pw->focus() && pw->gui_callback_.find(EVENT_ENTER) != pw->gui_callback_.end()) {
			pw->gui_callback_[EVENT_ENTER](x, y);
			*pw >> *p;
			p->show();
			pw->focus(true);
		} 
	} else if(pw->gui_callback_.find(event) != pw->gui_callback_.end()) {
		pw->gui_callback_[event](x, y);
		*pw >> *p;
		p->show();
	}
}

zeta::Window::Window(string title, cv::Rect2i r) : zeta::Widget{r}
{
	cv::namedWindow(title, 0);
	title_ = title;
	cv::setMouseCallback(title_, mouse_callback, this);
}

vector<zeta::Widget*>::iterator zeta::Window::begin() {
	return widgets_.begin();
}
vector<zeta::Widget*>::iterator zeta::Window::end() {
	return widgets_.end();
}

void zeta::Window::add(zeta::Widget &w)
{
	widgets_.push_back(&w);
}

void zeta::Window::show()
{
	cv::imshow(title_, mat_);
}

