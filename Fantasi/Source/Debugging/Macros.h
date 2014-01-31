#ifndef Macros_h__
#define Macros_h__
#include <windows.h>
#include <sstream>

#define DEBUG_PRINT(msg) if (_DEBUG) { std::wostringstream os_; os_ << msg; OutputDebugStringW( os_.str().c_str() ); }

#endif // Macros_h__
