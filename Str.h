#pragma once

#include <iostream>

/*
*
* Str.h
*
* Defines the class String, which is used to define and modify text. This is my own version, not ANSI.
*
*/

template<typename T>
class Vector;

using uint = unsigned int;

using std::wstring;
class String
{
private:
	wchar_t* Text;
	uint _Len = 0, _Compacity = 3;

	String FilterToNum(bool NegitiveNumbers, bool Decimals) const;
	void AssignByChars(const wchar_t* Chars);
	void AssignByChars(const wchar_t Char);
	void AssignByChars(const char* Chars);
	void AssignByChars(const char Char);
public:
	String();
	String(const String& Other);
	String(const wchar_t* Data);
	String(const wchar_t Data);
	String(const char Data);
	String(const char* Data);
	String(const std::basic_string<wchar_t>& Obj);
	String(const int& Data);
	String(const unsigned int& Data);
	String(const long long& Data);
	String(const long& Data);
	String(const unsigned long long& Data);
	String(const float& Data);
	String(const double& Data);
	~String()
	{
		Clear();
	}

	friend class AString;

	bool Add(const wchar_t& Item);
	bool Add(const char& Item);
	bool Remove(const wchar_t& Item);
	bool RemoveAt(uint Index);
	bool RemoveAllOf(const wchar_t& Item);
	bool RemoveRange(uint Begin, uint End);
	void Clear();

	String SubString(uint Begin) const
	{
		return SubString(Begin, _Len - 1);
	}
	String SubString(uint Begin, uint End) const;
	const wchar_t* ConstWchar() const;

	uint Length() const;
	uint IndexOf(const wchar_t& Item) const;
	bool Contains(const wchar_t& Item) const;
	bool Contains(String Other) const;
	uint CountOf(const wchar_t& Item) const;

	wchar_t operator[](int Index) const
	{
		return operator[]((unsigned int)Index);
	}
	wchar_t operator[](uint Index) const;

	String ToLower() const;
	String ToUpper() const;

	int ToInt() const;
	uint ToUInt() const;
	long long ToLong() const;
	unsigned long long ToULong() const;
	double ToDouble() const;
	float ToFloat() const;
	bool ToBool() const
	{
		return *this == L"True";
	}

	Vector<String> Split(wchar_t At) const;
	static String Combine(Vector<String> Parts, wchar_t CombineChar);
	String ReplaceChar(wchar_t Replace, wchar_t With) const;
	String ReplaceAll(wchar_t With) const;
	String ShiftEachChar(int By) const;

	bool BeginsWith(const wchar_t Value) const;
	bool BeginsWith(const String& Value) const;

	String RemoveWhiteSpace() const;
	String RemoveFromFront(wchar_t Item) const;
	String RemoveFromEnd(wchar_t Item) const;
	int TabIndex() const;

	static uint GetStrLen(const wchar_t*& Chars);
	static uint GetStrLen(const char*& Chars);

	String& operator+=(const wchar_t Two);
	String& operator+=(const String& Two);
	friend String operator+(const String& One, const wchar_t Two);
	friend String operator+(const String& One, const String& Two);
	friend String operator+(const wchar_t One, const String& Two);

	friend bool operator==(const String& One, const String& Two);
	friend bool operator!=(const String& One, const String& Two);

	friend bool operator==(const String& One, const wchar_t* Two);
	friend bool operator!=(const String& One, const wchar_t* Two);
	friend bool operator==(const wchar_t* One, const String& Two);
	friend bool operator!=(const wchar_t* One, const String& Two);

	String& operator=(const String& Obj);
	String& operator=(const wchar_t* Obj);
	String& operator=(const char* Obj);

	operator std::wstring() const;
	explicit operator const wchar_t*() const;

	friend std::wostream& operator<<(std::wostream& stream, const String& string);
	friend std::ostream& operator<<(std::basic_ostream<char>& stream, const String& String);
};

using StringList = Vector<String>;


std::wistream& getline(std::wistream& Istream, String& Str);
#ifdef _FILESYSTEM_
inline bool create_directory(const String& Path)
{
	return std::filesystem::create_directory((wstring)Path);
}
inline bool exists(const String& Path)
{
	return std::filesystem::exists((wstring)Path);
}
#endif
