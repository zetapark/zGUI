#include<iostream>
#include"asyncq.h"
using namespace std;

int main()
{
	WQueue<int, 100> q2{[&](int i) { cout << "q2 " << i << endl;}};
	WQueue<int, 100> q{[&](int i) { cout << "q " << i << endl; q2.push_back(i+1);}};
	thread th{[&]() { for(int i=0; i<10000; i++) q.push_back(i); }};
	thread th2{[&]() { for(int i=0; i<10000; i++) q.push_back(-i); }};
	th.join();
	th2.join();
	char c;
	cin >> c;
}

