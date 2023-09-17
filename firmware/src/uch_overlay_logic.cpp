#include "uch_overlay_logic.h"

#include <Arduino.h> // Serial

UchOverlayLogic::UchOverlayLogic(
	uint8_t indoor_power_factory_input_pin,
	uint8_t indoor_power_ctrl_output_pin,
	bool inverse_input_logic,
	bool inverse_output_logic
):
	indoor_power_factory_input_pin_(indoor_power_factory_input_pin),
	inverse_input_logic_(inverse_input_logic),
	indoor_power_ctrl_output_pin_(indoor_power_ctrl_output_pin),
	output_indoor_pwr_factory_(inverse_output_logic ? HIGH : LOW),
	output_indoor_pwr_shut_off_(inverse_output_logic ? LOW : HIGH),
	state_(IDLE),
	state_changed_(true),
	timestamp_last_print_(0),
	timestamp_last_test_(0),
	timestamp_last_print_in_state_(0),
	input_so_far_(0), // arbitrary at this point
	input_new_(0),
	timestamp_last_input_change_(0),
	timestamp_shutoff_start_(0),
	duration_last_pulse_(0)
{
	// hardware
	pinMode(indoor_power_factory_input_pin_, INPUT);
	pinMode(indoor_power_ctrl_output_pin_, OUTPUT);

	applyFactoryIndoorPower();
	input_so_far_ = readInputFactoryIndoorPower();
	input_new_ = input_so_far_;
}

void UchOverlayLogic::update() {
	auto timestamp = millis();
	input_new_ = readInputFactoryIndoorPower();

	bool input_changed = input_so_far_ != input_new_;

	// periodic logging
	if (timestamp - timestamp_last_print_ >= PERIODIC_PRINT_TIME) {
		Serial.printf(
			"[%lu] Input state: %d\r\n",
			timestamp,
			input_new_
		);
		timestamp_last_print_ = millis();
	}

	if (input_changed) {
		Serial.printf(
			"[%lu] Input changed from: %d to: %d (%lu ms since last change)\r\n",
			timestamp,
			input_so_far_,
			input_new_,
			timestamp - timestamp_last_input_change_
		);
		// save for later
		if (
			input_so_far_ == INPUT_INDOOR_POWER_ENABLE
			&& input_new_ == !INPUT_INDOOR_POWER_ENABLE
		) {
			duration_last_pulse_ = timestamp - timestamp_last_input_change_;
		}
		// update
		input_so_far_ = input_new_;
	}

	// conditions - select state
	selectState(timestamp, input_changed);
	switch (state_) {
		case (IDLE):
			stateHandlerIdle();
			break;
		case (COUNTING_PULSE_DURATION):
			stateHandlerCountingPulseDuration();
			break;
		case (WAITING_SHUT_OFF):
			stateHandlerWaitingShutOff();
			break;
		case (SHUT_OFF):
			stateHandlerShutOff();
			break;
	}

	// finalize loop execution
	if (input_changed) {
		timestamp_last_input_change_ = timestamp;
	}
}

void UchOverlayLogic::test() {
	auto timestamp = millis();
	if (timestamp - timestamp_last_test_ < PERIODIC_PRINT_TIME) {
		return;
	}
	Serial.printf(
		"[%lu][TEST] Relay Factory IN %d | Relay Control OUT %s\r\n",
		timestamp,
		readInputFactoryIndoorPower(),
		(readOutputFactoryIndoorPower() == output_indoor_pwr_factory_) ? "(FACTORY)" : "(OVERRIDE)"
	);
	timestamp_last_test_ = timestamp;
}

void UchOverlayLogic::reset() {
	state_ = IDLE;
	state_changed_ = true;
	timestamp_last_input_change_ = 0;
	timestamp_shutoff_start_ = 0;
	duration_last_pulse_ = 0;
}

void UchOverlayLogic::stateHandlerIdle() {
	applyFactoryIndoorPower();
}

void UchOverlayLogic::stateHandlerCountingPulseDuration() {
	applyFactoryIndoorPower();
	// counting...
}

void UchOverlayLogic::stateHandlerWaitingShutOff() {
	applyFactoryIndoorPower();
}

void UchOverlayLogic::stateHandlerShutOff() {
	cutOffIndoorPower();
}

