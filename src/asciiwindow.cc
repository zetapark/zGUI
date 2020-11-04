#include<opencv2/opencv.hpp>
#include"button.h"
using namespace std;

	/*
	Wtitle-------------------------------------------
	|     L0-------
	|			|Test Label|
	|     S0----------------- L1--
	|     |0 100 1|           ||
	|
	|     B0----    B1------   B2-----
	|     |Popup|   |Cancel|   |Click|
	|     C0 L2--
	|     T0--------
	|     ||
	|
	|     I0---------------------
	|     ||
	|     |
	|     |
	|     |
	|
	*/


z::AsciiWindow::AsciiWindow(const char *p, int unit_width, int unit_height, int margin)
	: z::Window{"", {0,0,1,1}}
{//Button, Image, Slider, Label, Text, Chckbox, Window
	uw_ = unit_width; uh_ = unit_height; margin_ = margin;
	width = 1;
	while(*p != 'W') p++;
	p++;
	while(*p != '-') title_ += *p++;
	while(*p == '-') p++, width++;
	width += title_.size();
	for(; *p; p++) if(*p == '|') {
		string s;
		while(*p != '\n' && *p) s += *p++;
		s.resize(width, ' ');
		art_.push_back(s);
	}
	parsed_ = art_;
	for(auto &a : parsed_) for(auto &b : a) b = ' ';
	height = art_.size() * unit_height;
	width *= unit_width;
	cv::resize(mat_, mat_, {width, height});
	parse_art();
	for(auto &a : C) *this + *a.get();
	for(auto &a : B) *this + *a.get();
	for(auto &a : L) *this + *a.get();
	for(auto &a : S) *this + *a.get();
	for(auto &a : T) *this + *a.get();
	for(auto &a : I) *this + *a.get();
}

void z::AsciiWindow::parse_art() {
	bool retry = false;
	for(int y=0; y<art_.size(); y++) for(int x=0; x<art_[y].size(); x++) 
		if(isalpha(art_[y][x]) && parsed_[y][x] != 'v')
			if(!parse_widget_area(y, x)) retry = true;//if L2 -> 2 is not in order
	if(retry) parse_art();
}

int z::AsciiWindow::get_size(char c) {
	switch(c) {
		case 'B': return B.size();
		case 'L': return L.size();
		case 'S': return S.size();
		case 'C': return C.size();
		case 'T': return T.size();
		case 'I': return I.size();
		default: throw 0;
	}
}

array<int, 3> get_slider_param(const string &s, int pos)
{
	array<int, 3> r;
	stringstream ss;
	ss << s.substr(pos);
	ss >> r[0] >> r[1] >> r[2];
	return r;
}

bool z::AsciiWindow::parse_widget_area(int y, int x)
{
	string text;
	if(art_[y+1][x] == '|') for(int x2 = x; art_[y+1][++x2] != '|';) {
		text += art_[y+1][x2];
		parsed_[y+1][x2] = 'v';
	}
	if(art_[y][x+1] - '0' != get_size(art_[y][x])) return false;

	parsed_[y][x] = 'v';
	int h = 1, w = 2, num = art_[y][x+1] - '0';
	for(int y2 = y; art_[++y2][x] == '|';) h++;
	for(int x2 = x+1; art_[y][++x2] == '-';) w++;
	cv::Rect2i r{x*uw_+margin_, y*uh_+margin_, w*uw_ - 2*margin_, h*uh_ - 2*margin_};
	switch(art_[y][x]) {
		case 'B': B.emplace_back(make_shared<z::Button>(text, r)); break;
		case 'L': L.emplace_back(make_shared<z::Label>(text, r)); break;
		case 'S': { auto [a, b, c] = get_slider_param(art_[y+1], x+1);
								S.emplace_back(make_shared<z::Slider>(r, a, b, c)); }
							break;
		case 'C': C.emplace_back(make_shared<z::CheckBox>(r)); break;
		case 'T': T.emplace_back(make_shared<z::TextInput>(r)); break;
		case 'I': I.emplace_back(make_shared<z::Image>(r)); break;
	}
	return true;
}
