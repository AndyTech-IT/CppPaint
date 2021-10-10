#pragma once
#include "api.h"
#include <cmath>
#include "CppPaint.h"

int GetClickedVidgetId(int x, int y, bool shift_pressed);
void DrawLine(int x1, int y1, int x2, int y2);
void DrawPoint(int x, int y);