void UchOverlayLogic::selectState(unsigned long timestamp, bool input_changed) {
	bool log_periodic = (timestamp - timestamp_last_print_in_state_) >= PERIODIC_PRINT_TIME;

	switch (state_) {
		case (IDLE): {
			enterState();
			// check terminal conditions
			if (input_changed && input_new_ == INPUT_INDOOR_POWER_ENABLE) {
				Serial.printf(
					"[%lu][IDLE] Input change detected, switching to COUNTING_PULSE_DURATION state\r\n",
					timestamp
				);
				changeState(COUNTING_PULSE_DURATION);
				// save timestamp
				timestamp_last_input_change_ = timestamp;
			}
			break;
		}

		case (COUNTING_PULSE_DURATION): {
			enterState();
			if (!input_changed) {
				break;
			}
			// input has changed, let's check new state (we expect specific)
			if (input_new_ == INPUT_INDOOR_POWER_ENABLE) {
				Serial.printf(
					"[%lu][COUNTING_PULSE_DURATION] Input has changed but the new input is HIGH which is wrong...\r\n",
					timestamp
				);
				// fallback
				changeState(IDLE);
				break;
			}
			// check pulse duration
			if (
				duration_last_pulse_ < DURATION_INDOOR_PWR_PULSE_MIN
				|| duration_last_pulse_ > DURATION_INDOOR_PWR_PULSE_MAX
			) {
				Serial.printf(
					"[%lu][COUNTING_PULSE_DURATION] Pulse duration was %lu ms - invalid as we expect %lu-%lu ms\r\n",
					timestamp,
					duration_last_pulse_,
					DURATION_INDOOR_PWR_PULSE_MIN,
					DURATION_INDOOR_PWR_PULSE_MAX
				);
				changeState(IDLE);
				break;
			}
			Serial.printf(
				"[%lu][COUNTING_PULSE_DURATION] Pulse duration was %lu ms - now we're waiting for shut off signal\r\n",
				timestamp,
				duration_last_pulse_
			);
			changeState(WAITING_SHUT_OFF);
			break;
		}

		case (WAITING_SHUT_OFF): {
			enterState();

			auto duration_since_last_input_change = timestamp - timestamp_last_input_change_;
			if (duration_since_last_input_change < TIME_TO_SHUT_OFF) {
				// waiting for the first pulse
				if (log_periodic) {
					Serial.printf(
						"[%lu][WAITING_SHUT_OFF] Waiting for the first pulse: %lu/%lu\r\n",
						timestamp,
						duration_since_last_input_change,
						TIME_TO_SHUT_OFF
					);
				}
				break;
			}
			if (duration_since_last_input_change > (TIME_TO_SHUT_OFF + TIME_TO_SHUT_OFF_TOLERANCE)) {
				// we did not receive the pulse...
				Serial.printf(
					"[%lu][WAITING_SHUT_OFF] Timeout has expired and we did not receive the pulse. "
					"Switching to the idle state\r\n",
					timestamp
				);
				changeState(IDLE);
				break;
			}
			// we're at the right time range, expecting the pulse to arrive
			if (!input_changed) {
				break;
			}
			if (input_new_ != INPUT_INDOOR_POWER_ENABLE) {
				Serial.printf(
					"[%lu][WAITING_SHUT_OFF] We're in the right time range but the pulse has changed to wrong "
					"signal (%d and we expect %d). Switching to the idle state\r\n",
					timestamp,
					input_new_,
					INPUT_INDOOR_POWER_ENABLE
				);
				changeState(IDLE);
				break;
			}
			Serial.printf(
				"[%lu][WAITING_SHUT_OFF] The expected pulse received correctly\r\n",
				timestamp
			);
			changeState(SHUT_OFF);
			break;
		}

		case (SHUT_OFF): {
			if (enterState()) {
				timestamp_shutoff_start_ = timestamp;
				Serial.printf(
					"[%lu][SHUT_OFF] Started shutting off the indoor power\r\n",
					timestamp
				);
			}
			auto shutoff_duration = timestamp - timestamp_shutoff_start_;
			if (shutoff_duration < DURATION_SHUT_OFF) {
				// waiting for the first pulse
				if (log_periodic) {
					Serial.printf(
						"[%lu][WAITING_SHUT_OFF] Waiting for the end of shut off: %lu/%lu\r\n",
						timestamp,
						shutoff_duration,
						DURATION_SHUT_OFF
					);
				}
				break;
			}
			Serial.printf(
				"[%lu][SHUT_OFF] Indoor power shut off finished\r\n",
				timestamp
			);
			changeState(IDLE);
			break;
		}
	}

	if (log_periodic) {
		timestamp_last_print_in_state_ = timestamp;
	}
}

void UchOverlayLogic::changeState(State new_state) {
	state_ = new_state;
	state_changed_ = true;
}

bool UchOverlayLogic::enterState() {
	if (!state_changed_) {
		return false;
	}
	state_changed_ = false;
	return true;
}

int UchOverlayLogic::readInputFactoryIndoorPower() const {
	auto raw_read = digitalRead(indoor_power_factory_input_pin_);
	return (inverse_input_logic_) ? (!raw_read) : (raw_read);
}

int UchOverlayLogic::readOutputFactoryIndoorPower() const {
	return digitalRead(indoor_power_ctrl_output_pin_);
}

void UchOverlayLogic::cutOffIndoorPower() {
	digitalWrite(indoor_power_ctrl_output_pin_, output_indoor_pwr_shut_off_);
}

void UchOverlayLogic::applyFactoryIndoorPower() {
	digitalWrite(indoor_power_ctrl_output_pin_, output_indoor_pwr_factory_);
}
