#include "App.h"
#include "engine.h"

int GetClickedVidgetId(int x, int y, bool shift_pressed)
{
	int vidget0_x = 100;
	int vidget0_y = 50;
	int vidget0_width = 300;
	int vidget0_height = 300;

	int vidget1_x = 500;
	int vidget1_y = 150;
	int vidget1_width = 250;
	int vidget1_height = 250;

	int vidget2_x = 280;
	int vidget2_y = 480;
	int vidget2_width = 200;
	int vidget2_height = 200;

	//Write you code here
	
	return -1;
}

#pragma region BlackBox Не открывать!
void DrawPoint(int x, int y) {
	//engine->draw_point(Point{ (double)x, (double)y });
}
#pragma endregion


void DrawLine(int x1, int y1, int x2, int y2)
{
	for (int i = 0; i < x2; i++)
		DrawPoint(i, y1);
}