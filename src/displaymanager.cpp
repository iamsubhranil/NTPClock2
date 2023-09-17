#include "displaymanager.h"
#include "config.h"
#include "font.h"

// Defining size, and output pins
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 8
#define CS_PIN 5

MD_Parola DisplayManager::display =
    MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

const String Greetings[] = {"Hiya, ",    "Hello, ",     "Greetings, ",
                            "Welcome, ", "What's up, ", "Howdy, ",
                            "Hey, "};

const textEffect_t Effects[] = {
    PA_SCROLL_UP,    ///< Text scrolls up through the display
    PA_SCROLL_DOWN,  ///< Text scrolls down through the display
    PA_SCROLL_LEFT,  ///< Text scrolls right to left on the display
    PA_SCROLL_RIGHT, ///< Text scrolls left to right on the display
#if ENA_SPRITE
    PA_SPRITE, ///< Text enters and exits using user defined sprite
#endif
#if ENA_MISC
    // PA_SLICE,    ///< Text enters and exits a slice (column) at a time from the
                 ///< right
    PA_MESH,     ///< Text enters and exits in columns moving in alternate
                 ///< direction (U/D)
    PA_FADE,     ///< Text enters and exits by fading from/to 0 and intensity
                 ///< setting
    PA_DISSOLVE, ///< Text dissolves from one display to another
    // PA_BLINDS,   ///< Text is replaced behind vertical blinds
    // PA_RANDOM,   ///< Text enters and exits as random dots
#endif           // ENA_MISC
#if ENA_WIPE
    // PA_WIPE, ///< Text appears/disappears one column at a time, looks like it
             ///< is wiped on and off
        // PA_WIPE_CURSOR, ///< WIPE with a light bar ahead of the change
#endif              // ENA_WIPES
#if ENA_SCAN
    PA_SCAN_HORIZ,  ///< Scan the LED column one at a time then
                    ///< appears/disappear at end
    PA_SCAN_HORIZX, ///< Scan a blank column through the text one column at a
                    ///< time then appears/disappear at end
    PA_SCAN_VERT,  ///< Scan the LED row one at a time then appears/disappear at
                   ///< end
    PA_SCAN_VERTX, ///< Scan a blank row through the text one row at a time
                   ///< then appears/disappear at end
#endif             // ENA_SCAN
#if ENA_OPNCLS
    PA_OPENING,        ///< Appear and disappear from the center of the display,
                       ///< towards the ends
    PA_OPENING_CURSOR, ///< OPENING with light bars ahead of the change
    PA_CLOSING, ///< Appear and disappear from the ends of the display, towards
                ///< the middle
    PA_CLOSING_CURSOR, ///< CLOSING with light bars ahead of the change
#endif                 // ENA_OPNCLS
#if ENA_SCR_DIA
    PA_SCROLL_UP_LEFT,    ///< Text moves in/out in a diagonal path up and left
                          ///< (North East)
    PA_SCROLL_UP_RIGHT,   ///< Text moves in/out in a diagonal path up and right
                          ///< (North West)
    PA_SCROLL_DOWN_LEFT,  ///< Text moves in/out in a diagonal path down and
                          ///< left (South East)
    PA_SCROLL_DOWN_RIGHT, ///< Text moves in/out in a diagonal path down and
                          ///< right (North West)
#endif                    // ENA_SCR_DIA
#if ENA_GROW
    PA_GROW_UP,   ///< Text grows from the bottom up and shrinks from the top
                  ///< down
    PA_GROW_DOWN, ///< Text grows from the top down and and shrinks from the
                  ///< bottom up
#endif            // ENA_GROW};
};

void DisplayManager::shutdownDisplay(bool val) {
    display.displayShutdown(val);
}

void DisplayManager::init() {
	display.begin(0);
	display.setIntensity(0);
	display.displayClear();
	display.setFont(ClockFont);
	int idx       = random(0, sizeof(Greetings) / sizeof(String));
	int inEffect  = random(0, sizeof(Effects) / sizeof(textEffect_t));
	int outEffect = random(0, sizeof(Effects) / sizeof(textEffect_t));
	printTextWithEffect(Greetings[idx] + Configuration::getUsername() + "!",
	                    Effects[inEffect], Effects[outEffect]);
	display.setTextAlignment(PA_CENTER);
	Configuration::onDisplayStateChange(
	    [](bool &state) { display.displayShutdown(!state); });
}
