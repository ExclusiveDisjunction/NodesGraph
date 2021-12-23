#include <Windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

using namespace std;

#include "Arithmetic.h"
#include "Container.h"
#include "Color.h"
#include "Str.h"

LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wn = {0};
	wn.cbClsExtra = 0;
	wn.cbWndExtra = 0;
	wn.lpfnWndProc = WndProc;
	wn.lpszClassName = TEXT("MainClass");
	wn.hInstance = hInstance;
	wn.hbrBackground = NULL;
	wn.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wn.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wn.style = CS_HREDRAW | CS_VREDRAW;

	ATOM ThisAtom = RegisterClass(&wn);
	if (!ThisAtom)
		return 1;

	HWND Window = CreateWindowEx(0l, MAKEINTATOM(ThisAtom), TEXT("Main"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);
	if (!Window)
		return 1;

	ShowWindow(Window, SW_MAXIMIZE);
	UpdateWindow(Window);

	MSG msg;
	while (GetMessageW(&msg, nullptr, 0, 0) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

struct Box
{
	Box(Vector2d Point) : Point(Point) {}

	Vector2d Point;
	AaColor Color = 0xFFFF00FF;
	double Size = 1;
	String Text;
};
struct Connection
{
	Connection(Box* One, Box* Two) : One(One), Two(Two) {}

	Box* One, * Two;
	AaColor Color = 0xFF00FFFF;
};

bool MouseDown = false, IsZooming = false;
Box* CurrentBox;
uint CurrentIndex;

enum Modes
{
	SelectionMode = 0,
	LinkMode = 1,
	MoveMode = 2,
	ConnectionSelMode = 4,
	AddNodeMode = 8
};
int Mode = 0;

Vector<Box*> Boxes;
Vector<Connection*> Connections;

HWND _Base;

void ZoomThis(double By)
{
	double OldZoom = Zoom;
	Zoom += By;

	if (Zoom < 0.1)
		Zoom = 0.1;
	if (Zoom > 12)
		Zoom = 12;

	if (OldZoom != Zoom)
		RedrawWindow(_Base, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASENOW);
}

LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
{
	_Base = Window;

	switch (Message)
	{
	case WM_CREATE:
	{
		return 0;
	}
	case WM_PAINT:
	{
		auto DrawLine = [](Vector2d Origin, Vector2d Axis, HDC Dc, RECT WndRect)
		{
			if (Origin == Axis)
				return;

			Origin = OriginToScreen(Origin, WndRect);
			Axis = OriginToScreen(Axis, WndRect);

			double My = Origin.Y - Axis.Y;
			double Mx = Origin.X - Axis.X;

			if (My == 0) //Horizontal
			{
				POINT P1 = { 0, long(Origin.Y) }, P2 = { WndRect.right, long(Origin.Y) };

				MoveToEx(Dc, P1.x, P1.y, NULL);
				LineTo(Dc, P2.x, P2.y);
			}
			else if (Mx == 0) //Vertical Line
			{
				POINT P1 = { long(Origin.X), 0 }, P2 = { long(Origin.X), WndRect.bottom };

				MoveToEx(Dc, P1.x, P1.y, NULL);
				LineTo(Dc, P2.x, P2.y);
			}
			else
			{
				double M = My / Mx;
				double B = Origin.Y - (M * Origin.X);

				double XIntercept = -B / M;

				if (XIntercept > 0)
				{
					double NewM = 1 / M;
					double NewB = -B / M;

					double YOne = 0;
					double YTwo = (NewM * double(WndRect.bottom)) + NewB;

					MoveToEx(Dc, (int)XIntercept, int(YOne), nullptr);
					LineTo(Dc, WndRect.bottom, int(YTwo));
				}
				else
				{
					double YOne = B;
					double YTwo = (M * double(WndRect.right)) + B;

					MoveToEx(Dc, 0, int(YOne), nullptr);
					LineTo(Dc, WndRect.right, int(YTwo));
				}
			}
		};

		PAINTSTRUCT ps;
		HDC Dc = BeginPaint(_Base, &ps);

		RECT WndRect;
		GetClientRect(_Base, &WndRect);

		HBRUSH Bk = CreateSolidBrush(0x000000);
		FillRect(Dc, &WndRect, Bk);
		DeleteObject(Bk);		

		Vector2d iHat = { Zoom, 0 }, jHat = { 0, Zoom };
		Vector2d TiHat = OriginToScreen(iHat, WndRect), TjHat = OriginToScreen(jHat, WndRect);

		{
			Vector<Vector2d> RenderPoints = Origin;

			HPEN BoxPen = CreatePen(PS_SOLID, (int)(5 * Zoom), 0x00FF00);
			HGDIOBJ OldPen = SelectObject(Dc, BoxPen);
			HGDIOBJ OldBrush = SelectObject(Dc, GetStockObject(NULL_BRUSH));
			for (Vector2d& Obj : RenderPoints)
			{
				Vector2d Tran = Obj.X * iHat + Obj.Y * jHat;
				Tran = OriginToScreen(Tran, WndRect);

				if (!PtInRect(&WndRect, Tran.operator POINT()))
					continue;

				int By = 10 * Zoom;
				Rectangle(Dc, long(Tran.X) - By, long(Tran.Y) - By, long(Tran.X + By), long(Tran.Y + By));
			}

			DeleteObject(BoxPen);
			SelectObject(Dc, OldPen);
			SelectObject(Dc, OldBrush);
		}

		if (!MouseDown)
		{
			double Precis = 1 / ((Zoom + 1) * 16);

			int Rate = int(50 * (1 / Zoom)) + 10;
			RECT BoundRect = { -Rate, -Rate, WndRect.right + Rate, WndRect.bottom + Rate }; //The bounding edge for rendering. If a node is outside of this range, it will not be considered for rendering.

			double BoxSize = 0;

			SetTextColor(Dc, 0xFFFFFF);
			SetBkMode(Dc, TRANSPARENT);
			
			//PUT BOX RENDERING HERE
			{
				for (Box* Item : Boxes)
				{
					AaColor Color = (Item == CurrentBox ? AaColor(0xFFFFFFFF) : Item->Color);
					HBRUSH Base = CreateSolidBrush(Color);
					HPEN Border = CreatePen(PS_SOLID, 3, Color);
					if (Item == CurrentBox)
						SetTextColor(Dc, 0x000000);
					else
						SetTextColor(Dc, 0xFFFFFF);

					SelectObject(Dc, Base);
					SelectObject(Dc, Border);

					Vector2d TopLeft = OriginToScreen(Item->Point - Vector2d(Item->Size, Item->Size) * Zoom, WndRect);
					Vector2d BottomRight = OriginToScreen(Item->Point + Vector2d(Item->Size, Item->Size) * Zoom, WndRect);

					int By = 40 * Zoom;
					BoxSize = 1;
					RECT Rect = { (int)TopLeft.X, (int)TopLeft.Y, (int)BottomRight.X, (int)BottomRight.Y };
					Rectangle(Dc, Rect.left, Rect.top, Rect.right, Rect.bottom);
					DrawTextW(Dc, static_cast<LPCWSTR>(Item->Text), Item->Text.Length(), &Rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

					DeleteObject(Base);
					DeleteObject(Border);
				}
			}

			//PUT LINE CONNECTIONS HERE
			{
				for (Connection* Item : Connections)
				{
					AaColor Color = Item->Color;
					HPEN Border = CreatePen(PS_SOLID, 3 * Zoom, Color);
					SelectObject(Dc, Border);

					Box* Source = Item->One;
					Box* To = Item->Two;
					int BoxSize1 = Source->Size;
					int BoxSize2 = To->Size;

					CubicBezier Bez;
					Bez.Begin = Source->Point - Vector2d(BoxSize1 * Zoom, 0);
					Bez.A1 = Source->Point - Vector2d(7 + BoxSize1 * Zoom, 0);
					Bez.End = To->Point + Vector2d(BoxSize2 * Zoom, 0);
					Bez.A2 = To->Point + Vector2d(7 + BoxSize2 * Zoom, 0);

					bool Begin = true;
					for (double T = 0.0; T <= 1.05; T += 0.05)
					{
						Vector2d Temp = Bez(T);
						Vector2d Tran = OriginToScreen(Temp, WndRect);
						if (Begin)
						{
							MoveToEx(Dc, (int)Tran.X, (int)Tran.Y, nullptr);
							Begin = false;
						}
						else
							LineTo(Dc, (int)Tran.X, (int)Tran.Y);
					}

					DeleteObject(Border);
				}
			}
		}

		SetTextColor(Dc, 0xFFFFFF);
		String CurrentMode;
		switch (Mode)
		{
		case LinkMode:
			CurrentMode = L"Link Mode";
			break;
		case MoveMode:
			CurrentMode = L"Move Mode";
			break;
		case ConnectionSelMode:
			CurrentMode = L"Connection Selection Mode";
			break;
		case AddNodeMode:
			CurrentMode = L"Add Node Mode";
			break;
		default:
			CurrentMode = L"Selection Mode";
			break;
		}
		if (CurrentBox)
			CurrentMode += L", Selected Box #" + String(CurrentIndex + 1);
		TextOut(Dc, 10, 10, static_cast<LPCWSTR>(CurrentMode), CurrentMode.Length());

		EndPaint(_Base, &ps);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		SetFocus(_Base);

		POINT Cursor;
		GetCursorPos(&Cursor);

		if (Mode == AddNodeMode)
		{
			RECT WndRect;
			GetClientRect(Window, &WndRect);

			Vector2d Cur = Vector2d((double)Cursor.x, (double)Cursor.y);
			Vector2d Conv = ScreenToOrigin(Cur, WndRect);

			Box* Item = new Box(Conv);
			Boxes.Add(Item);
			Item->Text = Boxes.Size;

			RedrawWindow(Window, nullptr, nullptr, RDW_ERASENOW | RDW_INVALIDATE);
			return 0;
		}
		else if (Mode == MoveMode && CurrentBox)
		{
			RECT WndRect;
			GetClientRect(Window, &WndRect);

			Vector2d Cur = Vector2d((double)Cursor.x, (double)Cursor.y);
			Vector2d Conv = ScreenToOrigin(Cur, WndRect);

			CurrentBox->Point = Conv;

			RedrawWindow(Window, nullptr, nullptr, RDW_ERASENOW | RDW_INVALIDATE);
			return 0;
		}

		CursorLast = { double(Cursor.x), double(Cursor.y) };
		OPosLast = OPos;

		MouseDown = true;
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		if (IsZooming)
		{
			POINT Cursor;
			GetCursorPos(&Cursor);

			double Distance = sqrt(pow(Cursor.x - CursorLast.X, 2) + pow(Cursor.y - CursorLast.Y, 2));
			Distance /= 32;
			if ((double)Cursor.y < CursorLast.Y)
				Distance *= -1;

			ZoomThis(Distance);
			CursorLast = { (double)Cursor.x, (double)Cursor.y };
		}
		else if (MouseDown)
		{
			if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
			{
				MouseDown = false;
				return 0;
			}

			POINT NewLoc;
			GetCursorPos(&NewLoc);

			double ChangeY = double(NewLoc.y) - CursorLast.Y;
			ChangeY *= 2;
			double ChangeX = double(NewLoc.x) - CursorLast.X;
			ChangeX *= 2;

			OPos = OPosLast + Vector2d(ChangeX, ChangeY);
			RedrawWindow(Window, nullptr, nullptr, RDW_ERASENOW | RDW_INVALIDATE);
		}
		return 0;
	}
	case WM_LBUTTONUP:
	{
		MouseDown = false;
		RedrawWindow(_Base, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASENOW);
		return 0;
	}
	case WM_MOUSEWHEEL:
	case WM_MOUSEHWHEEL:
	{
		double Change = (double)GET_WHEEL_DELTA_WPARAM(wp);
		Change /= (WHEEL_DELTA * 3);
		ZoomThis(Change);
		return 0;
	}
	case WM_KEYDOWN:
	{
		WPARAM Key = wp;

		if (Key == VK_CONTROL)
		{
			IsZooming = true;
			POINT Cursor;
			GetCursorPos(&Cursor);

			CursorLast = { double(Cursor.x), double(Cursor.y) };
		}
		else
		{
			RECT WndRect;
			GetClientRect(_Base, &WndRect);

			switch (Key)
			{
			case L'O':
				OPos = { (double)WndRect.right / 2, (double)WndRect.bottom / 2 };
				Zoom = 1;
				break;
			case VK_SHIFT:
			{
				Mode = AddNodeMode;
				break;
			}
			case L'L': //Link Mode
			{
				if (Mode == LinkMode)
					Mode = 0;
				else
					Mode = LinkMode;
				break;
			}
			case L'M': //Move Mode
			{
				if (Mode == MoveMode)
					Mode = 0;
				else
					Mode = MoveMode;
				break;
			}
			case L'0':
			case L'1':
			case L'2':
			case L'3':
			case L'4':
			case L'5':
			case L'6':
			case L'7':
			case L'8':
			case L'9':
			{
				uint Num = wp - L'0';
				Num--;

				if (Num < 0 || Num >= Boxes.Size)
				{
					MessageBox(Window, TEXT("The range is invalid."), L"Bound Error:", MB_OK | MB_ICONERROR);
					break;
				}

				if (Mode == LinkMode)
				{
					if (CurrentBox == nullptr)
					{
						CurrentIndex = Num;
						CurrentBox = Boxes[Num];
					}
					else
					{
						Box* This = Boxes[Num];
						Connections.Add(new Connection(CurrentBox, This));

						CurrentIndex = 0;
						CurrentBox = nullptr;
					}
				}
				else
				{
					CurrentIndex = Num;
					CurrentBox = Boxes[Num];
				}
			}
			}

			RedrawWindow(_Base, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASENOW);
		}
		return 0;
	}
	case WM_KEYUP:
	{
		switch (wp)
		{
		case VK_CONTROL:
			IsZooming = false;
		case VK_SHIFT:
		{
			if (Mode == AddNodeMode)
				Mode = 0;

			RedrawWindow(_Base, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASENOW);
			break;
		}
		}

		return 0;
	}
	case WM_DESTROY:
	{
		for (Box* Item : Boxes)
			delete Item;
		Boxes.Clear();
		for (Connection* Conn : Connections)
			delete Conn;
		Connections.Clear();

		PostQuitMessage(0);
		return 0;
	}
	default:
		return DefWindowProc(Window, Message, wp, lp);
	}
}