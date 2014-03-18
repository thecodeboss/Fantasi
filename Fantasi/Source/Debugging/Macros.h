#ifndef Macros_h__
#define Macros_h__

#if _DEBUG
	#define MEMORY_LEAK_CHECKING 1
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>

	#ifndef DBG_NEW
		#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
		#define new DBG_NEW
	#endif
#else
	#define MEMORY_LEAK_CHECKING 0
#endif

#include <windows.h>
#include <sstream>

#define DEBUG_PRINT(msg) if (_DEBUG) { std::wostringstream os_; os_ << msg; OutputDebugStringW( os_.str().c_str() ); }

#endif // Macros_h__
