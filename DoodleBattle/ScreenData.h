#ifndef SCREEN_DATA
#define SCREEN_DATA

#include <map>

class ScreenData
{
	public:
		ScreenData() {};
		virtual ~ScreenData() {};
		std::map<const char*, unsigned int> intParameters;
};

#endif