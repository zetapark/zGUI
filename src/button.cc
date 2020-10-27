#include<opencv2/opencv.hpp>
#include"button.h"
using namespace std;

z::Widget::Widget(cv::Rect_<int> r)
	: Rect_<int>{r}
	, mat_(r.height, r.width)
{
	mat_ = cv::Vec3b{200, 200, 200};
}

void z::Widget::operator>>(z::Widget &r)
{
	mat_.copyTo(r.mat_(*this));
}

bool z::Widget::focus() {
	return focus_;
}
void z::Widget::focus(bool tf) {
	focus_ = tf;
}

z::Button::Button(string text, cv::Rect_<int> r) : z::Widget{r}
{
	using namespace placeholders;
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
				*w >> *p;
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
			*pw >> *p;
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
			*pw >> *p;
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

void z::Window::add(z::Widget &w)
{
	widgets_.push_back(&w);
	w >> *this;
}

void z::Window::show()
{
	cv::imshow(title_, mat_);
	if(first_) {
		cv::setMouseCallback(title_, mouse_callback, this);
		first_ = false;
	}
}

z::Popup::Popup(string title, cv::Rect2i r, string content) : z::Window{title, r}
{
	using namespace std::placeholders;
	title_ = title;
	add(yes_);
	add(no_);
	cv::putText(mat_, content, {10, 30}, 0, 0.7, {0, 0, 0});
	yes_.click(bind(&Popup::click_yes, this, _1, _2));
	no_.click(bind(&Popup::click_no, this, _1, _2));
}

bool z::Popup::open()
{
	show();
	cout << "inside next" << endl;
	//cv::waitKey(300);
	cout << "waitkey collapse" << endl;
	
	std::unique_lock lck{mtx_};
	while(!closed_) {
		cv::waitKey(1);
	}
	cv_.wait(lck);
	cout << "continue" << endl;
	closed_ = false;
	return result_;
}

void z::Popup::click_yes(int, int)
{
	std::unique_lock lck{mtx_};
	cout << "yse" << endl;
	first_ = result_ = closed_ = true;
	cv::destroyWindow(title_);
	cv_.notify_one();
}

void z::Popup::click_no(int, int)
{
	std::unique_lock lck{mtx_};
	cout << "nono" << endl;
	first_ = closed_ = true;
	result_ = false; 
	cv::destroyWindow(title_);
	cv_.notify_one();
}
