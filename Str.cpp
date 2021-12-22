#include "Str.h"

#include <Windows.h>
#include <ostream>
#include <istream>
#include <sstream>

#include "Arithmetic.h"
#include "AString.h"
#include "Container.h"

using namespace std;

String::String()
{
    _Compacity = 3;
    _Len = 0;
    Text = static_cast<wchar_t*>(malloc(sizeof(wchar_t) * _Compacity));
    memset(Text, 0, sizeof(wchar_t) * _Compacity);
}
String::String(const String& Other) : String()
{
    AssignByChars(const_cast<const wchar_t*>(Other.Text));
}
String::String(const wchar_t* Chars) : String()
{
    AssignByChars(Chars);
}
String::String(const wchar_t Item) : String()
{
    AssignByChars(Item);
}
String::String(const char Item) : String()
{
    AssignByChars((const wchar_t)Item);
}
String::String(const char* Data) : String()
{
    AssignByChars(Data);
}
String::String(const std::basic_string<wchar_t>& Obj) : String(Obj.c_str()) {}
String::String(const int& Data) : String(std::to_wstring(Data)) {}
String::String(const unsigned int& Data) : String(std::to_wstring(Data)) {}
String::String(const long long& Data) : String(std::to_wstring(Data)) {}
String::String(const long& Data) : String(std::to_wstring(Data)) {}
String::String(const unsigned long long& Data) : String(std::to_wstring(Data)) {}
String::String(const float& Data) : String(std::to_wstring(Data)) {}
String::String(const double& Data) : String(std::to_wstring(Data)) {}

uint String::Length() const
{
    return static_cast<uint>(lstrlenW(Text));
}

void String::AssignByChars(const wchar_t* Chars)
{
    int Len = lstrlenW(Chars) + 1;
    _Len = static_cast<uint>(Len);
    _Compacity = _Len + 5;

    free(Text);
    Text = static_cast<wchar_t*>(malloc(sizeof(wchar_t) * _Compacity));
    memset(Text, 0, sizeof(wchar_t) * _Compacity);
    memcpy(Text, Chars, sizeof(wchar_t) * Len);

   /* int Len = lstrlenW(Chars) + 1;
    _Compacity = Len + (Len < 4 ? 5 : 20);
    Text = static_cast<wchar_t*>(realloc(Text, sizeof(wchar_t) * _Compacity));
    memset(Text, 0, sizeof(wchar_t) * _Compacity);
    memcpy(Text, Chars, sizeof(wchar_t) * Len);
    _Len = static_cast<int>(Len);*/
}
void String::AssignByChars(const wchar_t Char)
{
    if (_Compacity == 0)
    {
        _Compacity = 4;
        Text = static_cast<wchar_t*>(realloc(Text, sizeof(wchar_t) * _Compacity));
    }

    memset(Text, 0, sizeof(wchar_t) * _Compacity);
    Text[0] = Char;
    _Len = 1;
}
void String::AssignByChars(const char Char)
{
    if (_Compacity == 0)
    {
        _Compacity = 4;
        Text = static_cast<wchar_t*>(realloc(Text, sizeof(wchar_t) * _Compacity));
    }

    memset(Text, 0, sizeof(wchar_t) * _Compacity);
    Text[0] = Char;
    _Len = 1;
}
void String::AssignByChars(const char* Chars)
{
    AString Str(Chars);
    *this = (String)Str;
}

