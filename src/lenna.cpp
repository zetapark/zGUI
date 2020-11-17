#include<filesystem>
#include<iostream>
#include"cvmatrix.h"
#include"button.h"
using namespace std;

struct Mywin : z::AsciiWindow
{
	Mywin() : z::AsciiWindow{R"(
	WLenna Test-----------------------------
	| T0----------------------B1- B2------
	| |open file|             ||  |open|
	|
	| C0 L0-----  S0----------------- L1--
	| || |gray|   |0 100 1|           |0|
	| B0-----     S1----------------- L2--
	| |Quit|      |0 200 1|           |0|
	|
	| L3----- S2-------------------- L4----
	| |Rotate||0 360 3|              |0|
	|)", 15, 21}
	{
		for(const filesystem::path &p : filesystem::directory_iterator("./"))
			if(is_regular_file(p) && p.extension() == ".png" || p.extension() == ".jpg")
				v.push_back(p.filename());
		tie("open file", 30, *T[0], *B[1], v, 700, 150);//make combobox
		wrap("Edge Detection", 20, 10, *S[0], *S[1], *L[1], *L[2]);
		start(cv::WINDOW_AUTOSIZE);
		cv::moveWindow(title_, 500, 100);
		B[0]->click([this](){cv::destroyAllWindows();});
		C[0]->on_change( [this](bool t) {
			if(t) {
				m.restore();
				m.gray();
				m.show();
			} else {
				m.restore();
				m.show();
			}
		} );
		S[0]->on_change( [this](int val) {
			m.restore();
			m.gray();
			m.edge(val, S[1]->value());
			L[1]->text(to_string(val));
			*this << *L[1];
			m.show();
		} );
		S[1]->on_change( [this](int val) {
			m.restore();
			m.gray();
			m.edge(S[0]->value(), val);
			L[2]->text(to_string(val));
			*this << *L[2];
			m.show();
		} );
		S[2]->on_change( [this](int val) {
			m.restore();
			m.rotate(val);
			m.show();
			L[4]->text(to_string(val));
			*this << *L[4];
		} );
		B[2]->click( [this]() {
			m = cv::imread(T[0]->value());
			m.show();
			m.move_window(500, 400);
			m.save();
		} );
	}

	CVMat m = cv::imread("Lenna.jpg");
	vector<string> v;
};

int main()
{
	Mywin win;
	return win.loop();
}

