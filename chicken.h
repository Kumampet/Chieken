#pragma once
#include "DxLib.h"

class chicken {
private:
	int x, y;

public:
	chicken() {
		x = 320;
		y = 460;
	}
	void Init() {
		x = 320;
		y = 460;
	}
	void set_Y(int in_y) {
		y += in_y;
	}
	int get_X() {
		return x;
	}
	int get_Y() {
		return y;
	}
};