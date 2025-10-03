/*!
Copyright 2024 Maxim Noltmeer (m.noltmeer@gmail.com)

This file is part of Kobzar Engine.

	Kobzar Engine is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Kobzar Engine is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Kobzar Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------

#include <vcl.h>
#include <windows.h>
#include <process.h>
#include <gdiplus.h>

#pragma hdrstop
#pragma argsused

#include <System.Math.hpp>

#include "eli_interface.h"
#include "..\..\work-functions\MyFunc.h"
#include "..\..\work-functions\Logs.h"

#pragma comment(lib, "gdiplus.lib")

//для визначення сторін прямокутника вздовж яких йде обчислення
#define SDTOPLEFT 0
#define SDTOP 1
#define SDTOPRIGHT 2
#define SDRIGHT 3
#define SDBOTTOMRIGHT 4
#define SDBOTTOM 5
#define SDBOTTOMLEFT 6
#define SDLEFT 7

//Власне повідомлення для роботи з вікном
#define WM_KVL_CLEAR_WINDOW (WM_USER + 1)
#define WM_KVL_DRAW_POLYGON (WM_USER + 2)
#define WM_KVL_DRAW_ELLIPSE (WM_USER + 3)
#define WM_KVL_DRAW_LINE (WM_USER + 4)
#define WM_KVL_DRAW_ARC (WM_USER + 5)
#define WM_KVL_DRAW_RECT (WM_USER + 6)
#define WM_KVL_DRAW_IMAGE (WM_USER + 7)
#define WM_KVL_DRAW_PLATE (WM_USER + 8)
#define WM_KVL_DRAW_TEXT (WM_USER + 9)
#define WM_KVL_DRAW_BUBBLE (WM_USER + 10)
#define WM_KVL_DRAW_BLAST (WM_USER + 11)
#define WM_KVL_DRAW_BALLOON (WM_USER + 12)
#define WM_KVL_DRAW_CLOUD (WM_USER + 13)

ELI_INTERFACE *eIface;

struct LINE
{
  int X1 = 0;
  int Y1 = 0;
  int X2 = 0;
  int Y2 = 0;
  Gdiplus::Color Color;
  int Size = 1;
};

struct ARC
{
  int Left = 0;
  int Top = 0;
  int Width = 0;
  int Height = 0;
  int StartAngle = 0;
  int SweepAngle = 0;
  Gdiplus::Color Color;
  int Size = 1;
};

struct POLYGON
{
  std::vector<Gdiplus::Point> Points;
  Gdiplus::Color Color;
  int Border = 1;
  Gdiplus::Color BorderColor;
  int Shadow = 0;
};

struct ELLIPSE
{
  int Left = 0;
  int Top = 0;
  int Width = 0;
  int Height = 0;
  Gdiplus::Color Color;
  int Border = 1;
  Gdiplus::Color BorderColor;
  int Shadow = 0;
};

struct MYRECT
{
  int Left = 0;
  int Top = 0;
  int Width = 0;
  int Height = 0;
  Gdiplus::Color Color;
  int Border = 1;
  Gdiplus::Color BorderColor;
  int Shadow = 0;
};

struct PLATE : MYRECT
{
  int CornerRadius = 0;
};

struct BUBBLE : PLATE
{
  Gdiplus::PointF Tail;
  int TailWidth = 10;
};

struct BLAST : MYRECT
{
  int MinRayHeight = 10;
  int MaxRayHeight = 50;
  int DynamicRays = 0;
};

struct TEXT
{
  Gdiplus::RectF Rect;
  int UserFont = 0; //1 - використовується шрифт з колекції шрифтів, 0 - системний
  wchar_t FontName[32];
  int FontSize = 10;
  int FontStyle = 0;
  Gdiplus::Color FontColor;
  wchar_t Alignment[8];
  int CenterVerticaly = 0;
  int WordWrap = 0;
};

HINSTANCE DllHinst;
HANDLE WndThread;
HWND WHandle;
unsigned int thID;

//Віртуальне вікно (буфер)
HBITMAP hBitmap = NULL;
HDC hMemDC = NULL;

int WndWidth = 800, WndHeight = 600;
bool FullScreen = 0;
CRITICAL_SECTION cs;  //для безпечного доступу до пам’яті
Gdiplus::PrivateFontCollection *UserFontCollection; //колекція користув. шрифтів для ф-ї eLoadFont()

PLATE CurrentPlate; //для визначення параметрів поточного об'єкту типу Frame
TEXT CurrentText; //для визначення параметрів поточного об'єкту типу Text
BUBBLE CurrentBubble; //для визначення параметрів поточного об'єкту типу Bubble
POLYGON CurrentPoly; //для визначення параметрів поточного об'єкту типу Poly
LINE CurrentLine; //для визначення параметрів поточного об'єкту типу Line
ARC CurrentArc; //для визначення параметрів поточного об'єкту типу Arc
ELLIPSE CurrentEllipse; //для визначення параметрів поточного об'єкту типу Ellipse
MYRECT CurrentRect; //для визначення параметрів поточного об'єкту типу Rect
BLAST CurrentBlast; //для визначення параметрів поточного об'єкту типу Blast
BUBBLE CurrentBalloon; //для визначення параметрів поточного об'єкту типу Balloon
BUBBLE CurrentCloud; //для визначення параметрів поточного об'єкту типу Cloud

//Функція створення віртуального вікна (буфера)
void CreateVirtualWindow(HWND hWnd, int width, int height)
{
  try
	 {
	   HDC hdc = GetDC(hWnd);
	   hMemDC = CreateCompatibleDC(hdc);
	   hBitmap = CreateCompatibleBitmap(hdc, width, height);
	   SelectObject(hMemDC, hBitmap);
	   ReleaseDC(hWnd, hdc);
       SetBkMode(hMemDC, TRANSPARENT); //встановлюємо прозорий фон для тексту
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::CreateVirtualWindow: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

//Функція малювання у віртуальному вікні
void DrawToVirtualWindow(const wchar_t *file, int x, int y)
{
  try
	 {
	   if (!hMemDC)
		 throw Exception("Virtual buffer doesn't exists!");

//Завантаження зображення через GDI+
	   Gdiplus::Graphics graphics(hMemDC);
	   Gdiplus::Image image(file);

	   if (image.GetLastStatus() == Gdiplus::Ok)
		 graphics.DrawImage(&image, x, y);
	   else
		 throw Exception("GDI+ not initialised!");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::DrawToVirtualWindow: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

//Функція малювання тіні для основної фігури
void DrawShadow(Gdiplus::Graphics *graphics,
				Gdiplus::GraphicsPath *main_path,
				int offset = 5,
				Gdiplus::Color color = Gdiplus::Color(100, 0, 0, 0))
{
  try
	 {
	   Gdiplus::GraphicsPath ShadowPath;
	   Gdiplus::Matrix ShadowMatrix;
	   ShadowMatrix.Translate((float)offset, (float)offset);
	   ShadowPath.AddPath(main_path, FALSE);
	   ShadowPath.Transform(&ShadowMatrix);

	   Gdiplus::SolidBrush ShadowBrush(color);
	   graphics->FillPath(&ShadowBrush, &ShadowPath);
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::DrawShadow: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

//Функція копіювання вмісту віртуального вікна в основне
void CopyVirtualToMain(HDC hdc)
{
  try
	 {
	   BitBlt(hdc, 0, 0, WndWidth, WndHeight, hMemDC, 0, 0, SRCCOPY);
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::CopyVirtualToMain: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

//очистка віртуального буферу
void ClearVirtualWindow()
{
  try
	 {
	   if (!hMemDC)
		 throw Exception("Virtual buffer doesn't exists!");

	   RECT rect = {0, 0, WndWidth, WndHeight};
	   FillRect(hMemDC, &rect, (HBRUSH)(COLOR_WINDOW+1));
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::ClearVirtualWindow: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

//допоміжна функція для визначення імені доданого сімейства шрифтів
wchar_t *GetFontFamilyName(const wchar_t *font_file)
{
  wchar_t *res = new wchar_t[LF_FACESIZE];

  try
	 {
	   Gdiplus::PrivateFontCollection temp;
	   Gdiplus::Status status = temp.AddFontFile(font_file);

	   if (status != Gdiplus::Status::Ok)
		 throw Exception("Error adding font from file: " + String(font_file));

	   Gdiplus::FontFamily fm(L"", &temp);
	   int found = 0;
	   temp.GetFamilies(1, &fm, &found);
	   fm.GetFamilyName(res);
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::GetFontFamilyName: " + e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

//допоміжна функція для побудови прямокутника із закругленими кутами
Gdiplus::GraphicsPath *CreateRoundedRectPath(Gdiplus::RectF rect, int radius)
{
  Gdiplus::GraphicsPath *path = new Gdiplus::GraphicsPath();

  try
	 {
	   int x = rect.X, y = rect.Y, w = rect.Width, h = rect.Height;
	   int r = radius * 2;

	   path->AddArc(x, y, r, r, 180, 90);                 //верхній лівий кут
	   path->AddArc(x + w - r, y, r, r, 270, 90);         //верхній правий кут
	   path->AddArc(x + w - r, y + h - r, r, r, 0, 90);   //нижній правий кут
	   path->AddArc(x, y + h - r, r, r, 90, 90);          //нижній лівий кут
	   path->CloseFigure();
	 }
  catch (Exception &e)
	 {
	   if (path)
		 delete path;

	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::CreateRoundedRectPath: " + e.ToString());
	 }

  return path;
}
//---------------------------------------------------------------------------

//визначає кут променя від центру еліпсу до точки
inline double GetRayAngle(double xc, double yc, double a, double b, double xp, double yp)
{
  return std::atan2((yp - yc) / b, (xp - xc) / a);
}
//---------------------------------------------------------------------------

//повертає точку перетину променя з еліпсом
Gdiplus::PointF GetEllipseIntersection(Gdiplus::RectF &rect, const Gdiplus::PointF &target)
{
  float a = rect.Width / 2,
		b = rect.Height / 2,
		xc = rect.X + a,
		yc = rect.Y + b;

  double theta = GetRayAngle(xc, yc, a, b, target.X, target.Y);

  return Gdiplus::PointF(static_cast<float>(xc + a * std::cos(theta)),
						 static_cast<float>(yc + b * std::sin(theta)));
}
//---------------------------------------------------------------------------

//повертає координати сусідніх точок від місця перетину еліпса з променем до точки хвостика
std::pair<Gdiplus::PointF, Gdiplus::PointF> GetIntersectionNeighborsPoints(Gdiplus::RectF &rect,
																		   const Gdiplus::PointF &tail,
																		   int n = 360, // кількість дискретних точок еліпса
																		   int k = 10)  // зсув у кількості точок)
{
  float a = rect.Width / 2,
		b = rect.Height / 2,
		xc = rect.X + a,
		yc = rect.Y + b;

  double theta = GetRayAngle(xc, yc, a, b, tail.X, tail.Y); //кут від центру до точки хвостика
  double step = 1.0 * M_PI / n; //крок по куту 1 градус

  Gdiplus::PointF prev(static_cast<float>(xc + a * std::cos(theta - k * step)),
					   static_cast<float>(yc + b * std::sin(theta - k * step)));

  Gdiplus::PointF next(static_cast<float>(xc + a * std::cos(theta + k * step)),
					   static_cast<float>(yc + b * std::sin(theta + k * step)));

  return {prev, next};
}
//---------------------------------------------------------------------------

//повертає кути до сусідніх точок від точки перетину еліпса з променем до точки хвостика
std::pair<double, double> GetIntersectionNeighborsAngles(Gdiplus::RectF &rect,
														 const Gdiplus::PointF &tail,
														 int n = 360, // кількість дискретних точок еліпса
														 int k = 10)  // зсув у кількості точок)
{
  float a = rect.Width / 2,
		b = rect.Height / 2,
		xc = rect.X + a,
		yc = rect.Y + b;

  auto [prev, next] = GetIntersectionNeighborsPoints(rect, tail, n, k);

  double theta_prev = GetRayAngle(xc, yc, a, b, prev.X, prev.Y);
  double theta_next = GetRayAngle(xc, yc, a, b, next.X, next.Y);

  return {theta_prev, theta_next};
}
//---------------------------------------------------------------------------

//функція для вимірювання розміру тексту
Gdiplus::RectF MeasureTextRect(const std::wstring &text,
							   Gdiplus::RectF rect,
							   int user_font = 0,
							   const std::wstring &font_name = L"Segoe UI",
							   float font_size = 10.0f,
							   Gdiplus::FontStyle font_style = Gdiplus::FontStyleRegular,
							   const std::wstring &align = L"left", // "left", "center", "right"
							   bool center_vertically = false,
							   bool word_wrap = true)
{
  Gdiplus::RectF layout_rect;

  try
	 {
	   if (!hMemDC)
		 throw Exception("Virtual buffer doesn't exists!");

	   Gdiplus::Graphics graphics(hMemDC);
	   std::unique_ptr<Gdiplus::FontFamily> font_family;

	   graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);

	   if (user_font) //використовується користувацький шрифт
		 {
		   font_family.reset(new Gdiplus::FontFamily(font_name.c_str(), UserFontCollection));

		   if (!font_family->IsAvailable())
			 throw Exception("Invalid font family"); //якщо шрифта в колекції не знайшлось
		 }
	   else
		 font_family.reset(new Gdiplus::FontFamily(font_name.c_str()));

	   Gdiplus::Font font(font_family.get(), font_size, font_style, Gdiplus::UnitPixel);

	   Gdiplus::StringFormat format;

//Горизонтальне вирівнювання
	   if (align == L"center")
		 format.SetAlignment(Gdiplus::StringAlignmentCenter);
	   else if (align == L"right")
		 format.SetAlignment(Gdiplus::StringAlignmentFar);
	   else if (align == L"justify")
		 {
		   format.SetAlignment(Gdiplus::StringAlignmentNear);
		   format.SetFormatFlags(format.GetFormatFlags() |
								 Gdiplus::StringFormatFlagsLineLimit |
								 Gdiplus::StringFormatFlagsNoFitBlackBox);
		   format.SetTrimming(Gdiplus::StringTrimmingNone);
		 }
	   else
		 format.SetAlignment(Gdiplus::StringAlignmentNear); //left

//Вертикальне вирівнювання
	   format.SetLineAlignment(center_vertically ? Gdiplus::StringAlignmentCenter : Gdiplus::StringAlignmentNear);

//Перенос рядків
	   if (!word_wrap)
		 format.SetFormatFlags(Gdiplus::StringFormatFlagsNoWrap);

	   if (rect.Width == 0)
		 rect.Width = 10000;
	   else
		 rect.Width++;

	   if (rect.Height == 0)
		 rect.Height = 10000;
	   else
		 rect.Height++;

	   graphics.MeasureString(text.c_str(), -1, &font, rect, &format, &layout_rect);
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::MeasureTextRect: " + e.ToString());
	 }

  return layout_rect;
}
//---------------------------------------------------------------------------

int GenerateRayHeight(int min_ray_h, int max_ray_h)

{
  int res = min_ray_h;

  try
	 {
	   res = Random(max_ray_h);

	   if (res < min_ray_h)
		 res = min_ray_h;
	 }
  catch (Exception &e)
	 {
	   res = min_ray_h;

	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::GenerateRayHeight: " + e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

//Допоміжна функція для побудови полігона, що імітує спалах або вибух
Gdiplus::GraphicsPath *CreateBlastPolygon(Gdiplus::RectF rect,
										  int min_ray_h, //мінімальна висота променю
										  int max_ray_h, //максимальна висота променю
										  bool rand_h = false)

{
  Gdiplus::GraphicsPath *path = new Gdiplus::GraphicsPath();

  try
	 {
	   int max_w = (rect.Width + rect.Height) / 2;
	   int h = max_ray_h; //висота кожного променю
	   int w = max_w; //ширина основи променю

	   path->AddRectangle(rect); //додаємо основний прямокутник

	   struct PointVector
	   {
		 Gdiplus::PointF Point;
		 int VX = 0; //вказують куди йде зсув по координатам (-1, 0, +1)
		 int VY = 0;

		 PointVector(float px, float py, int vx, int vy)
		 {
		   Point = Gdiplus::PointF(px, py);
		   VX = vx;
		   VY = vy;
         }
       };
//будуємо промені
	   std::vector<PointVector> vectors;

	   vectors.push_back(PointVector(rect.X, rect.Y, -1, -1));
	   vectors.push_back(PointVector(rect.X + rect.Width / 2, rect.Y, 0, -1));
	   vectors.push_back(PointVector(rect.X + rect.Width, rect.Y, 1, -1));
	   vectors.push_back(PointVector(rect.X + rect.Width, rect.Y + rect.Height / 2, 1, 0));
	   vectors.push_back(PointVector(rect.X + rect.Width, rect.Y + rect.Height, 1, 1));
	   vectors.push_back(PointVector(rect.X + rect.Width / 2, rect.Y + rect.Height, 0, 1));
	   vectors.push_back(PointVector(rect.X, rect.Y + rect.Height, -1, 1));
	   vectors.push_back(PointVector(rect.X, rect.Y + rect.Height / 2, -1, 0));

	   for (auto v : vectors)
		  {
			Gdiplus::PointF center = GetEllipseIntersection(rect, v.Point); //отримуємо точку на еліпсі

			if (rand_h)
			  {
				h = GenerateRayHeight(min_ray_h, max_ray_h);
				w = GenerateRayHeight(10, max_w);
			  }

			Gdiplus::PointF vertex = Gdiplus::PointF(center.X + h * v.VX,
													 center.Y + h * v.VY); //знаходимо вершину променя

			auto [base1, base2] = GetIntersectionNeighborsPoints(rect, vertex, 360, w);

			Gdiplus::PointF ray[3] = {Gdiplus::PointF(base1),
									  Gdiplus::PointF(vertex),
									  Gdiplus::PointF(base2)
									  };
			path->AddPolygon(ray, 3);
		  }

	   path->Outline(NULL, 1.0f);
	 }
  catch (Exception &e)
	 {
	   if (path)
		 delete path;

	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::CreateBlastPolygon: " + e.ToString());
	 }

  return path;
}
//---------------------------------------------------------------------------

//rect – прямокутник, у якому має розташовуватись хмаринка
//countX/Y – кількість "пузирів" по горизонталі та вертикалі
//chaotic – коефіцієнт "хаотичності" параметрів точок (0.0f – рівна сітка, 1.0f – дуже розкидано)
Gdiplus::GraphicsPath *CreateCloudBase(Gdiplus::RectF &rect, int countX = 3, int countY = 2, float chaotic = 0.25f)
{
  if (rect.Width <= 0 || rect.Height <= 0) return nullptr;

  Gdiplus::GraphicsPath* res = new Gdiplus::GraphicsPath();

  try
	 {
	   float stepX = rect.Width / countX;
	   float stepY = rect.Height / countY;
	   float baseR = min(stepX, stepY) * 5.2f; // радіус еліпсів

	   res->AddEllipse(rect);

//лямбда, що модифікує параметр на основі хаотичного коефіцієнту
	   auto ChaoticParam = [&chaotic](float mltp, float prm)
	   {
		 float chaos = (Random(10) + 1) * chaotic / 100;

		 return float(prm * (mltp + chaos));
	   };

	   /*for (int iy = 0; iy < countY; iy++)
		  {
			for (int ix = 0; ix < countX; ix++)
			   {
				 float cx = rect.X - stepX / 2 + ix * stepX;
				 float cy = rect.Y - stepY / 2 + iy * stepY;

				 float dx = stepX;
				 float dy = stepY;

				 float r = baseR * (0.85f + 0.3f * ((rand() % 200 - 100) / 100.0f));

				 Gdiplus::RectFF ellipse(cx, cy, r, r);

				 res->AddEllipse(ellipse);
			   }
		  }*/

	   std::vector<Gdiplus::PointF> points;

	   points.push_back(Gdiplus::PointF(rect.X, rect.Y));
	   points.push_back(Gdiplus::PointF(rect.X + rect.Width / 2, rect.Y));
	   points.push_back(Gdiplus::PointF(rect.X + rect.Width, rect.Y));
	   points.push_back(Gdiplus::PointF(rect.X + rect.Width, rect.Y + rect.Height / 2));
	   points.push_back(Gdiplus::PointF(rect.X + rect.Width, rect.Y + rect.Height));
	   points.push_back(Gdiplus::PointF(rect.X + rect.Width / 2, rect.Y + rect.Height));
	   points.push_back(Gdiplus::PointF(rect.X, rect.Y + rect.Height));
	   points.push_back(Gdiplus::PointF(rect.X, rect.Y + rect.Height / 2));

	   float bbl_w = ChaoticParam(0.25f, rect.Width),
			 bbl_h = ChaoticParam(0.5f, rect.Height),
			 bbl_r = min(bbl_w, bbl_h) * 1.2f;

	   for (auto p : points)
		  {
			Gdiplus::PointF center = GetEllipseIntersection(rect, p); //отримуємо точку на еліпсі
			Gdiplus::RectF bubble(p.X - bbl_w / 2, p.Y - bbl_h / 2, bbl_w, bbl_h);

			res->AddEllipse(bubble);
          }

	   res->Outline();
	 }
  catch (Exception &e)
	 {
	   if (res) delete res;

	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::CreateCloudBase: " + e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

//rect – прямокутник, у якому має розташовуватись хмаринка
//countX/Y – кількість "пузирів" по горизонталі та вертикалі
//chaotic – коефіцієнт "хаотичності" параметрів точок (0.0f – рівна сітка, 1.0f – дуже розкидано)
bool CreateCloudBase2(std::vector<Gdiplus::PointF> &points, const Gdiplus::RectF &rect, float chaotic = 0.25f)
{
  bool res = false;

  try
	 {
	   float rectX = rect.X,
			 rectY = rect.Y,
			 sizeX = rect.Width,
			 sizeY = rect.Height,
			 stepX = sizeX / 3;

//лямбда, що модифікує параметр на основі хаотичного коефіцієнту
	   auto ChaoticParam = [&chaotic](float k, float prm)
	   {
		 float chaos = (rand() % 10 + 1) * chaotic / 100;

		 return float(prm * (k + chaos));
	   };

//лямбда, що вираховує координати основи кривої, згідно номера відрізку
	   auto GetCurveBase = [&rectX, &rectY, &sizeX, sizeY, &stepX, &ChaoticParam](int side)
	   {
		 Gdiplus::PointF l_base, r_base;

		 switch (side)
		   {
			 case SDTOPLEFT : l_base = Gdiplus::PointF(rectX + ChaoticParam(0.2f, stepX),
													   rectY - ChaoticParam(0.05f, stepX));
							  r_base = Gdiplus::PointF(rectX + stepX - ChaoticParam(0.05f, stepX),
													   rectY - ChaoticParam(0.1f, stepX));
							  break;

			 case SDTOP : l_base = Gdiplus::PointF(rectX + stepX + ChaoticParam(0.05f, stepX),
												   rectY - ChaoticParam(0.15f, stepX));
						  r_base = Gdiplus::PointF(rectX + stepX * 2 - ChaoticParam(0.05f, stepX),
												   rectY - ChaoticParam(0.15f, stepX));
						  break;

			 case SDTOPRIGHT : l_base = Gdiplus::PointF(rectX + stepX * 2 + ChaoticParam(0.05f, stepX),
														rectY - ChaoticParam(0.1f, stepX));
							   r_base = Gdiplus::PointF(rectX + stepX * 3 - ChaoticParam(0.2f, stepX),
														rectY - ChaoticParam(0.05f, stepX));
							   break;

			 case SDRIGHT : l_base = Gdiplus::PointF(rectX + sizeX + ChaoticParam(0.05f, stepX),
													 rectY + ChaoticParam(-0.0f, sizeY));
							r_base = Gdiplus::PointF(rectX + sizeX + ChaoticParam(0.05f, stepX),
													 rectY + ChaoticParam(1.0f, sizeY));
							break;

			 case SDBOTTOMRIGHT : l_base = Gdiplus::PointF(rectX + stepX * 3 - ChaoticParam(0.2f, stepX),
														   rectY + sizeY + ChaoticParam(0.05f, stepX));
								  r_base = Gdiplus::PointF(rectX + stepX * 2 + ChaoticParam(0.05f, stepX),
														   rectY + sizeY + ChaoticParam(0.1f, stepX));
								  break;

			 case SDBOTTOM : l_base = Gdiplus::PointF(rectX + stepX * 2 - ChaoticParam(0.05f, stepX),
													  rectY + sizeY + ChaoticParam(0.1f, stepX));
							 r_base = Gdiplus::PointF(rectX + stepX + ChaoticParam(0.05f, stepX),
													  rectY + sizeY + ChaoticParam(0.1f, stepX));
							 break;

			 case SDBOTTOMLEFT : l_base = Gdiplus::PointF(rectX + stepX - ChaoticParam(0.05f, stepX),
														  rectY + sizeY + ChaoticParam(0.1f, stepX));
								 r_base = Gdiplus::PointF(rectX + ChaoticParam(0.2f, stepX),
														  rectY + sizeY + ChaoticParam(0.05f, stepX));
								 break;

			 case SDLEFT : l_base = Gdiplus::PointF(rectX - ChaoticParam(0.05f, stepX),
													rectY + ChaoticParam(1.0f, sizeY));
						   r_base = Gdiplus::PointF(rectX - ChaoticParam(0.05f, stepX),
													rectY + ChaoticParam(0.0f, sizeY));
						   break;
		   }

		 return std::pair<Gdiplus::PointF, Gdiplus::PointF>(l_base, r_base);
	   };

	   auto GetCurveExtVertex = [&ChaoticParam](int x, int y, int width, int height, int side)
	   {
		 Gdiplus::PointF left, right;

		 if ((side == SDLEFT) || (side == SDRIGHT))
		   {
			 left = Gdiplus::PointF(x + ChaoticParam(0.6f, width), y + ChaoticParam(0.4f, height));
			 right = Gdiplus::PointF(x + ChaoticParam(0.6f, width), y + ChaoticParam(1.4f, height));
		   }
		 else if ((side == SDTOP) || (side == SDBOTTOM))
		   {
			 left = Gdiplus::PointF(x + ChaoticParam(0.4f, width), y + ChaoticParam(0.7f, height));
			 right = Gdiplus::PointF(x + ChaoticParam(1.4f, width), y + ChaoticParam(0.7f, height));
		   }
		 else
		   {
			 left = Gdiplus::PointF(x + ChaoticParam(0.4f, width), y + ChaoticParam(0.6f, height));
			 right = Gdiplus::PointF(x + ChaoticParam(1.4f, width), y + ChaoticParam(0.6f, height));
		   }

		 return std::pair<Gdiplus::PointF, Gdiplus::PointF>(left, right);
	   };

	   auto GetCurveVertex = [&ChaoticParam](int x, int y, int width, int height, int side)
	   {
		 Gdiplus::PointF res;

		 if ((side == SDLEFT) || (side == SDRIGHT))
		   res = Gdiplus::PointF(x + ChaoticParam(1.0f, width), y + ChaoticParam(1.0f, height));
		 else if ((side == SDTOP) || (side == SDBOTTOM))
		   res = Gdiplus::PointF(x + ChaoticParam(1.0f, width), y + ChaoticParam(1.1f, height));
		 else
		   res = Gdiplus::PointF(x + ChaoticParam(1.0f, width), y + ChaoticParam(1.0f, height));

		 return res;
	   };

//лямбда, що вираховує набір точок кривої, згідно сторони прямокутника
	  auto AddCurvePoints = [&rectX, &rectY, &stepX, &sizeY, &chaotic,
							 &GetCurveBase, &GetCurveExtVertex, &GetCurveVertex, &ChaoticParam]
							(std::vector<Gdiplus::PointF> &points, int side)
	   {
		 float w = 0.0f, h = 0.0f, fluff = 1.0f;

//визначаємо точки основи
		 auto [l_base, r_base] = GetCurveBase(side);

//визначаємо допоміжні точки
		 switch (side)
		   {
			 case SDTOPLEFT : w = stepX * 0.4f;
							  h = -stepX * 0.2f * ChaoticParam(1.0f, fluff);
							  break;

			 case SDTOP : w = stepX * 0.5f;
						  h = -stepX * 0.2f * ChaoticParam(1.3f, fluff);
						  break;

			 case SDTOPRIGHT : w = stepX * 0.4f;
							   h = -stepX * 0.2f * ChaoticParam(1.0f, fluff);
							   break;

			 case SDRIGHT : w = sizeY * 0.2f * ChaoticParam(1.1f, fluff);
							h = sizeY * 0.5f;
							break;

			 case SDBOTTOMRIGHT : w = -stepX * 0.4f;
								  h = stepX * 0.2f * ChaoticParam(1.0f, fluff);
								  break;

			 case SDBOTTOM : w = -stepX * 0.5f;
							 h = stepX * 0.2f * ChaoticParam(1.3f, fluff);
							 break;

			 case SDBOTTOMLEFT : w = -stepX * 0.4f;
								 h = stepX * 0.2f * ChaoticParam(1.0f, fluff);
								 break;

			 case SDLEFT : w = -sizeY * 0.2f * ChaoticParam(1.1f, fluff);
						   h = -sizeY * 0.5f;
						   break;
		   }

		 auto [l_ext, r_ext] = GetCurveExtVertex(l_base.X, l_base.Y, w, h, side);

//визначаємо точку вершини кривої
		 auto vert = GetCurveVertex(l_base.X, l_base.Y, w, h, side);

//додаємо в масив
		 points.push_back(l_base);
		 points.push_back(l_ext);
		 points.push_back(vert);
		 points.push_back(r_ext);
		 points.push_back(r_base);

		 //for (auto p : points)
			//std::wcout << "[" << p.X << " | " << p.Y << std::endl;
	   };

//починаємо заповнювати масив точок, рухаючись від лівого верхнього краю прямокутника
	   for (int i = SDTOPLEFT; i <= SDLEFT; i++)
		  AddCurvePoints(points, i);

	   points.push_back(Gdiplus::PointF(rect.X, rect.Y)); //проміжна точка для плавного з'єднання

	   res = true;
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::CreateCloudBase: " + e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

void DrawLineGDIPlus(int x1, int y1, int x2, int y2,
					 Gdiplus::Color color = Gdiplus::Color(255, 0, 0, 0),
					 int width = 0)
{
  try
	 {
	   if (!hMemDC)
		 throw Exception("Virtual buffer doesn't exists!");

	   Gdiplus::Graphics graphics(hMemDC);
	   graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	   Gdiplus::Pen pen(color, width);

	   graphics.DrawLine(&pen, x1, y1, x2, y2);
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::DrawLineGDIPlus: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void DrawArcGDIPlus(int x, int y,
					int width, int height,
					int st_angle, int sw_angle,
					Gdiplus::Color color = Gdiplus::Color(255, 0, 0, 0),
					int border_width = 0)
{
  try
	 {
	   if (!hMemDC)
		 throw Exception("Virtual buffer doesn't exists!");

	   Gdiplus::Graphics graphics(hMemDC);
	   graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	   Gdiplus::Pen pen(color, border_width);

	   graphics.DrawArc(&pen, x, y, width, height, st_angle, sw_angle);
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::DrawArcGDIPlus: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void DrawPolygonGDIPlus(Gdiplus::Point *points,
						int cnt,
						Gdiplus::Color fill_color = Gdiplus::Color(255, 255, 255, 255),
						Gdiplus::Color border_color = Gdiplus::Color(255, 0, 0, 0),
						float border_width = 1.0f,
						bool shadow = false)
{
  try
	 {
	   if (!hMemDC)
		 throw Exception("Virtual buffer doesn't exists!");

	   Gdiplus::Graphics graphics(hMemDC);
	   graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

//Створення основного шляху
	   Gdiplus::GraphicsPath path;

	   path.AddPolygon(points, cnt);

	   if (shadow) //Тінь
		 DrawShadow(&graphics, &path, 5, Gdiplus::Color(100, 0, 0, 0));

//зафарбовуємо і малюємо
	   Gdiplus::SolidBrush brush(fill_color);
	   graphics.FillPath(&brush, &path);

	   if (border_width > 0)
		 {
		   Gdiplus::Pen pen(border_color, border_width);
		   graphics.DrawPath(&pen, &path);
		 }
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::DrawPolygonGDIPlus: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void DrawEllipseGDIPlus(int x, int y,
						int width, int height,
						Gdiplus::Color fill_color = Gdiplus::Color(255, 255, 255, 255),
						Gdiplus::Color border_color = Gdiplus::Color(255, 0, 0, 0),
						float border_width = 1.0f,
					    bool shadow = false)
{
  try
	 {
	   if (!hMemDC)
		 throw Exception("Virtual buffer doesn't exists!");

	   Gdiplus::Graphics graphics(hMemDC);
	   graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

//Створення основного шляху
	   Gdiplus::GraphicsPath path;

       path.AddEllipse(x, y, width, height);

	   if (shadow) //Тінь
		 DrawShadow(&graphics, &path, 5, Gdiplus::Color(100, 0, 0, 0));

//зафарбовуємо і малюємо
	   Gdiplus::SolidBrush brush(fill_color);
	   graphics.FillPath(&brush, &path);

	   if (border_width > 0)
		 {
		   Gdiplus::Pen pen(border_color, border_width);
		   graphics.DrawPath(&pen, &path);
		 }
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::DrawEllipseGDIPlus: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void DrawTextGDIPlus(const std::wstring &text,
					 Gdiplus::RectF rect,
					 int user_font = 0,
					 const std::wstring &font_name = L"Segoe UI",
					 float font_size = 10.0f,
					 Gdiplus::FontStyle font_style = Gdiplus::FontStyleRegular,
					 Gdiplus::Color color = Gdiplus::Color(255, 0, 0, 0), // чорний
					 const std::wstring &align = L"left", // "left", "center", "right"
					 bool center_vertically = false,
					 bool word_wrap = true)
{
  try
	 {
	   if (!hMemDC)
		 throw Exception("Virtual buffer doesn't exists!");

	   Gdiplus::Graphics graphics(hMemDC);
	   std::unique_ptr<Gdiplus::FontFamily> font_family;

	   graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);

	   if (user_font) //використовується користувацький шрифт
		 {
		   font_family.reset(new Gdiplus::FontFamily(font_name.c_str(), UserFontCollection));

		   if (!font_family->IsAvailable())
			 font_family.reset(new Gdiplus::FontFamily(L"Arial")); //якщо шрифта в колекції не знайшлось
		 }
	   else
		 font_family.reset(new Gdiplus::FontFamily(font_name.c_str()));

	   Gdiplus::Font font(font_family.get(), font_size, font_style, Gdiplus::UnitPixel);
	   Gdiplus::SolidBrush brush(color);
	   Gdiplus::StringFormat format;

//Горизонтальне вирівнювання
	   if (align == L"center")
		 format.SetAlignment(Gdiplus::StringAlignmentCenter);
	   else if (align == L"right")
		 format.SetAlignment(Gdiplus::StringAlignmentFar);
	   else if (align == L"justify")
		 {
		   format.SetAlignment(Gdiplus::StringAlignmentNear);
		   format.SetFormatFlags(format.GetFormatFlags() |
								 Gdiplus::StringFormatFlagsLineLimit |
								 Gdiplus::StringFormatFlagsNoFitBlackBox);
		   format.SetTrimming(Gdiplus::StringTrimmingNone);
		 }
	   else
		 format.SetAlignment(Gdiplus::StringAlignmentNear);//left

//Вертикальне вирівнювання
	   format.SetLineAlignment(center_vertically ? Gdiplus::StringAlignmentCenter : Gdiplus::StringAlignmentNear);

//Перенос рядків
	   if (!word_wrap)
		 format.SetFormatFlags(Gdiplus::StringFormatFlagsNoWrap);

//визначаємо фактичний розмір області тексту
	   Gdiplus::RectF layout_rect;

	   if (rect.Width == 0)
		 rect.Width = 10000;

	   if (rect.Height == 0)
		 rect.Height = 10000;

	   if ((rect.Width == 0) || (rect.Height == 0))
		 graphics.MeasureString(text.c_str(), -1, &font, rect, &format, &layout_rect);
	   else
		 layout_rect = rect;

	   graphics.DrawString(text.c_str(), -1, &font, layout_rect, &format, &brush);
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::DrawTextGDIPlus: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void DrawRectangleGDIPlus(int x, int y, int width, int height,
						  Gdiplus::Color fill_color = Gdiplus::Color(255, 255, 255, 255),
						  Gdiplus::Color border_color = Gdiplus::Color(255, 0, 0, 0),
						  float border_width = 1.0f,
						  int corner_radius = 0,
						  bool shadow = false) //напівпрозора чорна тінь
{
  try
	 {
	   if (!hMemDC)
		 throw Exception("Virtual buffer doesn't exists!");

	   Gdiplus::Graphics graphics(hMemDC);
	   graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

//Створення основного шляху
	   Gdiplus::GraphicsPath path;
	   Gdiplus::RectF rect(x, y, width, height);

	   if (corner_radius > 0)
		 {
		   Gdiplus::GraphicsPath *RectPath = CreateRoundedRectPath(rect, corner_radius);
		   path.AddPath(RectPath, TRUE);

		   delete RectPath;
		 }
	   else
         path.AddRectangle(rect);

	   if (shadow) //Тінь
		 DrawShadow(&graphics, &path, 5, Gdiplus::Color(100, 0, 0, 0));

//зафарбовуємо і малюємо
	   Gdiplus::SolidBrush brush(fill_color);
	   graphics.FillPath(&brush, &path);

	   if (border_width > 0)
		 {
		   Gdiplus::Pen pen(border_color, border_width);
		   graphics.DrawPath(&pen, &path);
		 }
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::DrawRectangleGdiPlus: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void DrawSpeechBubbleRectGDIPlus(int x, int y, int width, int height,
								 Gdiplus::PointF& tail_point,
								 int tail_width = 10, //ширина основи хвостика
								 int corner_radius = 10,
								 Gdiplus::Color fill_color = Gdiplus::Color(255, 255, 255),
								 Gdiplus::Color border_color = Gdiplus::Color(0, 0, 0),
								 float border_width = 1.0f,
								 bool shadow = false)  //напівпрозора чорна тінь
{
  try
	 {
	   if (!hMemDC)
		 throw Exception("Virtual buffer doesn't exists!");

	   Gdiplus::Graphics graphics(hMemDC);
	   graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	   std::unique_ptr<Gdiplus::GraphicsPath> path(new Gdiplus::GraphicsPath());
	   Gdiplus::RectF rect(x, y, width, height);

	   if (corner_radius > 0) //заокруглений прямокутник
		 path.reset(CreateRoundedRectPath(rect, corner_radius));
	   else
		 path->AddRectangle(rect);

	   auto [base1, base2] = GetIntersectionNeighborsPoints(rect, tail_point, 360, tail_width);

	   Gdiplus::PointF tail[3] = {base1, tail_point, base2};

	   path->AddLines(tail, 3);
	   path->Outline(NULL, 1.0f);

       if (shadow) //тінь
		 DrawShadow(&graphics, path.get(), 5, Gdiplus::Color(100, 0, 0, 0));

//зафарбовуємо щоб отримати суцільний об'єкт
	   Gdiplus::SolidBrush brush(fill_color);
	   graphics.FillPath(&brush, path.get());

	   if (border_width > 0)
		 {
		   Gdiplus::Pen pen(border_color, border_width);
		   graphics.DrawPath(&pen, path.get());
		 }
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::DrawSpeechBubbleGDIPlus: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void DrawSpeechBlastGDIPlus(int x, int y, int width, int height,
							int min_ray_h,
							int max_ray_h,
							bool rand_h,
							Gdiplus::Color fill_color = Gdiplus::Color(255, 255, 255, 255),
							Gdiplus::Color border_color = Gdiplus::Color(255, 0, 0, 0),
							float border_width = 1.0f,
							bool shadow = false) //напівпрозора чорна тінь
{
  try
	 {
	   if (!hMemDC)
		 throw Exception("Virtual buffer doesn't exists!");

	   Gdiplus::Graphics graphics(hMemDC);
	   graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

//Створення основного шляху
	   Gdiplus::RectF rect(x, y, width, height);
	   std::unique_ptr<Gdiplus::GraphicsPath> path(CreateBlastPolygon(rect, min_ray_h, max_ray_h, rand_h));

	   if (shadow) //Тінь
		 DrawShadow(&graphics, path.get(), 5, Gdiplus::Color(100, 0, 0, 0));

//зафарбовуємо і малюємо
	   Gdiplus::SolidBrush brush(fill_color);
	   graphics.FillPath(&brush, path.get());

	   if (border_width > 0)
		 {
		   Gdiplus::Pen pen(border_color, border_width);
		   graphics.DrawPath(&pen, path.get());
		 }
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::DrawSpeechBlastGDIPlus: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void DrawSpeechBalloonGDIPlus(int x, int y, int width, int height,
							  const Gdiplus::PointF& tail_point,
							  int tail_width = 10, //ширина основи хвостика
							  Gdiplus::Color fill_color = Gdiplus::Color(255, 255, 255, 255),
							  Gdiplus::Color border_color = Gdiplus::Color(255, 0, 0, 0),
							  float border_width = 1.0f,
							  bool shadow = false) //напівпрозора чорна тінь
{
  try
	 {
	   if (!hMemDC)
		 throw Exception("Virtual buffer doesn't exists!");

	   Gdiplus::Graphics graphics(hMemDC);
	   graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

//Створення основного шляху
	   Gdiplus::RectF rect(x, y, width, height);
	   Gdiplus::GraphicsPath path;

	   float a = width / 2,
			 b = height / 2; //осі прямокутника

//отримуємо кути до точок основи хвостика,
//360 - кількість дискретних точок еліпса
//15 - крок зсуву по дузі ~ кількість пікселів
	   auto [prev_angle, next_angle] = GetIntersectionNeighborsAngles(rect, tail_point, 360, tail_width);

//обчислюємо кути для дуги
	   float start = (float)(prev_angle * 180 / M_PI); //конвертуємо радіани в градуси
	   float end = (float)(next_angle * 180 / M_PI);   
	   float sweep = end - start;

	   if (sweep <= 0)
		 sweep += 360.0f;

	   sweep -= 360.0f;

	   Gdiplus::PointF lp;

//будуємо дугу та хвостик
	   path.AddArc(rect, start, sweep);
	   path.GetLastPoint(&lp);
	   path.AddLine(lp, tail_point);

	   path.CloseFigure();

	   if (shadow) //Тінь
		 DrawShadow(&graphics, &path, 5, Gdiplus::Color(100, 0, 0, 0));

//зафарбовуємо і малюємо
	   Gdiplus::SolidBrush brush(fill_color);
	   graphics.FillPath(&brush, &path);

	   if (border_width > 0)
		 {
		   Gdiplus::Pen pen(border_color, border_width);
		   graphics.DrawPath(&pen, &path);
		 }
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::DrawSpeechBalloonGDIPlus: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void DrawSpeechCloudGDIPlus(int x, int y, int width, int height,
							  const Gdiplus::PointF& tail_point,
                              int tail_width = 10, //ширина основи хвостика
							  Gdiplus::Color fill_color = Gdiplus::Color(255, 255, 255, 255),
							  Gdiplus::Color border_color = Gdiplus::Color(255, 0, 0, 0),
							  float border_width = 1.0f,
							  bool shadow = false) //напівпрозора чорна тінь
{
  try
	 {
	   if (!hMemDC)
		 throw Exception("Virtual buffer doesn't exists!");

	   Gdiplus::Graphics graphics(hMemDC);
	   graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

//створення основного шляху
	   Gdiplus::RectF rect(x, y, width, height);
	   std::unique_ptr<Gdiplus::GraphicsPath> path(CreateCloudBase(rect, 4, 3, 0.3f));
	   std::vector<Gdiplus::PointF> points;

	   if (!path)
		 throw Exception("Can't build cloud base");

//формуємо плавний шлях
	   //path.AddCurve(points.data(), (int)points.size(), 0.5f); // 0.5f – натяг кривої
	   //path.CloseFigure();

	   //if (shadow) //Тінь
		 //DrawShadow(&graphics, &path, 5, Gdiplus::Color(100, 0, 0, 0));

//зафарбовуємо і малюємо
	   Gdiplus::SolidBrush brush(fill_color);
	   graphics.FillPath(&brush, path.get());

	   if (border_width > 0)
		 {
		   Gdiplus::Pen pen(border_color, border_width);
		   graphics.DrawPath(&pen, path.get());
		 }
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::DrawSpeechCloudGDIPlus: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

LRESULT CALLBACK HostWindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  static int sx, sy; //розміри вікна
  static POINT mouse; //координати курсора миші

  switch (message)
	{
	  case WM_CREATE:
		{
		  break;
		}

	  case WM_SIZE:
		{
		  sx = LOWORD(lParam);
		  sy = HIWORD(lParam);
		  break;
		}

	  case WM_KVL_CLEAR_WINDOW:
		{
		  EnterCriticalSection(&cs);

		  ClearVirtualWindow();

		  LeaveCriticalSection(&cs);

          InvalidateRect(WHandle, NULL, FALSE);  //перемальовуємо вікно
		  break;
		}

	  case WM_KVL_DRAW_LINE:
		{
		  EnterCriticalSection(&cs);

		  DrawLineGDIPlus(CurrentLine.X1, CurrentLine.Y1, CurrentLine.X2, CurrentLine.Y2,
						  CurrentLine.Color,
                          CurrentLine.Size);

		  LeaveCriticalSection(&cs);

		  InvalidateRect(WHandle, NULL, FALSE);  //перемальовуємо вікно
		  break;
		}

	  case WM_KVL_DRAW_ARC:
		{
		  EnterCriticalSection(&cs);

		  DrawArcGDIPlus(CurrentArc.Left, CurrentArc.Top,
		  				 CurrentArc.Width, CurrentArc.Height,
						 CurrentArc.StartAngle, CurrentArc.SweepAngle,
						 CurrentArc.Color,
						 CurrentArc.Size);

		  LeaveCriticalSection(&cs);

		  InvalidateRect(WHandle, NULL, FALSE);  //перемальовуємо вікно
		  break;
		}

	  case WM_KVL_DRAW_POLYGON:
		{
		  EnterCriticalSection(&cs);

		  DrawPolygonGDIPlus(CurrentPoly.Points.data(),
							 CurrentPoly.Points.size(),
							 CurrentPoly.Color,
							 CurrentPoly.BorderColor,
							 CurrentPoly.Border,
                             CurrentPoly.Shadow);

		  LeaveCriticalSection(&cs);

		  InvalidateRect(WHandle, NULL, FALSE);  //перемальовуємо вікно
		  break;
		}

	  case WM_KVL_DRAW_ELLIPSE:
		{
		  EnterCriticalSection(&cs);

		  DrawEllipseGDIPlus(CurrentEllipse.Left,
							 CurrentEllipse.Top,
							 CurrentEllipse.Width,
                             CurrentEllipse.Height,
							 CurrentEllipse.Color,
							 CurrentEllipse.BorderColor,
							 CurrentEllipse.Border,
							 CurrentEllipse.Shadow);

		  LeaveCriticalSection(&cs);

		  InvalidateRect(WHandle, NULL, FALSE);  //перемальовуємо вікно
		  break;
		}

	  case WM_KVL_DRAW_RECT:
		{
		  EnterCriticalSection(&cs);

		  DrawRectangleGDIPlus(CurrentRect.Left,
							   CurrentRect.Top,
							   CurrentRect.Width,
							   CurrentRect.Height,
							   CurrentRect.Color,
							   CurrentRect.BorderColor,
							   CurrentRect.Border,
							   0,
							   CurrentRect.Shadow);

		  LeaveCriticalSection(&cs);

		  InvalidateRect(WHandle, NULL, FALSE);  //перемальовуємо вікно
		  break;
		}

	  case WM_KVL_DRAW_IMAGE:
		{
		  EnterCriticalSection(&cs);

		  int x = LOWORD(lParam);
		  int y = HIWORD(lParam);

		  DrawToVirtualWindow((LPWSTR)wParam, x, y);  //оновлення буфера

		  LeaveCriticalSection(&cs);

		  InvalidateRect(WHandle, NULL, FALSE);  //перемальовуємо вікно
		  break;
		}

	  case WM_KVL_DRAW_PLATE:
		{
		  EnterCriticalSection(&cs);

		  DrawRectangleGDIPlus(CurrentPlate.Left,
							   CurrentPlate.Top,
							   CurrentPlate.Width,
							   CurrentPlate.Height,
							   CurrentPlate.Color,
							   CurrentPlate.BorderColor,
							   CurrentPlate.Border,
							   CurrentPlate.CornerRadius,
							   CurrentPlate.Shadow);

		  LeaveCriticalSection(&cs);

		  InvalidateRect(WHandle, NULL, FALSE);  //перемальовуємо вікно
		  break;
		}

	  case WM_KVL_DRAW_BUBBLE:
		{
		  EnterCriticalSection(&cs);

		  DrawSpeechBubbleRectGDIPlus(CurrentBubble.Left,
									  CurrentBubble.Top,
									  CurrentBubble.Width,
									  CurrentBubble.Height,  	  //основний прямокутник
									  CurrentBubble.Tail, 		  //координати кінчика хвостика
									  CurrentBubble.TailWidth,    //ширина основи хвостика
									  CurrentBubble.CornerRadius, //радіус заокруглення
									  CurrentBubble.Color,        //заливка
									  CurrentBubble.BorderColor,  //рамка
									  CurrentBubble.Border,       //товщина
									  CurrentBubble.Shadow);      //увімкнути тінь

		  LeaveCriticalSection(&cs);

		  InvalidateRect(WHandle, NULL, FALSE);  //перемальовуємо вікно
		  break;
		}

	  case WM_KVL_DRAW_BLAST:
		{
		  EnterCriticalSection(&cs);

		  DrawSpeechBlastGDIPlus(CurrentBlast.Left,
								 CurrentBlast.Top,
								 CurrentBlast.Width,
								 CurrentBlast.Height,  	     //основний прямокутник
								 CurrentBlast.MinRayHeight,  //мінімальна висота променю
								 CurrentBlast.MaxRayHeight,  //максимальна висота променю
                                 CurrentBlast.DynamicRays,   //варіативна висота променів
								 CurrentBlast.Color,         //заливка
								 CurrentBlast.BorderColor,   //рамка
								 CurrentBlast.Border,        //товщина
								 CurrentBlast.Shadow);       //увімкнути тінь);

		  LeaveCriticalSection(&cs);

		  InvalidateRect(WHandle, NULL, FALSE);  //перемальовуємо вікно
		  break;
		}

	  case WM_KVL_DRAW_BALLOON:
		{
		  EnterCriticalSection(&cs);

		  DrawSpeechBalloonGDIPlus(CurrentBalloon.Left,
								   CurrentBalloon.Top,
								   CurrentBalloon.Width,
								   CurrentBalloon.Height,  	    //основний прямокутник
								   CurrentBalloon.Tail, 		//координати кінчика хвостика
								   CurrentBalloon.TailWidth,    //ширина основи хвостика
								   CurrentBalloon.Color,        //заливка
								   CurrentBalloon.BorderColor,  //рамка
								   CurrentBalloon.Border,       //товщина
								   CurrentBalloon.Shadow);      //увімкнути тінь

		  LeaveCriticalSection(&cs);

		  InvalidateRect(WHandle, NULL, FALSE);  //перемальовуємо вікно
		  break;
		}

	  case WM_KVL_DRAW_CLOUD:
		{
		  EnterCriticalSection(&cs);

		  DrawSpeechCloudGDIPlus(CurrentCloud.Left,
								 CurrentCloud.Top,
								 CurrentCloud.Width,
								 CurrentCloud.Height,  	    //основний прямокутник
								 CurrentCloud.Tail, 		//координати кінчика хвостика
                                 CurrentCloud.TailWidth,    //ширина основи хвостика
								 CurrentCloud.Color,        //заливка
								 CurrentCloud.BorderColor,  //рамка
								 CurrentCloud.Border,       //товщина
								 CurrentCloud.Shadow);      //увімкнути тінь

		  LeaveCriticalSection(&cs);

		  InvalidateRect(WHandle, NULL, FALSE);  //перемальовуємо вікно
		  break;
		}

      case WM_KVL_DRAW_TEXT:
		{
		  EnterCriticalSection(&cs);

		  DrawTextGDIPlus((LPWSTR)wParam,
						  CurrentText.Rect,
						  CurrentText.UserFont,
						  CurrentText.FontName,
						  CurrentText.FontSize,
						  (Gdiplus::FontStyle)CurrentText.FontStyle,
						  CurrentText.FontColor,
						  CurrentText.Alignment,
						  CurrentText.CenterVerticaly,
						  CurrentText.WordWrap);

		  LeaveCriticalSection(&cs);

		  InvalidateRect(WHandle, NULL, FALSE);  //перемальовуємо вікно
		  break;
		}

	  case WM_DESTROY:
		{
		  PostQuitMessage(0);
		  break;
		}

	  case WM_PAINT:
		{
		  PAINTSTRUCT ps;
		  HDC hdc = BeginPaint(WHandle, &ps);
		  EnterCriticalSection(&cs);
		  CopyVirtualToMain(hdc);
		  LeaveCriticalSection(&cs);
		  EndPaint(WHandle, &ps);
		  break;
		}

	  case WM_LBUTTONUP:
		{
		  mouse.x = LOWORD(lParam);
		  mouse.y = HIWORD(lParam);

		  break;
		}

	  case WM_CLOSE:
		{
		  DestroyWindow(hwnd);
		  break;
		}

	  case WM_QUIT: break;

	  default:  /* for messages that we don't deal with */
		return DefWindowProc (hwnd, message, wParam, lParam);
	}

  return 0;
}
//-------------------------------------------------------------------------

unsigned __stdcall CreateHostWindow(void*)
{
  try
	 {
	   MSG Msg;

//Ініціалізація GDI+
	   Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	   ULONG_PTR gdiplusToken;
	   Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	   UserFontCollection = new Gdiplus::PrivateFontCollection; //ініціалізуємо колекцію шрифтів для вікна

	   WNDCLASSEX wcex = {sizeof(WNDCLASSEX),
						  CS_HREDRAW|CS_VREDRAW,
						  HostWindowProcedure,
						  0, 0,
						  GetModuleHandle(NULL),
						  NULL,
						  LoadCursor(NULL, IDC_ARROW),
						  (HBRUSH)(COLOR_WINDOW+1),
						  NULL,
						  L"KobzarVisualisation",
						  NULL};

	   RegisterClassEx(&wcex);

//Створюємо вікно
	  WHandle = CreateWindow(L"KobzarVisualisation",
							 L"Kobzar Visual Library Window", WS_OVERLAPPEDWINDOW,
							 CW_USEDEFAULT, CW_USEDEFAULT,
							 WndWidth, WndHeight,
							 NULL, NULL,
							 GetModuleHandle(NULL),
							 NULL);

		CreateVirtualWindow(WHandle, WndWidth, WndHeight);

		ShowWindow(WHandle, SW_SHOWNORMAL);
        UpdateWindow(WHandle);

		/* Run the message loop. It will run until GetMessage() returns 0 */
		while (GetMessage (&Msg, NULL, 0, 0))
		  {
			TranslateMessage(&Msg);
      		DispatchMessage(&Msg);
		  }

//Очищення пам’яті
		DeleteObject(hBitmap);
		DeleteDC(hMemDC);

//Завершення роботи з GDI+
		Gdiplus::GdiplusShutdown(gdiplusToken);

		delete UserFontCollection;

		UnregisterClass(L"KobzarVisualisation", DllHinst);
		WHandle = NULL;
	   _endthreadex(0);
	 }
  catch (...)
	 {
	   _endthreadex(0);
	   throw std::runtime_error("CreateHostWindow: error!");
	 }

  return 0;
}
//---------------------------------------------------------------------------

void StartWork()
{
  try
	 {
	   InitializeCriticalSection(&cs);  //Ініціалізуємо синхронізацію
	   WndThread = (HANDLE)_beginthreadex(NULL, 4096, CreateHostWindow, NULL, NULL, &thID);
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::StartWork: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void StopWork()
{
  try
	 {
	   if (WHandle)
		 PostMessage(WHandle, WM_CLOSE, 0, 0);

	   WaitForSingleObject(WndThread, 500);
	   DeleteCriticalSection(&cs);
	   CloseHandle(WndThread);
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::StopWork: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
extern "C"
{
__declspec(dllexport) void __stdcall eLoadFont(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

       String obj = eIface->GetParamToStr(L"pObjectName");

	   if (obj == "")
		 throw Exception("Can't get main object name!");

	   obj = "&" + obj;

	   String file = eIface->GetObjectProperty(obj.c_str(), L"Source");

	   file = ParseString(file, ".\\", String(eIface->GetInitDir()) + "\\");

	   if (!FileExists(file))
		 throw Exception("Error loading file: " + file);

       eIface->SetObjectProperty(obj.c_str(), L"Source", file.c_str());

	   std::unique_ptr<wchar_t[]> fname(GetFontFamilyName(file.c_str()));

	   eIface->SetObjectProperty(obj.c_str(), L"Name", fname.get());

	   Gdiplus::Status status = UserFontCollection->AddFontFile(file.c_str());

	   if (status != Gdiplus::Status::Ok)
		 throw Exception("Error adding font from file: " + file);

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eLoadFont: " + e.ToString());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eCreateForm(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   WndWidth = eIface->GetParamToInt(L"pWidth"),
	   WndHeight = eIface->GetParamToInt(L"pHeight");

	   bool full = eIface->GetParamToInt(L"pFullscreen");

	   if (!WHandle)
		  StartWork();
	   else
		 {
		   StopWork();
		   StartWork();
		 }

       Sleep(500);

	   PostMessage(WHandle, WM_KVL_CLEAR_WINDOW, 0, 0);

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eCreateForm: " + e.ToString());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eDestroyForm(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   if (!WHandle)
		  throw Exception("Window doesn't exists!");
	   else
		 PostMessage(WHandle, WM_CLOSE, 0, 0);

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eDestroyForm: " + e.ToString());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

_declspec(dllexport) void __stdcall eClearForm(void *p)
{
  try
	 {
       if (!WHandle)
		 throw Exception("Window doesn't exists!");

	   eIface = static_cast<ELI_INTERFACE*>(p);

	   PostMessage(WHandle, WM_KVL_CLEAR_WINDOW, 0, 0);

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eClearForm: " + e.ToString());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eCalcTextArea(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   TEXT CheckedText;

	   String obj = eIface->GetParamToStr(L"pObjectName");

	   if (obj == "")
		 throw Exception("Can't get main object name!");

	   obj = "&" + obj;

	   String obj_font = eIface->GetObjectProperty(obj.c_str(), L"Font");

	   if (obj_font == "")
		 throw Exception("Can't get Font object name!");

	   CheckedText.Rect.X = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Left"));
	   CheckedText.Rect.Y = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Top"));
	   CheckedText.Rect.Width = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Width"));
	   CheckedText.Rect.Height = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Height"));
	   wcscpy(CheckedText.Alignment, eIface->GetObjectProperty(obj.c_str(), L"Align"));
	   CheckedText.CenterVerticaly = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"CenterVerticaly"));
	   CheckedText.WordWrap = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"WordWrap"));
	   String text = eIface->GetObjectProperty(obj.c_str(), L"Data");

	   CheckedText.UserFont = _wtoi(eIface->GetObjectProperty(obj_font.c_str(), L"UserFont"));
	   wcscpy(CheckedText.FontName, eIface->GetObjectProperty(obj_font.c_str(), L"Name"));
	   CheckedText.FontSize = _wtoi(eIface->GetObjectProperty(obj_font.c_str(), L"Size"));

	   String style = eIface->GetObjectProperty(obj_font.c_str(), L"Style");

	   if (style == "b")
		 CheckedText.FontStyle = 1;
	   else if (style == "i")
		 CheckedText.FontStyle = 2;
	   else if (style == "bi")
		 CheckedText.FontStyle = 3;
	   else if (style == "u")
		 CheckedText.FontStyle = 4;
	   else if (style == "s")
		 CheckedText.FontStyle = 8;
	   else
		 CheckedText.FontStyle = 0;

	   CheckedText.Rect = MeasureTextRect(text.c_str(),
										  CheckedText.Rect,
										  CheckedText.UserFont,
										  CheckedText.FontName,
										  CheckedText.FontSize,
										  (Gdiplus::FontStyle)CheckedText.FontStyle,
										  CheckedText.Alignment,
										  CheckedText.CenterVerticaly,
										  CheckedText.WordWrap);

	   eIface->SetObjectProperty(obj.c_str(), L"Width", IntToStr((int)CheckedText.Rect.Width + 1).c_str());
	   eIface->SetObjectProperty(obj.c_str(), L"Height", IntToStr((int)CheckedText.Rect.Height + 1).c_str());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eGetTextArea: " + e.ToString());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eDrawLine(void *p)
{
  try
	 {
	   if (!WHandle)
		 throw Exception("Window doesn't exists!");

	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String obj = eIface->GetParamToStr(L"pObjectName");

	   if (obj == "")
		 throw Exception("Can't get main object name!");

	   obj = "&" + obj;

	   String obj_color = eIface->GetObjectProperty(obj.c_str(), L"Color");

	   if (obj_color == "")
		 throw Exception("Can't get Color object name!");

	   CurrentLine.X1 = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"X1"));
	   CurrentLine.Y1 = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Y1"));
	   CurrentLine.X2 = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"X2"));
	   CurrentLine.Y2 = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Y2"));
	   CurrentLine.Size = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Size"));

	   CurrentLine.Color = Gdiplus::Color(_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Alpha")),
										  _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Red")),
										  _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Green")),
										  _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Blue")));

	   PostMessage(WHandle, WM_KVL_DRAW_LINE, NULL, NULL);

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eDrawLine: " + e.ToString());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eDrawArc(void *p)
{
  try
	 {
	   if (!WHandle)
		 throw Exception("Window doesn't exists!");

	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String obj = eIface->GetParamToStr(L"pObjectName");

	   if (obj == "")
		 throw Exception("Can't get main object name!");

	   obj = "&" + obj;

	   String obj_color = eIface->GetObjectProperty(obj.c_str(), L"Color");

	   if (obj_color == "")
		 throw Exception("Can't get Color object name!");

	   CurrentArc.Left = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Left"));
	   CurrentArc.Top = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Top"));
	   CurrentArc.Width = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Width"));
	   CurrentArc.Height = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Height"));
	   CurrentArc.StartAngle = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"StartAngle"));
	   CurrentArc.SweepAngle = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"SweepAngle"));
	   CurrentArc.Size = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Size"));

	   CurrentArc.Color = Gdiplus::Color(_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Alpha")),
										 _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Red")),
										 _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Green")),
										 _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Blue")));

	   PostMessage(WHandle, WM_KVL_DRAW_ARC, NULL, NULL);

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eDrawArc: " + e.ToString());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eDrawPoly(void *p)
{
  try
	 {
	   if (!WHandle)
		 throw Exception("Window doesn't exists!");

	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String obj = eIface->GetParamToStr(L"pObjectName");

	   if (obj == "")
		 throw Exception("Can't get main object name!");

	   obj = "&" + obj;

	   int cnt = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Count"));
	   int x = 0, y = 0;
	   String pt = "";

	   for (int i = 0; i < cnt; i++)
		  {
			pt = eIface->GetObjectProperty(obj.c_str(), IntToStr(i).c_str());
			x = _wtoi(eIface->GetObjectProperty(pt.c_str(), L"X"));
			y = _wtoi(eIface->GetObjectProperty(pt.c_str(), L"Y"));

			CurrentPoly.Points.push_back(Gdiplus::Point(x, y));
		  }

	   String obj_color = eIface->GetObjectProperty(obj.c_str(), L"Color");

	   if (obj_color == "")
		 throw Exception("Can't get Color object name!");

	   String obj_bord = eIface->GetObjectProperty(obj.c_str(), L"Border");

	   if (obj_bord == "")
		 throw Exception("Can't get Border object name!");

	   CurrentPoly.Shadow = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Shadow"));

	   CurrentPoly.Color = Gdiplus::Color(_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Alpha")),
										  _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Red")),
										  _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Green")),
										  _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Blue")));

	   CurrentPoly.Border = _wtoi(eIface->GetObjectProperty(obj_bord.c_str(), L"Size"));

	   obj_color = eIface->GetObjectProperty(obj_bord.c_str(), L"Color");

	   if (obj_color == "")
		 throw Exception("Can't get Border Color object name!");

	   CurrentPoly.BorderColor = Gdiplus::Color(_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Alpha")),
												_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Red")),
												_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Green")),
												_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Blue")));


	   PostMessage(WHandle, WM_KVL_DRAW_POLYGON, NULL, NULL);

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eDrawPolygon: " + e.ToString());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eDrawEllipse(void *p)
{
  try
	 {
	   if (!WHandle)
		 throw Exception("Window doesn't exists!");

	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String obj = eIface->GetParamToStr(L"pObjectName");

	   if (obj == "")
		 throw Exception("Can't get main object name!");

	   obj = "&" + obj;

	   CurrentEllipse.Left = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Left"));
	   CurrentEllipse.Top = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Top"));
	   CurrentEllipse.Width = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Width"));
	   CurrentEllipse.Height = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Height"));

	   String obj_color = eIface->GetObjectProperty(obj.c_str(), L"Color");

	   if (obj_color == "")
		 throw Exception("Can't get Color object name!");

	   String obj_bord = eIface->GetObjectProperty(obj.c_str(), L"Border");

	   if (obj_bord == "")
		 throw Exception("Can't get Border object name!");

	   CurrentEllipse.Shadow = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Shadow"));

	   CurrentEllipse.Color = Gdiplus::Color(_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Alpha")),
											 _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Red")),
											 _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Green")),
											 _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Blue")));

	   CurrentEllipse.Border = _wtoi(eIface->GetObjectProperty(obj_bord.c_str(), L"Size"));

	   obj_color = eIface->GetObjectProperty(obj_bord.c_str(), L"Color");

	   if (obj_color == "")
		 throw Exception("Can't get Border Color object name!");

	   CurrentEllipse.BorderColor = Gdiplus::Color(_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Alpha")),
												   _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Red")),
												   _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Green")),
												   _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Blue")));


	   PostMessage(WHandle, WM_KVL_DRAW_ELLIPSE, NULL, NULL);

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eDrawEllipse: " + e.ToString());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eDrawRect(void *p)
{
  try
	 {
	   if (!WHandle)
		 throw Exception("Window doesn't exists!");

	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String obj = eIface->GetParamToStr(L"pObjectName");

	   if (obj == "")
		 throw Exception("Can't get main object name!");

	   obj = "&" + obj;

	   CurrentRect.Left = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Left"));
	   CurrentRect.Top = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Top"));
	   CurrentRect.Width = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Width"));
	   CurrentRect.Height = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Height"));

	   String obj_color = eIface->GetObjectProperty(obj.c_str(), L"Color");

	   if (obj_color == "")
		 throw Exception("Can't get Color object name!");

	   String obj_bord = eIface->GetObjectProperty(obj.c_str(), L"Border");

	   if (obj_bord == "")
		 throw Exception("Can't get Border object name!");

	   CurrentRect.Shadow = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Shadow"));

	   CurrentRect.Color = Gdiplus::Color(_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Alpha")),
										  _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Red")),
										  _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Green")),
										  _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Blue")));

	   CurrentRect.Border = _wtoi(eIface->GetObjectProperty(obj_bord.c_str(), L"Size"));

	   obj_color = eIface->GetObjectProperty(obj_bord.c_str(), L"Color");

	   if (obj_color == "")
		 throw Exception("Can't get Border Color object name!");

	   CurrentRect.BorderColor = Gdiplus::Color(_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Alpha")),
												_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Red")),
												_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Green")),
												_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Blue")));


	   PostMessage(WHandle, WM_KVL_DRAW_RECT, NULL, NULL);
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eDrawRect: " + e.ToString());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eDrawImage(void *p)
{
  try
	 {
	   if (!WHandle)
		 throw Exception("Window doesn't exists!");

	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String obj = eIface->GetParamToStr(L"pObjectName");

	   if (obj == "")
		 throw Exception("Can't get main object name!");

	   obj = "&" + obj;

	   int x = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Left")),
		   y = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Top"));

	   String file = eIface->GetObjectProperty(obj.c_str(), L"Source");

	   file = ParseString(file, ".\\", String(eIface->GetInitDir()) + "\\");

	   if (!FileExists(file))
		 throw Exception("Error loading image file: " + file);

	   PostMessage(WHandle, WM_KVL_DRAW_IMAGE, (WPARAM)file.c_str(), MAKELPARAM(x, y));

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eDrawImage: " + e.ToString());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eDrawPlate(void *p)
{
  try
	 {
	   if (!WHandle)
		 throw Exception("Window doesn't exists!");

	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String obj = eIface->GetParamToStr(L"pObjectName");

	   if (obj == "")
		 throw Exception("Can't get main object name!");

	   obj = "&" + obj;

	   String obj_color = eIface->GetObjectProperty(obj.c_str(), L"Color");

	   if (obj_color == "")
		 throw Exception("Can't get Color object name!");

	   String obj_bord = eIface->GetObjectProperty(obj.c_str(), L"Border");

	   if (obj_bord == "")
		 throw Exception("Can't get Border object name!");

	   CurrentPlate.Left = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Left"));
	   CurrentPlate.Top = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Top"));
	   CurrentPlate.Width = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Width"));
	   CurrentPlate.Height = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Height"));

	   CurrentPlate.CornerRadius = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Corner"));
	   CurrentPlate.Shadow = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Shadow"));

	   CurrentPlate.Color = Gdiplus::Color(_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Alpha")),
										   _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Red")),
										   _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Green")),
										   _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Blue")));

	   CurrentPlate.Border = _wtoi(eIface->GetObjectProperty(obj_bord.c_str(), L"Size"));

	   obj_color = eIface->GetObjectProperty(obj_bord.c_str(), L"Color");

	   if (obj_color == "")
		 throw Exception("Can't get Border Color object name!");

	   CurrentPlate.BorderColor = Gdiplus::Color(_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Alpha")),
												 _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Red")),
												 _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Green")),
												 _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Blue")));

	   PostMessage(WHandle, WM_KVL_DRAW_PLATE, NULL, NULL);

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eDrawPlate: " + e.ToString());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eDrawBubble(void *p)
{
  try
	 {
	   if (!WHandle)
		 throw Exception("Window doesn't exists!");

	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String obj = eIface->GetParamToStr(L"pObjectName");

	   if (obj == "")
		 throw Exception("Can't get main object name!");

	   obj = "&" + obj;

	   String obj_color = eIface->GetObjectProperty(obj.c_str(), L"Color");

	   if (obj_color == "")
		 throw Exception("Can't get Color object name!");

	   String obj_bord = eIface->GetObjectProperty(obj.c_str(), L"Border");

	   if (obj_color == "")
		 throw Exception("Can't get Border object name!");

	   String obj_tail = eIface->GetObjectProperty(obj.c_str(), L"Tail");

	   if (obj_tail == "")
		 throw Exception("Can't get Tail object name!");

	   CurrentBubble.Left = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Left"));
	   CurrentBubble.Top = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Top"));
	   CurrentBubble.Width = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Width"));
	   CurrentBubble.Height = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Height"));

	   CurrentBubble.TailWidth = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"TailWidth"));
	   CurrentBubble.CornerRadius = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Corner"));
	   CurrentBubble.Shadow = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Shadow"));

	   CurrentBubble.Color = Gdiplus::Color(_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Alpha")),
											_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Red")),
											_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Green")),
											_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Blue")));

	   CurrentBubble.Border = _wtoi(eIface->GetObjectProperty(obj_bord.c_str(), L"Size"));

	   CurrentBubble.Tail.X = _wtoi(eIface->GetObjectProperty(obj_tail.c_str(), L"X"));
	   CurrentBubble.Tail.Y = _wtoi(eIface->GetObjectProperty(obj_tail.c_str(), L"Y"));

	   obj_color = eIface->GetObjectProperty(obj_bord.c_str(), L"Color");

	   if (obj_color == "")
		 throw Exception("Can't get Border Color object name!");

	   CurrentBubble.BorderColor = Gdiplus::Color(_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Alpha")),
												  _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Red")),
												  _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Green")),
												  _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Blue")));

	   PostMessage(WHandle, WM_KVL_DRAW_BUBBLE, NULL, NULL);

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eDrawBubble: " + e.ToString());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eDrawBlast(void *p)
{
  try
	 {
	   if (!WHandle)
		 throw Exception("Window doesn't exists!");

	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String obj = eIface->GetParamToStr(L"pObjectName");

	   if (obj == "")
		 throw Exception("Can't get main object name!");

	   obj = "&" + obj;

	   String obj_color = eIface->GetObjectProperty(obj.c_str(), L"Color");

	   if (obj_color == "")
		 throw Exception("Can't get Color object name!");

	   String obj_bord = eIface->GetObjectProperty(obj.c_str(), L"Border");

	   if (obj_color == "")
		 throw Exception("Can't get Border object name!");

	   CurrentBlast.Left = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Left"));
	   CurrentBlast.Top = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Top"));
	   CurrentBlast.Width = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Width"));
	   CurrentBlast.Height = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Height"));
	   CurrentBlast.MinRayHeight = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"MinRayHeight"));
	   CurrentBlast.MaxRayHeight = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"MaxRayHeight"));
	   CurrentBlast.DynamicRays = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"DynamicRays"));

	   CurrentBlast.Shadow = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Shadow"));

	   CurrentBlast.Color = Gdiplus::Color(_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Alpha")),
										   _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Red")),
										   _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Green")),
										   _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Blue")));

	   CurrentBlast.Border = _wtoi(eIface->GetObjectProperty(obj_bord.c_str(), L"Size"));

	   obj_color = eIface->GetObjectProperty(obj_bord.c_str(), L"Color");

	   if (obj_color == "")
		 throw Exception("Can't get Border Color object name!");

	   CurrentBlast.BorderColor = Gdiplus::Color(_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Alpha")),
												 _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Red")),
												 _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Green")),
												 _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Blue")));

	   PostMessage(WHandle, WM_KVL_DRAW_BLAST, NULL, NULL);

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eDrawBlast: " + e.ToString());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eDrawBalloon(void *p)
{
  try
	 {
	   if (!WHandle)
		 throw Exception("Window doesn't exists!");

	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String obj = eIface->GetParamToStr(L"pObjectName");

	   if (obj == "")
		 throw Exception("Can't get main object name!");

	   obj = "&" + obj;

	   String obj_color = eIface->GetObjectProperty(obj.c_str(), L"Color");

	   if (obj_color == "")
		 throw Exception("Can't get Color object name!");

	   String obj_bord = eIface->GetObjectProperty(obj.c_str(), L"Border");

	   if (obj_color == "")
		 throw Exception("Can't get Border object name!");

	   String obj_tail = eIface->GetObjectProperty(obj.c_str(), L"Tail");

	   if (obj_tail == "")
		 throw Exception("Can't get Tail object name!");

	   CurrentBalloon.Left = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Left"));
	   CurrentBalloon.Top = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Top"));
	   CurrentBalloon.Width = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Width"));
	   CurrentBalloon.Height = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Height"));

	   CurrentBalloon.TailWidth = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"TailWidth"));
	   CurrentBalloon.CornerRadius = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Corner"));
	   CurrentBalloon.Shadow = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Shadow"));

	   CurrentBalloon.Color = Gdiplus::Color(_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Alpha")),
											 _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Red")),
											 _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Green")),
											 _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Blue")));

	   CurrentBalloon.Border = _wtoi(eIface->GetObjectProperty(obj_bord.c_str(), L"Size"));

	   CurrentBalloon.Tail.X = _wtoi(eIface->GetObjectProperty(obj_tail.c_str(), L"X"));
	   CurrentBalloon.Tail.Y = _wtoi(eIface->GetObjectProperty(obj_tail.c_str(), L"Y"));

	   obj_color = eIface->GetObjectProperty(obj_bord.c_str(), L"Color");

	   if (obj_color == "")
		 throw Exception("Can't get Border Color object name!");

	   CurrentBalloon.BorderColor = Gdiplus::Color(_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Alpha")),
												   _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Red")),
												   _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Green")),
												   _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Blue")));

	   PostMessage(WHandle, WM_KVL_DRAW_BALLOON, NULL, NULL);

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eDrawBalloon: " + e.ToString());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eDrawCloud(void *p)
{
  try
	 {
	   if (!WHandle)
		 throw Exception("Window doesn't exists!");

	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String obj = eIface->GetParamToStr(L"pObjectName");

	   if (obj == "")
		 throw Exception("Can't get main object name!");

	   obj = "&" + obj;

	   String obj_color = eIface->GetObjectProperty(obj.c_str(), L"Color");

	   if (obj_color == "")
		 throw Exception("Can't get Color object name!");

	   String obj_bord = eIface->GetObjectProperty(obj.c_str(), L"Border");

	   if (obj_color == "")
		 throw Exception("Can't get Border object name!");

	   String obj_tail = eIface->GetObjectProperty(obj.c_str(), L"Tail");

	   if (obj_tail == "")
		 throw Exception("Can't get Tail object name!");

	   CurrentCloud.Left = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Left"));
	   CurrentCloud.Top = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Top"));
	   CurrentCloud.Width = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Width"));
	   CurrentCloud.Height = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Height"));

	   CurrentCloud.TailWidth = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"TailWidth"));
	   CurrentCloud.CornerRadius = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Corner"));
	   CurrentCloud.Shadow = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Shadow"));

	   CurrentCloud.Color = Gdiplus::Color(_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Alpha")),
										   _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Red")),
										   _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Green")),
										   _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Blue")));

	   CurrentCloud.Border = _wtoi(eIface->GetObjectProperty(obj_bord.c_str(), L"Size"));

	   CurrentCloud.Tail.X = _wtoi(eIface->GetObjectProperty(obj_tail.c_str(), L"X"));
	   CurrentCloud.Tail.Y = _wtoi(eIface->GetObjectProperty(obj_tail.c_str(), L"Y"));

	   obj_color = eIface->GetObjectProperty(obj_bord.c_str(), L"Color");

	   if (obj_color == "")
		 throw Exception("Can't get Border Color object name!");

	   CurrentCloud.BorderColor = Gdiplus::Color(_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Alpha")),
												 _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Red")),
												 _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Green")),
												 _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Blue")));

	   PostMessage(WHandle, WM_KVL_DRAW_CLOUD, NULL, NULL);

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eDrawCloud: " + e.ToString());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eDrawText(void *p)
{
  try
	 {
	   if (!WHandle)
		 throw("Window doesn't exists!");

	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String obj = eIface->GetParamToStr(L"pObjectName");

	   if (obj == "")
		 throw Exception("Can't get main object name!");

	   obj = "&" + obj;

	   String obj_font = eIface->GetObjectProperty(obj.c_str(), L"Font");

	   if (obj_font == "")
		 throw Exception("Can't get Font object name!");

	   String obj_color = eIface->GetObjectProperty(obj_font.c_str(), L"Color");

	   if (obj_color == "")
		 throw Exception("Can't get Color object name!");

	   CurrentText.Rect.X = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Left"));
	   CurrentText.Rect.Y = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Top"));
	   CurrentText.Rect.Width = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Width"));
	   CurrentText.Rect.Height = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Height"));
	   wcscpy(CurrentText.Alignment, eIface->GetObjectProperty(obj.c_str(), L"Align"));
	   CurrentText.CenterVerticaly = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"CenterVerticaly"));
	   CurrentText.WordWrap = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"WordWrap"));
	   String text = eIface->GetObjectProperty(obj.c_str(), L"Data");

	   CurrentText.UserFont = _wtoi(eIface->GetObjectProperty(obj_font.c_str(), L"UserFont"));
	   wcscpy(CurrentText.FontName, eIface->GetObjectProperty(obj_font.c_str(), L"Name"));
	   CurrentText.FontSize = _wtoi(eIface->GetObjectProperty(obj_font.c_str(), L"Size"));

	   String style = eIface->GetObjectProperty(obj_font.c_str(), L"Style");

	   if (style == "b")
		 CurrentText.FontStyle = 1;
	   else if (style == "i")
		 CurrentText.FontStyle = 2;
	   else if (style == "bi")
		 CurrentText.FontStyle = 3;
	   else if (style == "u")
		 CurrentText.FontStyle = 4;
	   else if (style == "s")
		 CurrentText.FontStyle = 8;
	   else
		 CurrentText.FontStyle = 0;

	   CurrentText.FontColor = Gdiplus::Color(_wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Alpha")),
											  _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Red")),
											  _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Green")),
											  _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Blue")));


	   TEXT CheckedText = CurrentText;

	   CheckedText.Rect = MeasureTextRect(text.c_str(),
										  CheckedText.Rect,
                                          CheckedText.UserFont,
										  CheckedText.FontName,
										  CheckedText.FontSize,
										  (Gdiplus::FontStyle)CheckedText.FontStyle,
										  CheckedText.Alignment,
										  CheckedText.CenterVerticaly,
										  CheckedText.WordWrap);

	   eIface->SetObjectProperty(obj.c_str(), L"Width", IntToStr((int)CheckedText.Rect.Width + 1).c_str());
	   eIface->SetObjectProperty(obj.c_str(), L"Height", IntToStr((int)CheckedText.Rect.Height + 1).c_str());

	   PostMessage(WHandle, WM_KVL_DRAW_TEXT, (WPARAM)text.c_str(), NULL);

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eDrawText: " + e.ToString());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------
}

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
  if (reason == DLL_PROCESS_ATTACH)
	{
	  DllHinst = hinst;
	}
  else if (reason == DLL_PROCESS_DETACH)
	StopWork();

  return 1;
}
//---------------------------------------------------------------------------
