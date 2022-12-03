#include "wifimanager.h"
#include "config.h"
#include "displaymanager.h"
#include "server.h"

#include <DNSServer.h>
#include <WiFi.h>

void WiFiManager::init() {

	Configuration::onAPNameChange([](String &newAPName) { reconnect(); });
	Configuration::onAPPasswordChange(
	    [](String &newAPPassword) { reconnect(); });

	WiFi.mode(WIFI_STA);
	WiFi.begin(Configuration::getAPName().c_str(),
	           Configuration::getAPPassword().c_str());
	int elapsed_mills     = 0;
	int check_after_mills = 500;
	// try for 10 seconds
	int connection_timeout = 10 * 1000;

	DisplayManager::print("Connecting..");
	while(!isConnected() && elapsed_mills < 10000) {
		delay(check_after_mills);
		elapsed_mills += check_after_mills;
	}

	if(!isConnected()) {
		DisplayManager::printScrollingText("Failed!");
		setupAP();
	} else {
		DisplayManager::printScrollingText("Connected!");
	}
}

void WiFiManager::setupAP() {
	WiFi.mode(WIFI_AP_STA);
	IPAddress googleDNS = IPAddress(8, 8, 8, 8);
	WiFi.softAPConfig(googleDNS, googleDNS, IPAddress(255, 255, 255, 0));
	WiFi.softAP("ClockAP");

	// setup DNS
	DNSServer dnsServer;
	dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
	dnsServer.start(53 /* port */, "*", googleDNS);

	// start our webserver if not yet started
	ServerManager::init();

	DisplayManager::printScrollingText(
	    "Connect to ClockAP to configure..",
	    []() { return WiFiManager::isConnected(); },
	    [&dnsServer]() { dnsServer.processNextRequest(); },
	    [&dnsServer]() {
		    DisplayManager::clear();
		    DisplayManager::print("Connected!");
		    delay(1000);
		    dnsServer.stop();
		    WiFi.mode(WIFI_STA);
		    reconnect();
		    while(!isConnected()) {
		    }
		    // restart our server
		    ServerManager::init();
	    });
}

void WiFiManager::reconnect() {
	WiFi.disconnect();
	WiFi.begin(Configuration::getAPName().c_str(),
	           Configuration::getAPPassword().c_str());
}

bool WiFiManager::isConnected() {
	return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::ensureConnected() {
	while(!isConnected()) {
		init();
	}
}
