#include"button.h"
using namespace std;

struct Tie : z::Popup {
	Tie(string title, int font, vector<string> v) : z::Popup{title, {0, 0, 300, font * v.size()}} {
		this->v = v;
		int length = 0;
		for(int i=0, base=0; i<v.size(); i++) {
			int k = ft2_->getTextSize(v[i], font, -1, &base).width;
			if(length < k) length = k;
		}
		cv::resize(mat_, mat_, {length, font * v.size()});
		for(int i=0; i<v.size(); i++) {
			bts.emplace_back(make_shared<z::Button>(v[i], cv::Rect2i{0, i*font, length, 30})); 
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

void z::Window::tie(string title, int font, z::TextInput &t, z::Button &b, vector<string> v)
{//combobox
	static vector<shared_ptr<Tie>> vw;
	vw.emplace_back(make_shared<Tie>(title, font, v));
	b.text("\u25bc");
	update(b);
	int k = vw.size()-1;
	b.click([&, k](){t.value(vw[k]->open()); *this << t;});
}

void z::Window::tie(z::TextInput &t, z::Button &b1, z::Button &b2, double start, double step)
{//number input
	b1.text("\u25b5"); b2.text("\u25bf");
	t.value(to_string(start));
	update(b1); update(b2); update(t);
	b1.click([&](){t.value(to_string(stoi(t.value()) + step)); *this << t;});
	b2.click([&](){t.value(to_string(stoi(t.value()) - step)); *this << t;});
}

