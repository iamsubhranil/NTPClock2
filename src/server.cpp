#include "server.h"
#include "config.h"
#include "displaymanager.h"
#include "storage.h"
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
		request->send_P(200, "text/html",
		                Storage::read("/config_page.html").c_str());
	});

	webserver.on(
	    "/update_config", HTTP_GET, [](AsyncWebServerRequest *request) {
		    request->send(200, "text/plain", "OK");
		    if(request->hasParam("display_state")) {
			    auto a = request->getParam("display_state");
			    DisplayManager::printScrollingText("Display state changed");
			    if(a->value() == "on") {
				    Configuration::setDisplayState(true);
			    } else if(a->value() == "off") {
				    Configuration::setDisplayState(false);
			    }
		    }
	    });

	AsyncElegantOTA.begin(&webserver);
	webserver.begin();

	String s = WiFi.localIP().toString();
	snprintf(messageBuff, 50, "Visit http://%s to configure..", s.c_str());
	DisplayManager::printScrollingText(messageBuff);

	alreadyInit = true;
}