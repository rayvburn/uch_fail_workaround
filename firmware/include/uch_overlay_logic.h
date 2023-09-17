#pragma once

#include <stdint.h>

class UchOverlayLogic {
public:
	static constexpr unsigned long DURATION_INDOOR_PWR_PULSE_MIN = 200;
	static constexpr unsigned long DURATION_INDOOR_PWR_PULSE_MAX = 500;

	/// Factory signal typically appears after (in ms)
	static constexpr unsigned long TIME_TO_SHUT_OFF = 59000;
	/// How much longer we'll wait for shut off signals to appear compared to the ones given by constants (in ms)
	static constexpr unsigned long TIME_TO_SHUT_OFF_TOLERANCE = 2000;
	/// In ms, the factory signal lasts 60-175 ms
	static constexpr unsigned long DURATION_SHUT_OFF = 301000;

	/// In milliseconds
	static constexpr unsigned long PERIODIC_PRINT_TIME = 2000;

	/// Set @ref inverse_input_logic in the ctor so it matches that pattern of signal detection
	static constexpr int INPUT_INDOOR_POWER_ENABLE = 1; // a.k.a. HIGH

	/// States of the internal finite state machine
	enum State {
		IDLE = 0,
		COUNTING_PULSE_DURATION,
		WAITING_SHUT_OFF,
		SHUT_OFF
	};

	/**
	 * @param indoor_power_factory_input_pin input signal that drives the factory relay.
	 * We are looking for a short pulse of @ref event_turn_off_try level from main computer. Its length is between:
	 * DURATION_INDOOR_PWR_PULSE_MIN [ms] and DURATION_INDOOR_PWR_PULSE_MAX [ms].
	 * If such appeared, we force the relay to be turned off. After approx. 5 minutes of some circuit being disabled,
	 * the main computer checks that circuit and if disabled - decides that it's safe to turn off power in the cabin.
	 * @param indoor_power_ctrl_output_pin output signal that drives the factory relay. We either pass the factory
	 * state or overwrite it (after pulse detection).
	 * @param inverse_input_logic handy when input is connected via, e.g., an optocoupler
	 * @param inverse_output_logic handy when the output controls 12V input of the relay via N-MOSFET and a PNP
	 */
	UchOverlayLogic(
		uint8_t indoor_power_factory_input_pin,
		uint8_t indoor_power_ctrl_output_pin,
		bool inverse_input_logic,
		bool inverse_output_logic
	);

	/// Main method that should be called in each loop execution
	void update();

	/// GPIO testing - prints updates periodically
	void test();

	/// Resets the counting
	void reset();

	/// Whether is in non-idle state
	inline bool isActive() const {
		return state_ != IDLE;
	}

protected:
	// State handlers
	void stateHandlerIdle();
	void stateHandlerCountingPulseDuration();
	void stateHandlerWaitingShutOff();
	void stateHandlerShutOff();

	// Handles the logic that controls switching between the states
	void selectState(unsigned long timestamp, bool input_changed);

	void changeState(State new_state);
	/// Returns true if the state was changed
	bool enterState();

	// Pin manipulation
	int readInputFactoryIndoorPower() const;
	int readOutputFactoryIndoorPower() const;
	void cutOffIndoorPower();
	void applyFactoryIndoorPower();

	// MCU pin configuration
	uint8_t indoor_power_factory_input_pin_;
	bool inverse_input_logic_;

	uint8_t indoor_power_ctrl_output_pin_;
	int output_indoor_pwr_factory_;
	int output_indoor_pwr_shut_off_;

	// Current state of the internal state machine
	State state_;
	bool state_changed_;

	// diagnostics
	// timestamp of the last print of the input state
	unsigned long timestamp_last_print_;
	unsigned long timestamp_last_test_;
	unsigned long timestamp_last_print_in_state_;

	// state conditions
	int input_so_far_;
	int input_new_;
	unsigned long timestamp_last_input_change_;
	unsigned long timestamp_shutoff_start_;
	/// Related to the pulse at state given by @ref INPUT_INDOOR_POWER_ENABLE
	unsigned long duration_last_pulse_;
};
