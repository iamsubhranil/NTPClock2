#include "server.h"
#include "displaymanager.h"
#include <AsyncElegantOTA.h>

#include <WiFi.h>

AsyncWebServer ServerManager::webserver(80);

bool alreadyInit     = false;
char messageBuff[50] = {0};

void ServerManager::init() {
	if(alreadyInit) {
		webserver.end();
	}
	webserver.on("/", HTTP_ANY, [](AsyncWebServerRequest *request) {
		request->redirect("/update");
	});

	AsyncElegantOTA.begin(&webserver);
	webserver.begin();

	String s = WiFi.localIP().toString();
	snprintf(messageBuff, 50, "Visit http://%s to configure..", s.c_str());
	DisplayManager::printScrollingText(messageBuff);

	alreadyInit = true;
}