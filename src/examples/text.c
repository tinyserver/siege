#include <siege/siege.h>

#include <string.h>
#include <stdlib.h>
#include <math.h>

SGFont* sizes[12];
const size_t numsizes = sizeof(sizes) / sizeof(SGFont*);
const char* text = "A Quick Brown Fox Jumps Over The Lazy Dog 0123456789";

SGFont* font;
int main(void)
{
	sgLoadModule("SDL");
	sgLoadModule("OpenGL");
	sgLoadModule("Freetype");
	sgLoadModule("IConv");
	sgInit(640, 480, 32, 0);
	sgWindowSetTitle("SIEGE Text Demo");
	
	SGuint i;
	for(i = 0; i < numsizes; i++)
		sizes[i] = sgFontCreate("data/fonts/DejaVuSans.ttf", 7 + i, 127);

	font = sgFontCreate("data/fonts/DejaVuSans.ttf", 10, 127);

	SGuint width = sgWindowGetWidth();
	SGuint height = sgWindowGetHeight();

	//float wx, wy;
	//sgFontStrSize(font, &wx, &wy, "W");

	float dx, dy;
	float y;
	
	SGint mx, my;
	size_t pos;

	while(sgLoop(NULL))
	{
		sgMouseGetPos(&mx, &my);
		
		sgDrawColor4f(1.0, 1.0, 1.0, 1.0);
		y = 0.0;
		for(i = 0; i < numsizes; i++)
		{
			sgFontStrSize(sizes[i], &dx, &dy, text);
			y += dy;
			sgFontPrint(sizes[i], 0, floor(y), text);
		}

		sgDrawColor4f(1.0, 0.0, 0.0, 1.0);

		sgFontPrintf(font, width / 2, height - 64, "Mouse center angle: %f", atan2(my - (float)height / 2, mx - (float)width / 2) * 180.0 / SG_PI);

		sgDrawColor4f(0.0, 1.0, 0.0, 1.0);
		sgFontPrintf(font, width / 2, height - 92, "Mouse: %d,%d %d", mx, my, sgMouseGetWheel());

		sgFontPrintf(font, 128, height - 128, "AAA\nBBB\nCCC\nDDD\n\nEEE");

		sgDrawColor4f(0.0, 0.5, 0.75, 0.75);
		sgFontPrintf(font, width / 4, height - height / 3, "Well, this is some more text...\nUseful for UI, consoles, etc...");
		
		pos = sgFontFindIndexf(font, mx - (SGint)(width / 4), my - (SGint)(height - height / 3), "Well, this is some more text...\nUseful for UI, consoles, etc...");
		sgFontGetPos(font, &dx, &dy, pos, "Well, this is some more text...\nUseful for UI, consoles, etc...");
		dx += width / 4;
		dy += height - height / 3;
		sgDrawColor4f(1.0, 0.0, 0.0, 1.0);
		sgDrawLine(dx, dy, dx, dy - 10);

		sgDrawColor4f(1.0, 1.0, 1.0, 1.0);

		// thanks to Wikipedia for these!
		sgFontPrintW(font, 2, 256, L"Testing: ABC abc 012\nčšž\nSome cyrillic: Съешь ещё этих мягких французских булок да выпей же чаю\nAnd now for greek: Τάχιστη αλώπηξ βαφής ψημένη γη, δρασκελίζει υπέρ νωθρού κυνός");
        sgFontPrintfW(font, 2, 328, L"sgFontPrintfW: %ls", L"čšž");
        
		sgFontStrSizef(font, &dx, &dy, "Some test text");
		sgDrawColor4f(1.0, 1.0, 0.0, 1.0);
		sgDrawLine(640 - 160, 480 - 120, 640 - 160 + dx, 480 - 120);
		sgDrawRectangle(640 / 2 - dx / 2, 480 - 120 - dy / 2,
						640 / 2 + dx / 2, 480 - 120 + dy / 2,
						SG_TRUE);
		sgDrawColor4f(1.0, 1.0, 1.0, 1.0);
		sgFontPrint(font, 640 - 160, 480 - 120, "Some test text");

		sgDrawColor4f(0.0, 0.0, 1.0, 1.0);
		sgDrawRectangle(640 / 2 - dx / 2, 480 - 120 - dy / 2,
						640 / 2 + dx / 2, 480 - 120 + dy / 2,
						SG_TRUE);
		sgDrawColor4f(1.0, 1.0, 0.0, 1.0);
		sgDrawRectangle(640 / 2 - dx / 2, 480 - 120 - dy / 2,
						640 / 2 + dx / 2, 480 - 120 + dy / 2,
						SG_FALSE);
		sgDrawColor4f(1.0, 1.0, 1.0, 1.0);
		sgFontPrintCentered(font, 640 / 2, 480 - 120, "Some test text");

		sgWindowSwapBuffers();
		sgDrawClear();
	}

	sgFontDestroy(font);

	for(i = 0; i < numsizes; i++)
		sgFontDestroy(sizes[i]);

	sgDeinit();

	return 0;
}
