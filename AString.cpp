#include "AString.h"

#include "Str.h"

#include <Windows.h>
#include <ostream>
#include <istream>
#include <sstream>
#include <string>

#include "Container.h"

using namespace std;

AString::AString()
{
    _Compacity = 3;
    _Len = 0;
    Text = static_cast<char*>(malloc(sizeof(char) * _Compacity));
    memset(Text, 0, sizeof(char) * _Compacity);
}
AString::AString(const AString& Other) : AString()
{
    AssignByChars(const_cast<const char*>(Other.Text));
}
AString::AString(const char* Chars) : AString()
{
    AssignByChars(Chars);
}
AString::AString(const char Item) : AString()
{
    AssignByChars(Item);
}
AString::AString(const std::basic_string<char>& Obj) : AString(Obj.c_str()) {}
AString::AString(const int& Data) : AString(std::to_string(Data)) {}
AString::AString(const unsigned int& Data) : AString(std::to_string(Data)) {}
AString::AString(const long long& Data) : AString(std::to_string(Data)) {}
AString::AString(const long& Data) : AString(std::to_string(Data)) {}
AString::AString(const unsigned long long& Data) : AString(std::to_string(Data)) {}
AString::AString(const float& Data) : AString(std::to_string(Data)) {}
AString::AString(const double& Data) : AString(std::to_string(Data)) {}
AString::AString(const String& Data)
{
    const wchar_t* Ret = Data.ConstWchar();

    _Compacity = Data.Length() + 3;
    _Len = Data.Length();
    Text = static_cast<char*>(malloc(sizeof(char) * _Compacity));
    memset(Text, 0, sizeof(char) * _Compacity);

    for (uint i = 0; i < _Len; i++)
    {
        if (Ret[i] > 255 || Ret[i] < -255)
            Text[i] = '?';
        else
            Text[i] = (char)Ret[i];
    }
}

uint AString::Length() const
{
    return static_cast<uint>(lstrlenA(Text));
}

void AString::AssignByChars(const char* Chars)
{
    int Len = lstrlenA(Chars) + 1;
    _Len = static_cast<uint>(Len);
    _Compacity = _Len + 5;

    free(Text);
    Text = static_cast<char*>(malloc(sizeof(char) * _Compacity));
    memset(Text, 0, sizeof(char) * _Compacity);
    memcpy(Text, Chars, sizeof(char) * Len);

   /* int Len = lstrlenW(Chars) + 1;
    _Compacity = Len + (Len < 4 ? 5 : 20);
    Text = static_cast<char*>(realloc(Text, sizeof(char) * _Compacity));
    memset(Text, 0, sizeof(char) * _Compacity);
    memcpy(Text, Chars, sizeof(char) * Len);
    _Len = static_cast<int>(Len);*/
}
void AString::AssignByChars(const char Char)
{
    if (_Compacity == 0)
    {
        _Compacity = 4;
        Text = static_cast<char*>(realloc(Text, sizeof(char) * _Compacity));
    }

    memset(Text, 0, sizeof(char) * _Compacity);
    Text[0] = Char;
    _Len = 1;
}

