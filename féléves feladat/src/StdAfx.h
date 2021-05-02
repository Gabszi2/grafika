
#ifndef __STDAFX_H__
#define __STDAFX_H__

#ifdef WIN32
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLu32.lib")
#pragma comment(lib, "./lib/SDL/lib/SDLmain.lib")
#pragma comment(lib, "./lib/SDL/lib/SDL.lib")
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>


#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#ifndef DWORD
typedef unsigned long DWORD;
#endif

// Constants
#ifndef M_PI
#define M_PI 3.1415928
#endif

#ifdef _DEBUG
# define ASSERT(f)			assert(f)
# define VERIFY(f)			ASSERT(f)
#else
# define ASSERT(f)			((void)0)
# define VERIFY(f)			((void)(f))
#endif

#endif
