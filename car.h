#pragma once
#include "DxLib.h"
#include <string>
using namespace std;

class Car {
private:
	int x, y, now_x, car_num, car_type, speed;

public:
	Car() :
		x(0),
		y(0),
		now_x(0),
		car_type(0),
		speed(0)
	{}
	Car(int i) :
		Car()
	{
		car_num = i;
	}
	~Car(){}
	
	void resetX(int x) {
		now_x = x;
	}
	void set_move_X(int in_x) {
		now_x += in_x;
	}
	void set_Y(int in_y) {
		y = in_y;
	}
	void set_car_type(int type) {
		car_type = type;
	}
	void set_car_num(int num) {
		car_num = num;
	}
	void set_speed(int in_speed) {
		speed = in_speed;
	}
	int get_move_X(){
		return now_x;
	}
	int get_Y() {
		return y;
	}
	int get_car_num() {
		return car_num;
	}
	int get_car_type() {
		return car_type;
	}
	int get_car_speed() {
		return speed;
	}
};