bool String::Add(const wchar_t& Item)
{
    if ((_Len + 2) > _Compacity)
    {
        _Compacity += 20;
        Text = static_cast<wchar_t*>(realloc(Text, sizeof(wchar_t) * _Compacity));
        memset(&Text[_Len], 0, sizeof(wchar_t) * (_Compacity - _Len));
    }

    if (_Len == 0)
    {
        Text[0] = Item;
        _Len++;
    }
    else
        Text[_Len - 1] = Item;
    _Len++;
    return true;
}
bool String::Add(const char& Item)
{
    return Add((const wchar_t&)Item);
}
bool String::Remove(const wchar_t& Item)
{
    if (!Contains(Item))
        return false;

    uint Index = IndexOf(Item);
    return RemoveAt(Index);
}
bool String::RemoveAt(uint Index)
{
    if (Index >= _Len)
        return false;

    uint FirstSize = (_Len - Index + 1);
    wchar_t* TempText = static_cast<wchar_t*>(malloc(sizeof(wchar_t) * FirstSize));
    memcpy(TempText, &Text[Index + 1], sizeof(wchar_t) * FirstSize);
    memset(&Text[Index], 0, sizeof(wchar_t) * _Compacity);
    memcpy(&Text[Index], TempText, sizeof(wchar_t) * FirstSize);

    _Len--;

    free(TempText);
    return true;
}
bool String::RemoveAllOf(const wchar_t& Item)
{
    wchar_t* NewText = static_cast<wchar_t*>(malloc(sizeof(wchar_t) * _Compacity));
    memset(NewText, 0, sizeof(wchar_t) * _Compacity);

    uint j = 0;
    bool Return = false;
    for (uint i = 0; i < _Len; i++)
    {
        if (Text[i] == Item)
            continue;
        else
        {
            NewText[j] = Text[i];
            j++;
            Return = true;
        }
    }

    memcpy(Text, NewText, sizeof(wchar_t) * _Compacity);
    free(NewText);

    return Return;
}
bool String::RemoveRange(uint Begin, uint End)
{
    wchar_t* TempText = static_cast<wchar_t*>(malloc(sizeof(wchar_t) * (_Len - End + 1)));
    memcpy(TempText, &Text[End + 1], sizeof(wchar_t) * (_Len - End + 1));
    memcpy(&Text[Begin], TempText, sizeof(wchar_t) * (_Compacity - _Len));
    memset(&Text[_Len - 1], 0, sizeof(wchar_t) * (_Compacity - _Len));

    free(TempText);
    return true;
}
void String::Clear()
{
    free(Text);
    Text = nullptr;
    _Compacity = _Len = 0;
}

const wchar_t* String::ConstWchar() const
{
    return Text;
}
String String::SubString(uint Begin, uint End) const
{
    String Return;
    wchar_t*& Text = Return.Text;
   
    uint Comp = End - Begin + 5;
    uint Size = End - Begin + 1;

    free(Text);
    Text = static_cast<wchar_t*>(malloc(sizeof(wchar_t) * Comp));
    memset(Text, 0, sizeof(wchar_t) * Comp);

    memcpy(Text, &this->Text[Begin], sizeof(wchar_t) * (Size));

    return Return;
}

wchar_t String::operator[](uint Index) const
{
    if (Index >= _Len)
        return 0;

    return Text[Index];
}

