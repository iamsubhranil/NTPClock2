#include "server.h"
#include "config.h"
#include "displaymanager.h"
#include "storage.h"
#include "wifimanager.h"
#include <AsyncElegantOTA.h>
#include <WiFi.h>

AsyncWebServer ServerManager::webserver(80);

bool alreadyInit     = false;
char messageBuff[50] = {0};

String generateField(String id, String label, const String &value,
                     String inputType) {

	/*

	        <div class="col-12">
	        <div class="form-check">
	            <input class="form-check-input" type="checkbox" id="gridCheck">
	            <label class="form-check-label" for="gridCheck">
	            Check me out
	            </label>
	        </div>
	*/
	if(inputType == "checkbox") {
		return "<div class=\"col-12\"><div class=\"form-check\"><input "
		       "class=\"form-check-input\" type=\"" +
		       inputType + "\" id=\"" + id + "\" " + " name=\"" + id + "\" " +
		       (value == "1" ? "checked" : "") +
		       "><label class=\"form-check-label\" for=\"" + id + "\">" +
		       label + "</label></div></div>";
	}
	return "<div class=\"col-12\"><label for=\"" + id +
	       "\" class=\"form-label\">" + label + "</label><input type=\"" +
	       inputType + "\" class=\"form-control\" id=\"" + id + "\" " +
	       "name=\"" + id + "\" value=\"" + value + "\"></div>";
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
	String ret = "";
#define OPTION(name, type, defaultValue, displayName) \
	ret += generateFieldFor##type(#name, displayName, \
	                              Configuration::get##name());
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
		webserver.reset();
	}
	webserver.on("/", HTTP_ANY, [](AsyncWebServerRequest *request) {
		request->send(SPIFFS, "/config_modern.html", String(), false,
		              generateHTML);
	});
	webserver.onNotFound([](AsyncWebServerRequest *request) {
		request->send(SPIFFS, "/config_modern.html", String(), false,
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

	if(WiFiManager::isConnected()) {
		IPAddress ip = WiFi.localIP();
		String    s  = ip.toString();
		snprintf(messageBuff, 50, "Visit http://%s to configure..", s.c_str());
		DisplayManager::printScrollingText(messageBuff);
	}

	alreadyInit = true;
}