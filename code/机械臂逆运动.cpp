#include <iostream>
#include <cmath>
#include <map>
#include <cstdio>
#define PI 3.14159265358979323846
#define MISTAKE 0.0001
#define TOA(x) ((double)(x) / 180 * PI)
using namespace std;
const double a[6] = { 0, 33, 155, 135, 0, 0 };            //连杆长度
const double A[6] = { 0, PI / 2 , 0, 0, -PI / 2 , 0 };    //连杆转角
const double d[6] = { 0, 75, 0, 0, 0, 218 };              //连杆偏距
double nx, ny, nz;
double ox, oy, oz;
double ax, ay, az;
double px, py, pz;
//有效范围
double ru[6] = { 0,TOA(169),TOA(90),TOA(146),TOA(102.5),TOA(167.5) };
double rd[6] = { 0,TOA(-169),TOA(-65),TOA(-151),TOA(-102.5),TOA(-167.5) };

void calculate_angle_1();
void calculate_angle_234(double _1);
void calculate_angle_3(double _1, double _234);
void calculate_angle_5(double _1, double _3, double _234);
void calculate_angle_2(double _1, double _3, double _234, double _5);
void calculate_angle_4(double _1, double _2, double _3, double _234, double _5);
void filter(double _1, double _2, double _3, double _4, double _5);
void print(double _1, double _2, double _3, double _4, double _5);
void f(double _1, double _2, double _3, double _4, double _5);
void fix(double &x);
//计算角1
void calculate_angle_1() {
	double _1;
	_1 = atan2(py, px);
	if (_1 >= rd[1] && _1 <= ru[1]) {
		calculate_angle_234(_1);
	}
	_1 = atan2(-py, -px);
	if (_1 >= rd[1] && _1 <= ru[1]) {
		calculate_angle_234(_1);
	}
}
//计算角2+3+4
void calculate_angle_234(double _1) {
	double s1, c1, s234, c234, _234;
	s1 = sin(_1);
	c1 = cos(_1);
	s234 = ax * c1 + ay * s1;
	c234 = -az;
	_234 = atan2(s234, c234);
	if (_234 >= rd[2] + rd[3] + rd[4] && _234 <= ru[2] + ru[3] + ru[4]) {
		calculate_angle_3(_1, _234);
	}
	_234 = atan2(-s234, -c234);
	if (_234 >= rd[2] + rd[3] + rd[4] && _234 <= ru[2] + ru[3] + ru[4]) {
		calculate_angle_3(_1, _234);
	}
}
//计算角3
void calculate_angle_3(double _1, double _234) {
	double s1, c1, s234, c234, x, y, m, n, s3, c3, _3;
	s1 = sin(_1);
	c1 = cos(_1);
	s234 = sin(_234);
	c234 = cos(_234);
	x = c1*px - a[1] + py * s1;
	y = pz - d[1];
	m = x + d[5] * s234;
	n = y - d[5] * c234;
	c3 = (m*m + n*n - a[2] * a[2] - a[3] * a[3]) / (2 * a[2] * a[3]);
	if (fabs(c3) > 1) return;
	s3 = sqrt(1 - c3*c3);
	_3 = atan2(s3, c3);
	if (_3 >= rd[3] && _3 <= ru[3]) {
		calculate_angle_5(_1, _3, _234);
	}
	_3 = atan2(-s3, c3);
	if (_3 >= rd[3] && _3 <= ru[3]) {
		calculate_angle_5(_1, _3, _234);
	}
}
//计算角5
void calculate_angle_5(double _1, double _3, double _234) {
	double s1, c1, s5, c5, _5;
	s1 = sin(_1);
	c1 = cos(_1);
	s5 = nx*s1 - c1*ny;
	c5 = ox*s1 - c1*oy;
	_5 = atan2(s5, c5);
	if (_5 >= rd[5] && _5 <= ru[5]) {
		calculate_angle_2(_1, _3, _234, _5);
	}
	_5 = atan2(-s5, -c5);
	if (_5 >= rd[5] && _5 <= ru[5]) {
		calculate_angle_2(_1, _3, _234, _5);
	}
}
//计算角2
void calculate_angle_2(double _1, double _3, double _234, double _5) {
	double s1, c1, s3, c3, _2, f1, f2, f3, t;
	s1 = sin(_1);
	c1 = cos(_1);
	s3 = sin(_3);
	c3 = cos(_3);
	f1 = py*s1 + px*c1 - d[5] * ax*c1 - d[5] * ay*s1 - a[1];
	f2 = d[1] + az*d[5] - pz;
	f3 = a[2] * s3;
	t = f1*f1 + f2*f2 - f3*f3;
	if (t < 0) return;
	t = sqrt(t);
	double t1 = atan2(f2, f1) / PI * 180;
	double t2 = atan2(f3, t) / PI * 180;
	_2 = -atan2(f2 , f1) + atan2(f3 , t) - _3;
	fix(_2);
	if (_2 >= rd[2] && _2 <= ru[2]) {
		calculate_angle_4(_1, _2, _3, _234, _5);
	}
	_2 = -atan2(f2, f1) + atan2(f3, -t) - _3;
	fix(_2);
	if (_2 >= rd[2] && _2 <= ru[2]) {
		calculate_angle_4(_1, _2, _3, _234, _5);
	}
}
//计算角4
void calculate_angle_4(double _1, double _2, double _3, double _234, double _5) {
	double _4;
	_4 = _234 - _3 - _2;
	fix(_4);
	if (_4 < rd[4] || _4 > ru[4]) return;  
	filter(_1, _2, _3, _4, _5);
}
//过滤
void filter(double _1, double _2, double _3, double _4, double _5) {
	double s1, s2, s3, s4, s5, s23, s234;
	double c1, c2, c3, c4, c5, c23, c234;
	double _ax, _ay, _az, _ox, _oy, _oz, _nx, _ny, _nz, _px, _py, _pz;
	s1 = sin(_1);
	s2 = sin(_2);
	s3 = sin(_3);
	s4 = sin(_4);
	s5 = sin(_5);
	s23 = sin(_2 + _3);
	s234 = sin(_2 + _3 + _4);
	c1 = cos(_1);
	c2 = cos(_2);
	c3 = cos(_3);
	c4 = cos(_4);
	c5 = cos(_5);
	c23 = cos(_2 + _3);
	c234 = cos(_2 + _3 + _4);
	_nx = -s1*s5 + c1*c5*c234;
	_ny = c1*s5 + c5*s1*c234;
	_nz = c5*s234;
	_ox = -c5*s1 - c1*s5*c234;
	_oy = c1*c5 - s1*s5*c234;
	_oz = s5*s234;
	_ax = -c1*s234;
	_ay = -s1*s234;
	_az = c234;
	_px = c1*(a[1] + a[2] * c2 + a[3] * c23) - d[5] * c1*s234;
	_py = s1*(a[1] + a[2] * c2 + a[3] * c23) - d[5] * s1*s234;
	_pz = d[1] + a[2] * s2 + a[3] * s23 + d[5] * c234;
	if (fabs(ax - _ax) > MISTAKE) return;
	if (fabs(ay - _ay) > MISTAKE) return;
	if (fabs(az - _az) > MISTAKE) return;
	if (fabs(ox - _ox) > MISTAKE) return;
	if (fabs(oy - _oy) > MISTAKE) return;
	if (fabs(oz - _oz) > MISTAKE) return;
	if (fabs(nx - _nx) > MISTAKE) return;
	if (fabs(ny - _ny) > MISTAKE) return;
	if (fabs(nz - _nz) > MISTAKE) return;
	if (fabs(px - _px) > MISTAKE) return;
	if (fabs(py - _py) > MISTAKE) return;
	if (fabs(pz - _pz) > MISTAKE) return;
	//printf("%.2lf\t%.2lf\t%.2lf\t%.2lf\n", _nx, _ox, _ax, _px);
	//printf("%.2lf\t%.2lf\t%.2lf\t%.2lf\n", _ny, _oy, _ay, _py);
	//printf("%.2lf\t%.2lf\t%.2lf\t%.2lf\n", _nz, _oz, _az, _pz);
	print(_1, _2, _3, _4, _5);
}
//打印
void print(double _1, double _2, double _3, double _4, double _5) {
	cout << "*********************************************************************************" << endl;
	double temp = 180 / PI;
	printf("解：  %.2lf\t\t%.2lf\t\t%.2lf\t\t%.2lf\t\t%.2lf\n", _1 *temp, _2 *temp, _3 *temp, _4 *temp, _5 *temp);
	cout << "*********************************************************************************" << endl;
}
//正解
void f(double _1, double _2, double _3, double _4, double _5) {
	double s1, s2, s3, s4, s5, s23, s234;
	double c1, c2, c3, c4, c5, c23, c234;
	s1 = sin(_1);
	s2 = sin(_2);
	s3 = sin(_3);
	s4 = sin(_4);
	s5 = sin(_5);
	s23 = sin(_2 + _3);
	s234 = sin(_2 + _3 + _4);
	c1 = cos(_1);
	c2 = cos(_2);
	c3 = cos(_3);
	c4 = cos(_4);
	c5 = cos(_5);
	c23 = cos(_2 + _3);
	c234 = cos(_2 + _3 + _4);
	nx = -s1*s5 + c1*c5*c234;
	ny = c1*s5 + c5*s1*c234;
	nz = c5*s234;
	ox = -c5*s1 - c1*s5*c234;
	oy = c1*c5 - s1*s5*c234;
	oz = s5*s234;
	ax = -c1*s234;
	ay = -s1*s234;
	az = c234;
	px = c1*(a[1] + a[2] * c2 + a[3] * c23) - d[5] * c1*s234;
	py = s1*(a[1] + a[2] * c2 + a[3] * c23) - d[5] * s1*s234;
	pz = d[1] + a[2] * s2 + a[3] * s23 + d[5] * c234;
	cout << "正解出的姿态矩阵为：" << endl;
	printf("%.2lf\t%.2lf\t%.2lf\t%.2lf\n", nx, ox, ax, px);
	printf("%.2lf\t%.2lf\t%.2lf\t%.2lf\n", ny, oy, ay, py);
	printf("%.2lf\t%.2lf\t%.2lf\t%.2lf\n", nz, oz, az, pz);
	printf("%.2lf\t%.2lf\t%.2lf\t%.2lf\n", 0.0, 0.0, 0.0, 1.0);
}
//先根据输入的角度正解算出末端姿态，再根据末端姿态来反解每个关节的角度
void begin() {
	double _1, _2, _3, _4, _5;
	cout << "请输入五个关节的角度：" << endl;
	cin >> _1 >> _2 >> _3 >> _4 >> _5;
	//_1 = 80; _2 = 0; _3 = -60; _4 = 10; _5 = 70;
	f(_1 / 180 * PI, _2 / 180 * PI, _3 / 180 * PI, _4 / 180 * PI, _5 / 180 * PI);
	calculate_angle_1();
}
//把角度纠正在 (-PI~PI] 区间
void fix(double &x) {
	while (x > PI) {
		x -= 2 * PI;
	}
	while (x <= -PI) {
		x += 2 * PI;
	}
}
int main() {

	while (true)
	{
		begin();
	}

	return 0;
}