#include "server.h"
#include <AsyncElegantOTA.h>

AsyncWebServer ServerManager::webserver(80);

void ServerManager::init() {
	webserver.on("/", HTTP_ANY, [](AsyncWebServerRequest *request) {
		request->redirect("/update");
	});

	AsyncElegantOTA.begin(&webserver);
	webserver.begin();
}