String String::FilterToNum(bool NegitiveNumbers, bool Decimals) const
{
    String Base = *this;

    if (Base == L"")
        return L"0";

    String Return = L"";
    bool UsedDec = false;
    if (Base._Len < 2)
        Base += L't'; //T is a disregard character, it is used to keep the next evaluation from throwing an exception. The array must have 2 elements, and if it has 1, then it adds "t" to the end. If the len is 0, then it the program returns 0. If it is >= 2, then the block is skiped.

    if ((Decimals && NegitiveNumbers) && (Base[0] == L'-' && Base[1] == L'.'))
    {
        UsedDec = true;
        Return += L"-0.";
    }
    else if (NegitiveNumbers && Base[0] == '-')
    {
        Return += '-';
    }
    else if (Decimals && Base[0] == '.')
    {
        UsedDec = true;
        Return += L"0.";
    }

    for (uint i = 0; i < Base._Len; i++)
    {
        wchar_t Item = Base[i];

        if (Item == L'.' && Decimals && !UsedDec)
        {
            Return += '.';
            UsedDec = true;
        }
        if (Item >= L'0' && Item <= L'9')
            Return += Item;
    }

    if (Return == L"" || Return == L"-0." || Return == L"0.")
        Return = L"0";

    return Return;
}
int String::ToInt() const
{
    String Value = FilterToNum(true, false);
    wstringstream Temp = wstringstream((wstring)Value);

    int Return = 0;
    Temp >> Return;
    return Return;
}
unsigned int String::ToUInt() const
{
    String Value = FilterToNum(false, false);
    wstringstream Temp = wstringstream((wstring)Value);

    unsigned int Return = 0;
    Temp >> Return;
    return Return;
}
long long String::ToLong() const
{
    String Value = FilterToNum(true, false);
    wstringstream Temp = wstringstream((wstring)Value);

    long long Return = 0;
    Temp >> Return;
    return Return;
}
unsigned long long String::ToULong() const
{
    String Value = FilterToNum(false, false);
    wstringstream Temp = wstringstream((wstring)Value);

    ulong Return = 0;
    Temp >> Return;
    return Return;
}
double String::ToDouble() const
{
    String Value = FilterToNum(true, true);
    wstringstream Temp = wstringstream((wstring)Value);

    double Return = 0;
    Temp >> Return;
    return Return;
}
float String::ToFloat() const
{
    String Value = FilterToNum(true, true);

    wstringstream Temp = wstringstream((wstring)Value);

    float Return = 0;
    Temp >> Return;
    return Return;
}

Vector<String> String::Split(wchar_t At) const
{
    const String& Base = *this;
    if (Base == String())
    {
        StringList Return;
        Return.Add(Base);

        return Return;
    }

    int Len = Base.CountOf(At) + 1;

    if (Len == 1)
    {
        Vector<String> returnT;
        returnT.Add(Base);
        return returnT;
    }

    Vector<String> Return;
    String Temp = L"";
    int Returni = 0;

    for (uint i = 0; i < Base.Length(); i++)
    {
        wchar_t Item = Base[i];
        if (Item == At)
        {
            Return.Add(Temp);
            Temp = L"";
            Returni++;
        }
        else
            Temp += Item;
    }

    Return.Add(Temp);
    return Return;
}
String String::Combine(Vector<String> Parts, wchar_t CombineChar)
{
    String Return = L"";
    uint LastIndex = Parts.Size - 1;
    for (uint i = 0; i < Parts.Size; i++)
    {
        Return += Parts[i];
        if (i != LastIndex)
            Return += CombineChar;
    }
    return Return;
}
String String::ReplaceChar(wchar_t Replace, wchar_t With) const
{
    Vector<String> Parts = Split(Replace);
    String Return = String::Combine(Parts, With);

    return Return;
}
String String::ReplaceAll(wchar_t With) const
{
    String Return;
    for (uint i = 0; i < _Len; i++)
        Return += With;

    return Return;
}
String String::ShiftEachChar(int By) const
{
    String Return;
    for (int i = 0; i < lstrlenW(Text); i++)
        Return += (wchar_t)(Text[i] + By);

    return Return;
}

