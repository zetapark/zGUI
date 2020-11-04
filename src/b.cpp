#include<CvPlot/imp/cvplot.ipp>
#include<mutex>
#include<thread>
#include<iostream>
#include<opencv2/imgproc.hpp>
#include"asyncq.h"
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

class Pop : public z::Popup
{
public:
	Pop(string title, cv::Rect2i r) : Popup(title, r) {
		*this + sl_ + bt_ + la_;
		bt_.click([this]() { quit(sl_.value()); });
	}
protected:
	z::Slider sl_{{50, 50, 200, 30}, 0, 30, 1};
	z::Button bt_{"ok", {150, 100, 40, 30}};
	z::Label la_{"What do you want?", {50, 20, 200, 30}};
};

class Mat2 : public cv::Mat
{
	public:
		Mat2(const cv::Mat &r) {
			r.copyTo(*this);
		}
		Mat2 &operator=(const cv::Mat &r) {
			r.copyTo(*this);
			return *this;
		}
};

class Win : public z::Window
{
public:
	Win(string title, cv::Rect2i r) : z::Window{title, r} {
		input_.enter([](string s){cout << s << endl;});
		popup_.click([this]() {cout << pop_win_.open() << endl;});
		quit_.click([this](){ close();});
		sl_.on_change( [this](int val) { sl_label_.text(std::to_string(val)); *this << sl_label_; });
		chk_.on_change([this](bool checked) { lb_.text(checked ? "green" : "blue"); *this << lb_; });
		*this + popup_ + quit_ + click_ + chk_ + input_ + img_ + sl_ + lb_ + sl_label_;
		start();//main thread imshow or namedWindow should come first
		th_ = thread{&Win::run, this};
	}
	~Win() {
		run_ = false;
		th_.join();
	}
protected:
	
	std::thread th_;
	RotateView<double, 4000, 1000> v_;
	cv::Mat mat2_;
	Pop pop_win_{"yes no", {0, 0, 300, 300}};
	z::Button popup_{"Popup", {100, 100, 90, 30}}, quit_{"Quit", {200, 100, 80, 30}},
		click_{"click", {300, 100, 90, 30}};
	z::CheckBox chk_{{100, 200, 20, 20}};
	z::TextInput input_{{100, 300, 140, 30}};
	z::Image img_{{250, 200, 800, 500}};
	z::Slider sl_{{10, 10, 300, 30}, 0, 100, 1};
	z::Label lb_{"0", {130, 200, 70, 30}}, sl_label_{"", {320, 10, 40, 30}};
	//WQueue<Mat2, 1000> q1_{[this](cv::Mat a) { img_ = a; *this << img_;}};
	//WQueue<z::Image, 1000> q2_{[this](z::Image a) {*this << a; }};
	//WQueue<CVPLOT_LIBRARY_INTERFACE CvPlot::Axes, 1000> q_{[this](CVPLOT_LIBRARY_INTERFACE  CvPlot::Axes a) { q1_.push_back(a.render(500, 800)); }};
private:
	bool run_ = true;
	void run() {
		double x = 0;
		for(int i=0; i<2000; i++, x+=0.01) v_.push_back(sin(x));
		for(int i = 0; run_ && !chk_.checked(); i++, x+=0.01) {
			auto a = CvPlot::plot({&v_[i], sl_.value() * 10}, "-b");
			img_ = a.render(500, 800);
			*this << img_;//this should be executed after main win.start()
			v_.push_back(sin(x));
			if(i == 3000) i = -1;
			this_thread::sleep_for(10ms);
		}
	}
};

struct Mywin : z::AsciiWindow {
	Mywin() : z::AsciiWindow{ R"(
		WThis is a test--------------------------------------
		|     L0------------------    B3---------
		|     |Test Label|            |This is|
		|     S0----------------- L1--
		|     |0 100 1|           |0|
		|
		|     B0------- B1------- B2-----
		|     |Popup|   |Cancel|  |Quit|
		|     C0 L2--------
		|     || |check|
		|     T0--------
		|     ||
		|
		|     I0---------------------
		|     ||
		|     |
		|     |
		|     |
		|)" }//no tab inside
	{
		S[0]->on_change([this](int val) {L[1]->text(to_string(val)); *this << *L[1];});
		B[0]->click([]() {cout << "hello" << endl;});
		B[1]->click([this]() {L[0]->text(to_string(S[0]->value())); *this << *L[0];});
		B[2]->click([this]() {close();});
		B[3]->click([this]() {cout << "3" << endl;});
	}
};

int main()
{
	Mywin win;
	win.start();
	return win.loop();
}

