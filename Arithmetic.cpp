#include "Arithmetic.h"

#include "Container.h"

#include <Windows.h>

const double PI = 3.141592653;

int gcd(int a, int b)
{
	int t;
	while (1) 
	{
		t = a % b;
		if (t == 0)
			return b;
		a = b;
		b = t;
	}
}
float Lerp(float X, float Y, float T) { return X * (1.0f - T) + (Y * T); }
Vector<int> SieveOfEratosthenes(int n, int Start)
{
	// Create a boolean array
	// "prime[0..n]" and initialize
	// all entries it as true.
	// A value in prime[i] will
	// finally be false if i is
	// Not a prime, else true.
	bool* prime = new bool[n + 1];
	size_t Size = sizeof(bool) * (n + 1);
	memset(prime, true, Size);

	for (int p = 2; p * p <= n; p++)
	{
		// If prime[p] is not changed,
		// then it is a prime
		if (prime[p] == true)
		{
			// Update all multiples
			// of p greater than or
			// equal to the square of it
			// numbers which are multiple
			// of p and are less than p^2
			// are already been marked.
			for (int i = p * p; i <= n; i += p)
				prime[i] = false;
		}
	}

	Vector<int> Return;
	// Print all prime numbers
	for (int p = 2; p <= n; p++)
		if (prime[p] && p >= Start)
			Return.Add(p);
	return Return;
}

Vector2d::Vector2d() : X(0), Y(0) { }
Vector2d::Vector2d(double X, double Y) : X(X), Y(Y) {}

double Vector2d::Magnitude() { return sqrt(X * X + Y * Y); }
double Vector2d::Angle() { return atan(Magnitude()); }

Vector2d& Vector2d::operator+=(const Vector2d& Other)
{
	X += Other.X;
	Y += Other.Y;
	return *this;
}
Vector2d& Vector2d::operator-=(const Vector2d& Other)
{
	X -= Other.X;
	Y -= Other.Y;
	return *this;
}
Vector2d& Vector2d::operator/=(double Other)
{
	X /= Other;
	Y /= Other;
	return *this;
}
Vector2d& Vector2d::operator*=(double Other)
{
	X *= Other;
	Y *= Other;
	return *this;
}
Vector2d& Vector2d::operator-()
{
	X = -X;
	Y = -Y;
	return *this;
}

Vector2d operator+(const Vector2d& One, const Vector2d& Two) { return { One.X + Two.X, One.Y + Two.Y }; }
Vector2d operator-(const Vector2d& One, const Vector2d& Two) { return { One.X - Two.X, One.Y - Two.Y }; }

Vector2d operator*(const Vector2d& One, double Two) { return { One.X * Two, One.Y * Two }; }
Vector2d operator*(double One, const Vector2d& Two) { return { Two.X * One, Two.Y * One }; }
double operator*(const Vector2d& One, const Vector2d& Two) { return One.X * Two.X + One.Y * Two.Y; }

Vector2d operator/(const Vector2d& One, double Two) { return { One.X / Two, One.Y / Two }; }
Vector2d operator/(double One, const Vector2d& Two) { return { Two.X / One, Two.Y / One }; }

bool Vector2d::operator==(const Vector2d& Other) const
{
	return Other.X == X && Other.Y == Y;
}
bool Vector2d::operator!=(const Vector2d& Other) const
{
	return !(*this == Other);
}

Vector2d Origin, OPos, CursorLast, OPosLast;
double Zoom = 1;

Vector2d OriginToScreen(const Vector2d& Old, RECT WndRect)
{
	return Vector2d{ (Old.X * 50.0) + OPos.X, (Old.Y * -50.0) + OPos.Y };
}
Vector2d ScreenToOrigin(const Vector2d& Old, RECT WndRect)
{
	return Vector2d{ (Old.X - OPos.X) / 50, (Old.Y - OPos.Y) / -50 };
}