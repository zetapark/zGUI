#include<opencv2/opencv.hpp>
#include"button.h"
using namespace std;

void mouse_callback(int event, int x, int y, int flags, void *ptr)
{//get mouse event
	z::Window *p = (z::Window*)ptr;
	z::Widget *pw = nullptr;
	for(auto w : *p) {
		if(w->contains({x, y})) pw = w;//set 
		else if(w->focus()) {//leave event
			if(w->gui_callback_.find(EVENT_LEAVE) != w->gui_callback_.end()) {
				w->gui_callback_[EVENT_LEAVE](x, y);
				*p << *w;
			}
			if(w->user_callback_.find(EVENT_LEAVE) != w->user_callback_.end())
				w->user_callback_[EVENT_LEAVE](x, y);
			w->focus(false);
		}
	}
	if(!pw) return;

	if(event == cv::EVENT_MOUSEMOVE) {
		if(!pw->focus()) {//enter event
			pw->focus(true);
			if(pw->gui_callback_.find(EVENT_ENTER) != pw->gui_callback_.end()) {
				pw->gui_callback_[EVENT_ENTER](x, y);
				*p << *pw;
			} 
			if(pw->user_callback_.find(EVENT_ENTER) != pw->user_callback_.end())
				pw->user_callback_[EVENT_ENTER](x, y);
		} else {//move event
			if(pw->gui_callback_.find(event) != pw->gui_callback_.end()) {
				pw->gui_callback_[event](x, y);
				*p << *pw;
			}
			if(pw->user_callback_.find(event) != pw->user_callback_.end())
				pw->user_callback_[event](x, y);
		}
	} else {//all other event
		if(pw->gui_callback_.find(event) != pw->gui_callback_.end()) {
			pw->gui_callback_[event](x, y);
			*p << *pw;
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

z::Window& z::Window::operator+(z::Widget &w)
{
	//lock_guard<mutex> lck{mtx_};
	widgets_.push_back(&w);
	w.mat_.copyTo(mat_(w));
	return *this;
}

z::Window& z::Window::operator<<(z::Widget &r)
{
	//lock_guard<mutex> lck{mtx_};
	r.mat_.copyTo(mat_(r));
	show();
	return *this;
}

void z::Window::show()
{
	cv::imshow(title_, mat_);
}

void z::Window::quit()
{
	cv::destroyWindow(title_);
}

void z::Window::start()
{
	show();
	cv::setMouseCallback(title_, mouse_callback, this);
}

int z::Window::loop()
{//get keyboard event
	start();
	for(int key; (key = cv::waitKey()) != -1;) {//destroy window make waitkey return -1
		for(z::Widget* p : *this) if(p->focus()) {
			if(p->gui_callback_.find(EVENT_KEYBOARD) != p->gui_callback_.end()) {
				p->gui_callback_[EVENT_KEYBOARD](key, 0);
				*this << *p;
			}
			if(p->user_callback_.find(EVENT_KEYBOARD) != p->user_callback_.end())
				p->user_callback_[EVENT_KEYBOARD](key, 0);
		}
	}
	return 0;
}
