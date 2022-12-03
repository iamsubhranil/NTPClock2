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
		auto a = display.getTextAlignment();
		display.displayClear();
		display.displayScroll(text, PA_RIGHT, PA_SCROLL_LEFT, 10);
		while(until()) {
			if(display.displayAnimate())
				display.displayReset();
			doThen();
			// 30fps
			delay(32);
		}
		onComplete();
		display.setTextAlignment(a);
	}

	static void printScrollingText(const char *text, int duration_millis) {
		int end = millis() + duration_millis;
		printScrollingText(
		    text, [end]() { return millis() < end; }, []() {}, []() {});
	}

	static void printScrollingText(const char *text) {
		printScrollingText(
		    text, []() { return !display.displayAnimate(); }, []() {}, []() {});
	}
};