#include "haze_framework.h"

int main(int argc ,char **argv)
{
	assert(hgs.init(argc ,argv) >= 0);

	hgs.signal();
	
	hgs.cycle();

	hgs.exit();
	return 0;
}
