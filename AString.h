#pragma once

#include <iostream>

/*
*
* Str.h
*
* Defines the class AString, which is used to define and modify text. This is my own version, not ANSI.
*
*/

template<typename T>
class Vector;

using uint = unsigned int;

using std::string;
class AString
{
private:
	char* Text;
	uint _Len = 0, _Compacity = 3;

	AString FilterToNum(bool NegitiveNumbers, bool Decimals) const;
	void AssignByChars(const char* Chars);
	void AssignByChars(const char Char);
public:
	AString();
	AString(const AString& Other);
	AString(const char* Data);
	AString(const char Data);
	AString(const std::basic_string<char>& Obj);
	AString(const int& Data);
	AString(const unsigned int& Data);
	AString(const long long& Data);
	AString(const long& Data);
	AString(const unsigned long long& Data);
	AString(const float& Data);
	AString(const double& Data);
	AString(const class String& Data);
	~AString()
	{
		Clear();
	}

	bool Add(const char& Item);
	bool Remove(const char& Item);
	bool RemoveAt(uint Index);
	bool RemoveAllOf(const char& Item);
	bool RemoveRange(uint Begin, uint End);
	void Clear();

	AString SubString(uint Begin) const
	{
		return SubString(Begin, _Len - 1);
	}
	AString SubString(uint Begin, uint End) const;
	const char* ConstWchar() const;

	uint Length() const;
	uint IndexOf(const char& Item) const;
	bool Contains(const char& Item) const;
	bool Contains(AString Other) const;
	uint CountOf(const char& Item) const;

	char operator[](int Index) const
	{
		return operator[]((unsigned int)Index);
	}
	char operator[](uint Index) const;

	AString ToLower() const;
	AString ToUpper() const;

	int ToInt() const;
	uint ToUInt() const;
	long long ToLong() const;
	unsigned long long ToULong() const;
	double ToDouble() const;
	float ToFloat() const;
	bool ToBool() const
	{
		return *this == "True";
	}

	Vector<AString> Split(char At) const;
	static AString Combine(Vector<AString> Parts, char CombineChar);
	AString ReplaceChar(char Replace, char With) const;
	AString ReplaceAll(char With) const;
	AString ShiftEachChar(int By) const;

	bool BeginsWith(const char Value) const;
	bool BeginsWith(const AString& Value) const;

	AString RemoveWhiteSpace() const;
	AString RemoveFromFront(char Item) const;
	AString RemoveFromEnd(char Item) const;
	int TabIndex() const;

	static uint GetStrLen(const char*& Chars);

	AString& operator+=(const char Two);
	AString& operator+=(const AString& Two);
	friend AString operator+(const AString& One, const char Two);
	friend AString operator+(const AString& One, const AString& Two);
	friend AString operator+(const char One, const AString& Two);

	friend bool operator==(const AString& One, const AString& Two);
	friend bool operator!=(const AString& One, const AString& Two);

	friend bool operator==(const AString& One, const char* Two);
	friend bool operator!=(const AString& One, const char* Two);
	friend bool operator==(const char* One, const AString& Two);
	friend bool operator!=(const char* One, const AString& Two);

	AString& operator=(const AString& Obj);
	AString& operator=(const char* Obj);

	operator std::string() const;
	explicit operator class String() const;
	explicit operator const char*() const;

	friend std::ostream& operator<<(std::ostream& stream, const AString& string);
};

using AStringList = Vector<AString>;


std::istream& getline(std::istream& Istream, AString& Str);
#ifdef _FILESYSTEM_
inline bool create_directory(const AString& Path)
{
	return std::filesystem::create_directory((string)Path);
}
inline bool exists(const AString& Path)
{
	return std::filesystem::exists((string)Path);
}
#endif
