#pragma once

#include <SPIFFS.h>

struct Storage {

	static bool initialized;

	static void   init();
	static bool   isInitialized();
	static File   open(const char *path, const char *mode);
	static bool   exists(const char *path);
	static String read(const char *path);
	static bool   write(const char *path, const char *data);
};