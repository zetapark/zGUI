#include<catch.hpp>
#include<sstream>
using namespace std;

//z::Window parse_art(const char *p, int, int);

TEST_CASE("ascii to str") {
	array<int, 3> r;
	stringstream ss;
	string s = "|0 100 1|";
	ss << s.substr(1);
	ss >> r[0] >> r[1] >> r[2];
	REQUIRE((r[0] == 0 && r[1] == 100 && r[2] == 1));
}

