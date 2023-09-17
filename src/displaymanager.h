#pragma once

#include <MD_Parola.h>

struct DisplayManager {
	static MD_Parola display;
	static void      init();
	static MD_Parola getDisplay() { return display; }
	static void      clear() { display.displayClear(); }
	static void      print(const char *text) { display.print(text); }
	static void      println(const char *text) { display.println(text); }
	static void		 shutdownDisplay(bool value);

	template <typename U, typename D, typename C>
	static void printScrollingText(const String &str, U until, D doThen,
	                               C onComplete) {
		printScrollingText(str.c_str(), until, doThen, onComplete);
	}
	template <typename U, typename D, typename C>
	static void printScrollingText(const char *text, U until, D doThen,
	                               C onComplete) {
		auto a = display.getTextAlignment();
		display.displayClear();
		display.displayScroll(text, PA_RIGHT, PA_SCROLL_LEFT, 10);
		while(!until()) {
			if(display.displayAnimate())
				display.displayReset();
			doThen();
			// 30fps
			delay(32);
		}
		display.setTextAlignment(a);
		onComplete();
	}

	template <typename U, typename D, typename C>
	static void printTextWithEffect(const char *text, U until, D doThen,
	                                C            onComplete,
	                                textEffect_t effectIn  = PA_RANDOM,
	                                textEffect_t effectOut = PA_RANDOM) {
		auto a = display.getTextAlignment();
		display.displayClear();
		display.displayText(text, PA_CENTER, 7, 30, effectIn, effectOut);
		while(!until()) {
			if(display.displayAnimate())
				display.displayReset();
			doThen();
			// 30fps
			delay(32);
		}
		display.setTextAlignment(a);
		onComplete();
	}

	static void printTextWithEffect(const String &str, int durationMillis,
	                                textEffect_t effectIn  = PA_RANDOM,
	                                textEffect_t effectOut = PA_RANDOM) {
		printTextWithEffect(str.c_str(), durationMillis, effectIn, effectOut);
	}

	static void printTextWithEffect(const char *str, int durationMillis,
	                                textEffect_t effectIn  = PA_RANDOM,
	                                textEffect_t effectOut = PA_RANDOM) {
		int end = millis() + durationMillis;
		printTextWithEffect(
		    str, [end]() { return millis() > end; }, []() {}, []() {}, effectIn,
		    effectOut);
	}

	static void printTextWithEffect(const char  *str,
	                                textEffect_t effectIn  = PA_RANDOM,
	                                textEffect_t effectOut = PA_RANDOM) {
		printTextWithEffect(
		    str, []() { return display.displayAnimate(); }, []() {}, []() {},
		    effectIn, effectOut);
	}

	static void printTextWithEffect(const String &str,
	                                textEffect_t  effectIn  = PA_RANDOM,
	                                textEffect_t  effectOut = PA_RANDOM) {
		printTextWithEffect(
		    str.c_str(), []() { return display.displayAnimate(); }, []() {},
		    []() {}, effectIn, effectOut);
	}

	template <typename U, typename D, typename C>
	static void printAnimatingText(std::initializer_list<const char *> text,
	                               int frameTimeMs, U until, D doThen,
	                               C onComplete) {
		auto a = display.getTextAlignment();
		display.displayClear();
		display.setTextAlignment(PA_CENTER);
		auto currentText = text.begin();
		while(!until()) {
			display.displayClear();
			display.printf("%s", *currentText);
			doThen();
			currentText++;
			if(currentText == text.end())
				currentText = text.begin();
			delay(frameTimeMs);
		}
		display.displayClear();
		display.setTextAlignment(a);
		onComplete();
	}

	static void printScrollingText(const String &str, int duration) {
		printScrollingText(str.c_str(), duration);
	}
	static void printScrollingText(const char *text, int duration_millis) {
		int end = millis() + duration_millis;
		printScrollingText(
		    text, [end]() { return millis() > end; }, []() {}, []() {});
	}

	static void printScrollingText(const String &str) {
		printScrollingText(str.c_str());
	}
	static void printScrollingText(const char *text) {
		printScrollingText(
		    text, []() { return display.displayAnimate(); }, []() {}, []() {});
	}
};