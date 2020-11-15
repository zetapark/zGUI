#include"button.h"
using namespace std;

struct Tie : z::Popup {
	Tie(string title, vector<string> v) : z::Popup{title, {0, 0, 300, 30 * v.size()}} {
		this->v = v;
		for(int i=0; i<v.size(); i++) {
			bts.emplace_back(make_shared<z::Button>(v[i], cv::Rect2i{0, i*30, 300, 30})); 
			bts.back()->click([i, this](){quit(i);});
			*this + *bts.back();
			update(*bts.back());
		}
	}
	vector<shared_ptr<z::Button>> bts;
	vector<string> v;
	string open() {
		return v[z::Popup::open()];
	}
};

void z::Window::tie(string title, z::TextInput &t, z::Button &b, vector<string> v)
{
	static vector<shared_ptr<Tie>> vw;
	vw.emplace_back(make_shared<Tie>(title, v));
	b.text("\u25bc");
	int k = vw.size()-1;
	b.click([&, k](){t.value(vw[k]->open()); *this << t;});
}

