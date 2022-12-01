#pragma once

struct WiFiManager {
	static void init();
	static bool isConnected();
	static void ensureConnected();
	static void setupAP();
	static void reconnect();
};