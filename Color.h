#pragma once

#include <Windows.h>
#include <fstream>

class AaColor
{
private:
	using Byte = unsigned char;

	unsigned int _Value;
public:
	AaColor() : _Value(0)
	{

	}
	AaColor(const Byte R, const Byte G, const Byte B)
	{
		Assign(0xFF, R, G, B);
	}
	AaColor(const Byte A, const Byte R, const Byte G, const Byte B)
	{
		Assign(A, R, G, B);
	}
	AaColor(const AaColor& Other)
	{
		Assign(Other.A(), Other.R(), Other.G(), Other.B());
	}
	AaColor(const unsigned int& Other)
	{
		_Value = Other;
	}

	Byte A() const
	{
		return static_cast<Byte>((_Value >> 24) & 0xFF);
	}
	Byte R() const
	{
		return static_cast<Byte>((_Value >> 16) & 0xFF);
	}
	Byte G() const
	{
		return static_cast<Byte>((_Value >> 8) & 0xFF);
	}
	Byte B() const
	{
		return static_cast<Byte>(_Value & 0xFF);
	}	

	void Assign(Byte A, Byte R, Byte G, Byte B)
	{
		_Value = static_cast<unsigned int>((A << 24) | (R << 16) | (G << 8) | B);
	}
	void Assign(unsigned int New)
	{
		_Value = New;
	}

	void Reduce(float By)
	{
		Assign(A(), static_cast<Byte>(R() * By), static_cast<Byte>(G() * By), static_cast<Byte>(B() * By));
	}

	operator COLORREF() const //ColorRef Values are reversed
	{
		return RGB(R(), G(), B());
	}
#ifdef _GDIPLUS_H
	explicit operator Gdiplus::Color() const
	{
		return Gdiplus::Color(A(), R(), G(), B());
	}
#endif

	std::wofstream& operator<<(std::wofstream& OutFile) const
	{
		OutFile << _Value;
		return OutFile;
	}
	std::wifstream& operator>>(std::wifstream& InFile)
	{
		InFile >> _Value;
		return InFile;
	}
	std::ofstream& operator<<(std::ofstream& OutFile) const
	{
		OutFile << _Value;
		return OutFile;
	}
	std::ifstream& operator>>(std::ifstream& InFile)
	{
		InFile >> _Value;
		return InFile;
	}
};