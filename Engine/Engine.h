#pragma once
#include <cstdio>

//#define RELEASE_BUILD

#ifndef WIN32
#define _ANDROID
#endif

#ifdef _ANDROID

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <android/log.h>
	#ifndef RELEASE_BUILD
		#define  LOG_TAG    "GAME"
		#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
		#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
	#else
		#define LOGI //
		#define LOGE //
	#endif
#else

#ifdef _WIN32

#include <GLES2\gl2.h>

	#ifndef RELEASE_BUILD
		#define LOGI(...) printf(__VA_ARGS__)
		#define LOGE(...) printf(__VA_ARGS__)
	#else
		#define LOGI //
		#define LOGE //
	#endif

#endif
#endif


#include "TGA.h"

#define SCREEN_W 800
#define SCREEN_H 480

#define SAFE_DEL(p) if (p) delete p; p = 0;
#define SAFE_DEL_ARRAY(p) if (p) delete []p; p = 0;

#include <string>
struct DATA_LINE
{
	std::string key;
	std::string value;
};
