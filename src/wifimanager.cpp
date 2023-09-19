#include "wifimanager.h"
#include "config.h"
#include "displaymanager.h"
#include "ledmanager.h"
#include "scheduler.h"
#include "server.h"

#include <DNSServer.h>
#include <WiFi.h>

static bool scheduledEnsureConnected = false;

void WiFiManager::init() {

	Configuration::onAPNameChange([](String &newAPName) { reconnect(false); });
	Configuration::onAPPasswordChange(
	    [](String &newAPPassword) { reconnect(false); });
	reconnect();
}

void WiFiManager::setupAP() {
	WiFi.mode(WIFI_AP_STA);
	IPAddress clockIP = IPAddress(192, 168, 3, 1);
	WiFi.softAPConfig(clockIP, clockIP, IPAddress(255, 255, 255, 0));
	WiFi.softAP("ClockAP");

	// setup DNS
	DNSServer dnsServer;
	dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
	dnsServer.start(53 /* port */, "*", clockIP);

	// restart our webserver
	ServerManager::init();
	LEDManager::blink(LEDManager::BLUE);
	DisplayManager::printAnimatingText(
	    {"\x1a   ClockAP   \x1b", "\x1a  ClockAP  \x1b",
	     "\x1a ClockAP \x1b", "\x1a""ClockAP\x1b"},
	    250, []() { return WiFiManager::isConnected(); },
	    [&dnsServer]() { dnsServer.processNextRequest(); },
	    [&dnsServer]() {
		    DisplayManager::clear();
		    DisplayManager::print("WiFi: OK \x02");
		    delay(1000);
		    dnsServer.stop();
		    WiFi.mode(WIFI_STA);
		    // restart our server
		    ServerManager::init();
		    LEDManager::off(LEDManager::BLUE);
	    });
}

void ensureConnection() {
	int start_mills       = millis();
	int elapsed_mills     = 0;
	int check_after_mills = 500;
	// try for 10 seconds
	int connection_timeout = 10 * 1000;

	DisplayManager::printAnimatingText(
	    {"WiFi: \x07", "WiFi: \x09", "WiFi: \x0a", "WiFi: \x08"}, 250,
	    [&elapsed_mills, &connection_timeout]() {
		    return WiFiManager::isConnected() ||
		           elapsed_mills > connection_timeout;
	    },
	    [&elapsed_mills, &start_mills]() {
		    elapsed_mills = millis() - start_mills;
	    },
	    []() {
		    if(!WiFiManager::isConnected()) {
			    DisplayManager::print("WiFi: Failed \x01");
			    WiFiManager::setupAP();
		    } else {
			    DisplayManager::print("WiFi: OK \x02");
			    delay(1000);
		    }
	    });
	scheduledEnsureConnected = false;
}

void WiFiManager::reconnect(bool instant) {
	WiFi.disconnect();
	WiFi.begin(Configuration::getAPName().c_str(),
	           Configuration::getAPPassword().c_str());

	if(instant) {
		ensureConnection();
		return;
	}

	if(!scheduledEnsureConnected) {
		scheduledEnsureConnected = true;
		Scheduler::every(10).second().perform(ensureConnection, true);
	}
}

bool WiFiManager::isConnected() {
	return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::ensureConnected() {
	while(!isConnected()) {
		reconnect();
	}
}
