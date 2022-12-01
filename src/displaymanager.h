#pragma once

#include <MD_Parola.h>

struct DisplayManager {
	static MD_Parola display;
	static void      init();
	static MD_Parola getDisplay() { return display; }
	static void      clear() { display.displayClear(); }
	static void      print(const char *text) { display.print(text); }
	static void      println(const char *text) { display.println(text); }

	template <typename U, typename D, typename C>
	static void printScrollingText(const char *text, U until, D doThen,
	                               C onComplete) {
		display.displayClear();
		display.displayScroll(text, PA_CENTER, PA_SCROLL_LEFT, 20);
		while(until()) {
			display.displayAnimate();
			doThen();
			// 30fps
			delay(32);
		}
		onComplete();
	}
};