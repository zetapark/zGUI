#include<thread>
#include<CvPlot/imp/cvplot.ipp>
#define CVUI_IMPLEMENTATION
#include<CvPlot/cvui.h>
#include<iostream>
#include<opencv2/imgproc.hpp>
#include"button.h"
using namespace std;
using namespace placeholders;

template<class T, int N, int V> requires (N >= 2 * V)//buffer should be more than 2 times
class RotateView : public array<T, N>
{//to show graph of V values without stopping (values are added dynamically)
public:
	void push_back(T val) {
		if(N - pos_ < V) (*this)[V - N + pos_ - 1] = val;
		(*this)[pos_++] = val;
		if(pos_ == N) pos_ = V;
	}
protected:
	int pos_ = 0;
};

class Win : public z::Window
{
public:
	Win(string title, cv::Rect2i r) : z::Window{title, r}, th_{&Win::run, this} {
		input_.enter([](string s){cout << s << endl;});
		popup_.click([this]() {cout << (pop_win_.open() ? "yes" : "no") << endl;});
		quit_.click([this](){quit();});
		sl_.on_change( [this](int val) {
//				auto axes = CvPlot::plot({&v_[val], 1000});
//				img_ = axes.render(500, 800);
//				*this << img_;
		});
		chk_.on_change([this](bool checked) { lb_.text(checked ? "green" : "blue"); *this << lb_; });
		*this + popup_ + quit_ + click_ + chk_ + input_ + img_ + sl_ + lb_;
	}
	~Win() {
		run_ = false;
		th_.join();
	}
protected:
	std::thread th_;
	array<double, 4000> v_;
	cv::Mat mat2_;
	z::Popup pop_win_{"yes no", {0, 0, 300, 300}, "What do you want?"};
	z::Button popup_{"Popup", {100, 100, 90, 30}}, quit_{"Quit", {200, 100, 80, 30}},
		click_{"click", {300, 100, 90, 30}};
	z::CheckBox chk_{"option", {100, 200, 20, 20}};
	z::TextInput input_{{100, 300, 140, 30}};
	z::Image img_{{250, 200, 800, 500}};
	z::Slider sl_{{10, 10, 300, 30}, 0, 100, 1};
	z::Label lb_{"0", {130, 200, 70, 30}};
private:
	bool run_ = true;
	void run() {
		double x = 0;
		for(int i=0; i<4000; i++, x+=0.01) v_[i] = sin(x);
		for(int i = 0; i<3000 && run_; i++) {
			auto axes = CvPlot::plot({&v_[i], sl_.value() * 10}, chk_.checked() ? "-g" : "-b");
			img_ = axes.render(500, 800);
			*this << img_;
			this_thread::sleep_for(10ms);
		}
	}
};

int main()
{
	Win win{"hello", {0, 0, 1100, 900}};
	win.show();
	return win.loop();
}