String String::ToLower() const
{
    String Return = *this;

    for (uint i = 0; i < _Len; i++)
    {
        wchar_t Item = Return.Text[i];
        if (Item >= L'A' && Item <= L'Z')
            Return.Text[i] = (wchar_t)(Item + 32);
    }

    return Return;
}
String String::ToUpper() const
{
    String Return = *this;

    for (uint i = 0; i < _Len; i++)
    {
        wchar_t Item = Return.Text[i];
        if (Item >= L'a' && Item <= L'z')
            Return.Text[i] = (wchar_t)(Item - 32);
    }

    return Return;
}
String String::RemoveWhiteSpace() const
{
    const String Value = *this;
    String Return;
    for (uint i = 0; i < _Len; i++)
    {
        wchar_t item = Value[i];
        if (item >= 0 && item <= ' ')
            continue;
        Return += item;
    }

    return Return;
}
String String::RemoveFromFront(wchar_t Item) const
{
    String Return;

    bool Flag = true;
    for (uint i = 0; i < _Len; i++)
    {
        if (Flag && Text[i] == Item)
            continue;
        else
        {
            Flag = false;
            Return += Text[i];
        }
    }

    return Return;
}
String String::RemoveFromEnd(wchar_t Item) const
{
    if (!Contains(Item))
        return *this;

    int TargetIndex = static_cast<int>(_Len);
    bool Flag = false;

    if (_Len == 0)
        return L"";

    for (int i = static_cast<int>(_Len) - 1; i >= 0; i--)
    {
        if (Text[i] == Item)
            continue;
        else if (!Flag)
        {
            TargetIndex = i;
            Flag = true;
        }
    }

    if (TargetIndex < 0 || TargetIndex >= static_cast<int>(_Len))
        return String();

    if (TargetIndex == static_cast<int>(_Len) && Text[_Len - 1] != Item)
        return *this;
    else
        return SubString(0, TargetIndex + 1);
}
int String::TabIndex() const
{
    int Return = 0;
    String Temp = String(*this);
    while (Temp.BeginsWith(L'\t'))
    {
        Return++;
        Temp.RemoveAt(0u);
    }

    return Return;
}

bool String::BeginsWith(const String& Value) const
{
    bool Return = true;

    String Parent = *this;

    for (uint i = 0; i < Value.Length(); i++)
    {
        if (Parent[i] == Value[i])
            Return &= true;
        else
            Return &= false;
    }
    return Return;
}
bool String::BeginsWith(const wchar_t Value) const
{
    return operator[](0u) == Value;
}
bool String::Contains(const wchar_t& Delem) const
{
    for (uint i = 0; i < _Len; i++)
    {
        if (operator[](i) == Delem)
            return true;
    }
    return false;
}
bool String::Contains(String Obj) const
{
    if (*this == Obj)
        return true;

    wchar_t FirstChar = (Obj.Length() == 0 ? L'\0' : Obj[0u]);
    for (uint i = 0u; i < _Len; i++)
    {
        wchar_t Current = this->operator[](i);

        if (Current == FirstChar)
        {
            bool Return = true;
            for (uint j = i + 1, m = 0u; j < _Len && m < Obj._Len; j++, m++)
                Return &= this->operator[](j) == Obj[m];

            if (Return)
                return true;
        }
    }

    return false;
}
uint String::CountOf(const wchar_t& Delemeter) const
{
    const String& Base = *this;
    uint Return = 0;
    for (uint i = 0; i < _Len; i++)
    {
        if (Base[i] == Delemeter)
            Return++;
    }
    return Return;
}
uint String::IndexOf(const wchar_t& Item) const
{
    for (uint i = 0; i < _Len; i++)
    {
        if (operator[](i) == Item)
            return i;
    }

    return uintmax;
}

uint String::GetStrLen(const wchar_t*& Chars)
{
    return lstrlenW(Chars);
}
uint String::GetStrLen(const char*& Chars)
{
    return lstrlenA(Chars);
}

