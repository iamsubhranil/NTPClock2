#include "server.h"
#include "config.h"
#include "displaymanager.h"
#include "storage.h"
#include <AsyncElegantOTA.h>
#include <WiFi.h>

AsyncWebServer ServerManager::webserver(80);

bool alreadyInit     = false;
char messageBuff[50] = {0};

String generateField(String id, String label, const String &value,
                     String inputType) {

	return "<label>" + label + " <input type=\"" + inputType + "\" name=\"" +
	       id + "\" id=\"" + id + "\" value=\"" + value + "\"" +
	       ((inputType == "checkbox" && value == "1") ? "checked" : "") +
	       "></label>";
}

String generateFieldForString(String id, String label, const String &value) {
	return generateField(id, label, value, "text");
}

String generateFieldForint(String id, String label, const int &value) {
	return generateField(id, label, String(value), "number");
}

String generateFieldForbool(String id, String label, const bool &value) {
	return generateField(id, label, String(value), "checkbox");
}

String generateHTML(const String &replace) {
	String ret = "<br><br>";
#define OPTION(name, type, defaultValue, displayName)           \
	ret += generateFieldFor##type(#name, displayName,           \
	                              Configuration::get##name()) + \
	       "<br><br>";
#include "config_options.h"
	return ret;
}

String parseString(const String &str) {
	return String(str);
}

int parseint(const String &str) {
	return str.toInt();
}

bool parsebool(const String &str) {
	return str.toInt();
}

void ServerManager::init() {
	if(alreadyInit) {
		webserver.end();
	}
	webserver.on("/", HTTP_ANY, [](AsyncWebServerRequest *request) {
		request->send(SPIFFS, "/config_page.html", String(), false,
		              generateHTML);
	});

	webserver.on("/update_config", HTTP_POST,
	             [](AsyncWebServerRequest *request) {
#define OPTION(name, type, defaultValue, displayName)                    \
	if(request->hasParam(#name, true)) {                                 \
		Configuration::set##name(                                        \
		    #type == "bool"                                              \
		        ? parse##type("1")                                       \
		        : parse##type(request->getParam(#name, true)->value())); \
	} else if(#type == "bool") {                                         \
		Configuration::set##name(parse##type("0"));                      \
	}
#include "config_options.h"
		             request->redirect("/");
		             // request->send(200, "text/plain", "OK");
	             });

	webserver.on("/reboot", HTTP_ANY, [](AsyncWebServerRequest *request) {
		request->send(200, "text/plain", "Restarting Cloak..");
		ESP.restart();
	});

	AsyncElegantOTA.begin(&webserver);
	webserver.begin();

	String s = WiFi.localIP().toString();
	snprintf(messageBuff, 50, "Visit http://%s to configure..", s.c_str());
	DisplayManager::printScrollingText(messageBuff);

	alreadyInit = true;
}