#pragma once

#include <iostream>
#include <Windows.h>

class Vector2d
{
public:
	Vector2d();
	Vector2d(double X, double Y);

	double X;
	double Y;

	double Magnitude();
	double Angle();

	Vector2d& operator+=(const Vector2d& Other);
	Vector2d& operator-=(const Vector2d& Other);
	Vector2d& operator/=(double Other);
	Vector2d& operator*=(double Other);
	Vector2d& operator-();

	friend Vector2d operator+(const Vector2d& One, const Vector2d& Two);
	friend Vector2d operator-(const Vector2d& One, const Vector2d& Two);

	friend Vector2d operator*(const Vector2d& One, double Two);
	friend Vector2d operator*(double One, const Vector2d& Two);
	friend double operator*(const Vector2d& One, const Vector2d& Two);

	friend Vector2d operator/(const Vector2d& One, double Two);
	friend Vector2d operator/(double One, const Vector2d& Two);

	bool operator==(const Vector2d& Other) const;
	bool operator!=(const Vector2d& Other) const;

#ifdef _WINDOWS_
	operator POINT() const
	{
		return POINT{ static_cast<LONG>(X), static_cast<LONG>(Y) };
	}
#endif
};

int gcd(int a, int b);

template<typename T>
class Vector;

Vector<int> SieveOfEratosthenes(int n, int Start = 2);
float Lerp(float X, float Y, float T); //Returns a value that is T between X and Y

using uint = unsigned int;
using ulong = unsigned long long;

constexpr uint uintmax = 4294967295;
extern const double PI;

Vector2d OriginToScreen(const Vector2d& Old, RECT WndRect);
Vector2d ScreenToOrigin(const Vector2d& Old, RECT WndRect);

extern Vector2d Origin, OPos, CursorLast, OPosLast;
extern double Zoom;

class CubicBezier
{
public:
	Vector2d Begin, A1, A2, End;

	Vector2d operator()(double T)
	{
		return (pow(1 - T, 3) * Begin) + 3 * (pow(1 - T, 2) * T * A1) + (3 * (1 - T) * pow(T, 2) * A2) + (pow(T, 3) * End);
	}
};