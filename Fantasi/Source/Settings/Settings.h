#ifndef Settings_h__
#define Settings_h__

struct Settings {

	const char * ProgramTitle;
	unsigned ScreenX, ScreenY;
	bool bFullScreen;

	Settings(const char * program_title, unsigned screen_x, unsigned screen_y, bool fullscreen)
		: ProgramTitle(program_title)
		, ScreenX(screen_x)
		, ScreenY(screen_y)
		, bFullScreen(fullscreen) {}
};

#endif // Settings_h__