bool AString::Add(const char& Item)
{
    if ((_Len + 2) > _Compacity)
    {
        _Compacity += 20;
        Text = static_cast<char*>(realloc(Text, sizeof(char) * _Compacity));
        memset(&Text[_Len], 0, sizeof(char) * (_Compacity - _Len));
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
bool AString::Remove(const char& Item)
{
    if (!Contains(Item))
        return false;

    uint Index = IndexOf(Item);
    return RemoveAt(Index);
}
bool AString::RemoveAt(uint Index)
{
    if (Index >= _Len)
        return false;

    uint FirstSize = (_Len - Index + 1);
    char* TempText = static_cast<char*>(malloc(sizeof(char) * FirstSize));
    memcpy(TempText, &Text[Index + 1], sizeof(char) * FirstSize);
    memset(&Text[Index], 0, sizeof(char) * _Compacity);
    memcpy(&Text[Index], TempText, sizeof(char) * FirstSize);

    _Len--;

    free(TempText);
    return true;
}
bool AString::RemoveAllOf(const char& Item)
{
    char* NewText = static_cast<char*>(malloc(sizeof(char) * _Compacity));
    memset(NewText, 0, sizeof(char) * _Compacity);

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

    memcpy(Text, NewText, sizeof(char) * _Compacity);
    free(NewText);

    return Return;
}
bool AString::RemoveRange(uint Begin, uint End)
{
    char* TempText = static_cast<char*>(malloc(sizeof(char) * (_Len - End + 1)));
    memcpy(TempText, &Text[End + 1], sizeof(char) * (_Len - End + 1));
    memcpy(&Text[Begin], TempText, sizeof(char) * (_Compacity - _Len));
    memset(&Text[_Len - 1], 0, sizeof(char) * (_Compacity - _Len));

    free(TempText);
    return true;
}
void AString::Clear()
{
    free(Text);
    Text = nullptr;
    _Compacity = _Len = 0;
}

const char* AString::ConstWchar() const
{
    return Text;
}
AString AString::SubString(uint Begin, uint End) const
{
    AString Return;
    char*& Text = Return.Text;
   
    uint Comp = End - Begin + 5;
    uint Size = End - Begin + 1;

    free(Text);
    Text = static_cast<char*>(malloc(sizeof(char) * Comp));
    memset(Text, 0, sizeof(char) * Comp);

    memcpy(Text, &this->Text[Begin], sizeof(char) * (Size));

    return Return;
}

char AString::operator[](uint Index) const
{
    if (Index >= _Len)
        return 0;

    return Text[Index];
}

AString AString::FilterToNum(bool NegitiveNumbers, bool Decimals) const
{
    AString Base = *this;

    if (Base == "")
        return "0";

    AString Return = "";
    bool UsedDec = false;
    if (Base._Len < 2)
        Base += 't'; //T is a disregard character, it is used to keep the next evaluation from throwing an exception. The array must have 2 elements, and if it has 1, then it adds "t" to the end. If the len is 0, then it the program returns 0. If it is >= 2, then the block is skiped.

    if ((Decimals && NegitiveNumbers) && (Base[0] == '-' && Base[1] == '.'))
    {
        UsedDec = true;
        Return += "-0.";
    }
    else if (NegitiveNumbers && Base[0] == '-')
    {
        Return += '-';
    }
    else if (Decimals && Base[0] == '.')
    {
        UsedDec = true;
        Return += "0.";
    }

    for (uint i = 0; i < Base._Len; i++)
    {
        char Item = Base[i];

        if (Item == '.' && Decimals && !UsedDec)
        {
            Return += '.';
            UsedDec = true;
        }
        if (Item >= '0' && Item <= '9')
            Return += Item;
    }

    if (Return == "" || Return == "-0." || Return == "0.")
        Return = "0";

    return Return;
}
int AString::ToInt() const
{
    AString Value = FilterToNum(true, false);
    stringstream Temp = stringstream((string)Value);

    int Return = 0;
    Temp >> Return;
    return Return;
}
unsigned int AString::ToUInt() const
{
    AString Value = FilterToNum(false, false);
    stringstream Temp = stringstream((string)Value);

    unsigned int Return = 0;
    Temp >> Return;
    return Return;
}
long long AString::ToLong() const
{
    AString Value = FilterToNum(true, false);
    stringstream Temp = stringstream((string)Value);

    long long Return = 0;
    Temp >> Return;
    return Return;
}
unsigned long long AString::ToULong() const
{
    AString Value = FilterToNum(false, false);
    stringstream Temp = stringstream((string)Value);

    unsigned long long Return = 0;
    Temp >> Return;
    return Return;
}
double AString::ToDouble() const
{
    AString Value = FilterToNum(true, true);
    stringstream Temp = stringstream((string)Value);

    double Return = 0;
    Temp >> Return;
    return Return;
}
float AString::ToFloat() const
{
    AString Value = FilterToNum(true, true);

    stringstream Temp = stringstream((string)Value);

    float Return = 0;
    Temp >> Return;
    return Return;
}

Vector<AString> AString::Split(char At) const
{
    const AString& Base = *this;
    if (Base == AString())
    {
        AStringList Return;
        Return.Add(Base);

        return Return;
    }

    int Len = Base.CountOf(At) + 1;

    if (Len == 1)
    {
        Vector<AString> returnT;
        returnT.Add(Base);
        return returnT;
    }

    Vector<AString> Return;
    AString Temp = "";
    int Returni = 0;

    for (uint i = 0; i < Base.Length(); i++)
    {
        char Item = Base[i];
        if (Item == At)
        {
            Return.Add(Temp);
            Temp = "";
            Returni++;
        }
        else
            Temp += Item;
    }

    Return.Add(Temp);
    return Return;
}
AString AString::Combine(Vector<AString> Parts, char CombineChar)
{
    AString Return = "";
    uint LastIndex = Parts.Size - 1;
    for (uint i = 0; i < Parts.Size; i++)
    {
        Return += Parts[i];
        if (i != LastIndex)
            Return += CombineChar;
    }
    return Return;
}
AString AString::ReplaceChar(char Replace, char With) const
{
    Vector<AString> Parts = Split(Replace);
    AString Return = AString::Combine(Parts, With);

    return Return;
}
AString AString::ReplaceAll(char With) const
{
    AString Return;
    for (uint i = 0; i < _Len; i++)
        Return += With;

    return Return;
}
AString AString::ShiftEachChar(int By) const
{
    AString Return;
    for (int i = 0; i < lstrlenA(Text); i++)
        Return += (char)(Text[i] + By);

    return Return;
}

AString AString::ToLower() const
{
    AString Return = *this;

    for (uint i = 0; i < _Len; i++)
    {
        char Item = Return.Text[i];
        if (Item >= 'A' && Item <= 'Z')
            Return.Text[i] = (char)(Item + 32);
    }

    return Return;
}
AString AString::ToUpper() const
{
    AString Return = *this;

    for (uint i = 0; i < _Len; i++)
    {
        char Item = Return.Text[i];
        if (Item >= 'a' && Item <= 'z')
            Return.Text[i] = (char)(Item - 32);
    }

    return Return;
}
AString AString::RemoveWhiteSpace() const
{
    const AString Value = *this;
    AString Return;
    for (uint i = 0; i < _Len; i++)
    {
        char item = Value[i];
        if (item >= 0 && item <= ' ')
            continue;
        Return += item;
    }

    return Return;
}
AString AString::RemoveFromFront(char Item) const
{
    AString Return;

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
AString AString::RemoveFromEnd(char Item) const
{
    if (!Contains(Item))
        return *this;

    int TargetIndex = static_cast<int>(_Len);
    bool Flag = false;

    if (_Len == 0)
        return "";

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
        return AString();

    if (TargetIndex == static_cast<int>(_Len) && Text[_Len - 1] != Item)
        return *this;
    else
        return SubString(0, TargetIndex + 1);
}
int AString::TabIndex() const
{
    int Return = 0;
    AString Temp = AString(*this);
    while (Temp.BeginsWith(L'\t'))
    {
        Return++;
        Temp.RemoveAt(0u);
    }

    return Return;
}

bool AString::BeginsWith(const AString& Value) const
{
    bool Return = true;

    AString Parent = *this;

    for (uint i = 0; i < Value.Length(); i++)
    {
        if (Parent[i] == Value[i])
            Return &= true;
        else
            Return &= false;
    }
    return Return;
}
bool AString::BeginsWith(const char Value) const
{
    return operator[](0u) == Value;
}
bool AString::Contains(const char& Delem) const
{
    for (uint i = 0; i < _Len; i++)
    {
        if (operator[](i) == Delem)
            return true;
    }
    return false;
}
bool AString::Contains(AString Obj) const
{
    if (*this == Obj)
        return true;

    char FirstChar = (Obj.Length() == 0 ? '\0' : Obj[0u]);
    for (uint i = 0u; i < _Len; i++)
    {
        char Current = this->operator[](i);

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
uint AString::CountOf(const char& Delemeter) const
{
    const AString& Base = *this;
    uint Return = 0;
    for (uint i = 0; i < _Len; i++)
    {
        if (Base[i] == Delemeter)
            Return++;
    }
    return Return;
}
uint AString::IndexOf(const char& Item) const
{
    for (uint i = 0; i < _Len; i++)
    {
        if (operator[](i) == Item)
            return i;
    }

    return -1;
}

uint AString::GetStrLen(const char*& Chars)
{
    return lstrlenA(Chars);
}

AString& AString::operator+=(const char New)
{
    this->Add(New);

    return *this;
}
AString& AString::operator+=(const AString& Two)
{
    if (_Len + Two._Len < _Compacity)
    {
        uint Size = _Len + Two._Len;

        memcpy(&Text[lstrlenA(Text)], Two.Text, sizeof(char) * Two._Len);
        _Len = Size;
    }
    else
    {
        uint Size = _Len + Two._Len, Comp = Size + 20;

        Text = static_cast<char*>(realloc(Text, sizeof(char) * Comp));
        memset(&Text[Size], 0, sizeof(char) * (Comp - Size));

        memcpy(&Text[lstrlenA(Text)], Two.Text, sizeof(char) * Two._Len);

        _Compacity = Comp;
        _Len = Size;
    }
    

    return *this;
}
AString operator+(const AString& One, const AString& Two)
{
    char* Text;
    uint Comp = Two._Len + One._Len + 1;
    uint Size = Two._Len + One._Len;

    Text = static_cast<char*>(malloc(sizeof(char) * Comp));
    memset(Text, 0, sizeof(char) * Comp);
    memcpy(Text, One.Text, sizeof(char) * One._Len);
    memcpy(&Text[lstrlenA(One.Text)], Two.Text, sizeof(char) * Two._Len);

    AString Return;
    free(Return.Text);
    Return.Text = Text;
    Return._Compacity = Comp;
    Return._Len = Size;

    return Return;
}
AString operator+(const AString& One, const char Two)
{
    char* Text;
    uint Comp = 1 + One._Len + 1;
    uint Size = 1 + One._Len;

    Text = static_cast<char*>(malloc(sizeof(char) * Comp));
    memset(Text, 0, sizeof(char) * Comp);
    memcpy(Text, One.Text, sizeof(char) * One._Len);
    Text[lstrlenA(One.Text)] = Two;

    AString Return;
    free(Return.Text);
    Return.Text = Text;
    Return._Compacity = Comp;
    Return._Len = Size;

    return Return;
}
AString operator+(const char One, const AString& Two)
{
    char* Text;
    uint Comp = Two._Len + 1 + 1;
    uint Size = Two._Len + 1;

    Text = static_cast<char*>(malloc(sizeof(char) * Comp));
    memset(Text, 0, sizeof(char) * Comp);
    Text[0] = One;
    memcpy(&Text[1], Two.Text, sizeof(char) * Two._Len);

    AString Return;
    free(Return.Text);
    Return.Text = Text;
    Return._Compacity = Comp;
    Return._Len = Size;

    return Return;
}

bool operator==(const AString& One, const AString& Two)
{
    if (lstrlenA(One.Text) != lstrlenA(Two.Text))
        return false;

    bool Return = true;
    for (uint i = 0; i < One._Len; i++)
        Return &= One[i] == Two[i];

    return Return;
}
bool operator!=(const AString& One, const AString& Two)
{
    return !(One == Two);
}

bool operator==(const AString& One, const char* Two)
{
    if (lstrlenA(One.Text) != lstrlenA(Two))
        return false;

    bool Return = true;
    for (uint i = 0; i < One._Len; i++)
        Return &= One.Text[i] == Two[i];

    return Return;
}
bool operator!=(const AString& One, const char* Two)
{
    return !(One == Two);
}
bool operator==(const char* One, const AString& Two)
{
    return Two == One;
}
bool operator!=(const char* One, const AString& Two)
{
    return !(One == Two);
}

AString& AString::operator=(const AString& Obj)
{
    this->AssignByChars(const_cast<const char*>(Obj.Text));
    return *this;
}
AString& AString::operator=(const char* Obj)
{
    this->AssignByChars(Obj);
    return *this;
}

AString::operator std::string() const
{
    return std::string(Text);
}
AString::operator String() const
{
    const char* Data = Text;

    String Return;
    Return._Compacity = _Compacity;
    Return._Len = _Len;

    free(Return.Text);
    Return.Text = (wchar_t*)malloc(sizeof(wchar_t) * _Len);
    memset(Return.Text, 0, sizeof(wchar_t) * _Len);

    for (uint i = 0; i < _Len; i++)
        Return.Text[i] = (wchar_t)Data[i];

    return Return;
}
AString::operator const char* () const
{
    return Text;
}

ostream& operator<<(ostream& stream, const AString& AString)
{
    stream << AString.Text;
    return stream;
}

std::istream& getline(std::istream& Istream, AString& Str)
{
    string Val;
    getline(Istream, Val);
    Str = AString(Val);
    return Istream;
}