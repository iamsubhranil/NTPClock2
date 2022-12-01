#pragma once

#include <ESPAsyncWebServer.h>

struct ServerManager {

	static AsyncWebServer webserver;

	static void init();
};