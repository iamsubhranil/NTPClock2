#include "server.h"
#include <AsyncElegantOTA.h>

AsyncWebServer ServerManager::webserver(80);

bool alreadyInit = false;

void ServerManager::init() {
	if(alreadyInit) {
		webserver.end();
	}
	webserver.on("/", HTTP_ANY, [](AsyncWebServerRequest *request) {
		request->redirect("/update");
	});

	AsyncElegantOTA.begin(&webserver);
	webserver.begin();
	alreadyInit = true;
}