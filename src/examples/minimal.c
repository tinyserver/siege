#include <siege/siege.h>

int main(void)
{
	sgLoadModules(2, "SDL", "OpenGL");
	sgInit(640, 480, 32, 0);

	SGint ret = sgRun();
	
	sgDeinit();

	return ret;
}
