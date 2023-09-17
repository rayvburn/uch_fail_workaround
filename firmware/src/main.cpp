#include <Arduino.h>
#include "uch_overlay_logic.h"

// Pins as args
UchOverlayLogic logic(D7, D5, true, false);

void setup() {
	Serial.begin(115200);
	Serial.println("Start");
}

void loop() {
	logic.update();
	// logic.test();
}
