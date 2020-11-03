#include<opencv2/opencv.hpp>
#include"button.h"
using namespace std;

	/*
	Wtitle-------------------------------------------
	|     L0-------
	|			|Test Label|
	|     S0----------------- L1--
	|     |0 1 100|           ||
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


z::AsciiWindow::AsciiWindow(const char *p, int unit_width, int unit_height, int tab, int margin)
	: z::Window{"", {0,0,1,1}}
{//Button, Image, Slider, Label, Text, Chckbox, Window
	uw_ = unit_width; uh_ = unit_height; margin_ = margin; tab_ = tab;
	width = 1;
	while(*p != 'W') p++;
	p++;
	while(*p != '-') title_ += *p++;
	while(*p == '-') {
		p++;
		width++;
	}
	width += title_.size();
	for(; *p; p++) if(*p == '|') {
		string s;
		while(*p != '\n' && *p) s += *p++;
		s.resize(width, ' ');
		art_.push_back(s);
	}
	string s; s.resize(width, ' ');
	parsed_.resize(art_.size(), s);
	height = art_.size() * unit_height;
	width *= unit_width;
	cv::resize(mat_, mat_, {width, height});
}

void z::AsciiWindow::parse_art() {
	for(int y=0; y<art_.size(); y++) for(int x=0; x<art_[y].size(); x++) {
		if(char c = art[y][x]; is_alpha(c) && parsed_[y][x] != 'v') parse_widget_area(y, x);
	}
}
array<int, 3> get_slider(const string &s, int pos)
{
	while(s[pos] != 
}
bool parse_widget_area(int y, int x) {
	parsed_[y][x] = 'v';
	int h = 1, w = 2, num = art_[y][x+1] - '0';
	for(int y2 = y; art_[++y2][x] == '|';) h++;
	for(int x2 = x+1; art_[y][++x2] == '-';) w++;
	Rect2i r{x+margin_, y+margin_, w*uw_ - 2*margin_, h*uh_ - 2*margin_};
	string text;
	int a, b, c;
	switch(art_[y][x]) {
		case 'L': for(int x2 = x; art_[y+1][++x2] != '|') text += art_[y+1][x2];
							L.push_back({text, r});
							break;
		case 'S': for(int x2=x; is_num(art_[y+1][++x2]);)
		case 'B':
		case 'C':
		case 'T':
		case 'I':
	}
	parsed_[y][x] = 'v';
}
