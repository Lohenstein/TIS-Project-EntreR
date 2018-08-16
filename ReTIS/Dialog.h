#pragma once

using namespace std;

class cDialog {
public:
	string message;
	string out;
	clock_t wait_time = 50;
	clock_t start = 0, end = 0;
	int str_count = 0;

	cDialog(string str) {
		message = str;
		start = clock();
	}
	~cDialog() {
		delete this;
	}

	void	Render();
	void	Update();
};