String& String::operator+=(const wchar_t New)
{
    this->Add(New);

    return *this;
}
String& String::operator+=(const String& Two)
{
    if (_Len + Two._Len < _Compacity)
    {
        uint Size = _Len + Two._Len;

        memcpy(&Text[lstrlenW(Text)], Two.Text, sizeof(wchar_t) * Two._Len);
        _Len = Size;
    }
    else
    {
        uint Size = _Len + Two._Len, Comp = Size + 20;

        Text = static_cast<wchar_t*>(realloc(Text, sizeof(wchar_t) * Comp));
        memset(&Text[Size], 0, sizeof(wchar_t) * (Comp - Size));

        memcpy(&Text[lstrlenW(Text)], Two.Text, sizeof(wchar_t) * Two._Len);

        _Compacity = Comp;
        _Len = Size;
    }
    

    return *this;
}
String operator+(const String& One, const String& Two)
{
    wchar_t* Text;
    uint Comp = Two._Len + One._Len + 1;
    uint Size = Two._Len + One._Len;

    Text = static_cast<wchar_t*>(malloc(sizeof(wchar_t) * Comp));
    memset(Text, 0, sizeof(wchar_t) * Comp);
    memcpy(Text, One.Text, sizeof(wchar_t) * One._Len);
    memcpy(&Text[lstrlenW(One.Text)], Two.Text, sizeof(wchar_t) * Two._Len);

    String Return;
    free(Return.Text);
    Return.Text = Text;
    Return._Compacity = Comp;
    Return._Len = Size;

    return Return;
}
String operator+(const String& One, const wchar_t Two)
{
    wchar_t* Text;
    uint Comp = 1 + One._Len + 1;
    uint Size = 1 + One._Len;

    Text = static_cast<wchar_t*>(malloc(sizeof(wchar_t) * Comp));
    memset(Text, 0, sizeof(wchar_t) * Comp);
    memcpy(Text, One.Text, sizeof(wchar_t) * One._Len);
    Text[lstrlenW(One.Text)] = Two;

    String Return;
    free(Return.Text);
    Return.Text = Text;
    Return._Compacity = Comp;
    Return._Len = Size;

    return Return;
}
String operator+(const wchar_t One, const String& Two)
{
    wchar_t* Text;
    uint Comp = Two._Len + 1 + 1;
    uint Size = Two._Len + 1;

    Text = static_cast<wchar_t*>(malloc(sizeof(wchar_t) * Comp));
    memset(Text, 0, sizeof(wchar_t) * Comp);
    Text[0] = One;
    memcpy(&Text[1], Two.Text, sizeof(wchar_t) * Two._Len);

    String Return;
    free(Return.Text);
    Return.Text = Text;
    Return._Compacity = Comp;
    Return._Len = Size;

    return Return;
}

bool operator==(const String& One, const String& Two)
{
    if (lstrlenW(One.Text) != lstrlenW(Two.Text))
        return false;

    bool Return = true;
    for (uint i = 0; i < One._Len; i++)
        Return &= One[i] == Two[i];

    return Return;
}
bool operator!=(const String& One, const String& Two)
{
    return !(One == Two);
}

bool operator==(const String& One, const wchar_t* Two)
{
    if (lstrlenW(One.Text) != lstrlenW(Two))
        return false;

    bool Return = true;
    for (uint i = 0; i < One._Len; i++)
        Return &= One.Text[i] == Two[i];

    return Return;
}
bool operator!=(const String& One, const wchar_t* Two)
{
    return !(One == Two);
}
bool operator==(const wchar_t* One, const String& Two)
{
    return Two == One;
}
bool operator!=(const wchar_t* One, const String& Two)
{
    return !(One == Two);
}

String& String::operator=(const String& Obj)
{
    this->AssignByChars(const_cast<const wchar_t*>(Obj.Text));
    return *this;
}
String& String::operator=(const wchar_t* Obj)
{
    this->AssignByChars(Obj);
    return *this;
}
String& String::operator=(const char* Data)
{
    this->AssignByChars(Data);
    return *this;
}

String::operator std::wstring() const
{
    return std::wstring(Text);
}
String::operator const wchar_t* () const
{
    return Text;
}

wostream& operator<<(wostream& stream, const String& String)
{
    stream << String.Text;
    return stream;
}
ostream& operator<<(basic_ostream<char>& stream, const String& String)
{
    AString This = AString(String);
    stream << This;
    return stream;
}

std::wistream& getline(std::wistream& Istream, String& Str)
{
    wstring Val;
    getline(Istream, Val);
    Str = String(Val);
    return Istream;
}