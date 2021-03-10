#define IN  4
#define OUT 5

#define DURATION_LOW_PULSE_MIN (50u)
#define DURATION_LOW_PULSE_MAX (1000u)

#define DURATION_DISABLE  (310000u)
#define DISABLE           (HIGH)
#define ENABLE            (LOW)

// -----------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  pinMode(IN, INPUT);
  pinMode(OUT, OUTPUT);
  
  digitalWrite(OUT, ENABLE);
  delay(2000);
  Serial.println("Start");
  delay(5000);
}

// -----------------------------------------------------------------------------

void loop() {
  static bool scanning_low = false;
  static uint32_t ms_in_low_start = 0;
  static uint32_t ms_disable_start = 0;
  static bool force_turn_off = false;
  static int input_so_far = digitalRead(IN);
  static int counter_print = 0;

  int input_new = digitalRead(IN);
  
  // detect pulse that computer performs, trying to turn off power from the interior equipment
  bool event_turn_off_try = input_new == HIGH;
  bool input_event = false;
  bool event_driver_return = false;
  
  if (input_so_far != input_new) {
      Serial.print("Input changed from: ");
      Serial.print(input_so_far);
      Serial.print(" to: ");
      Serial.println(input_new);
      input_so_far = input_new;
      input_event = true;
      // state opposite to the one expressed as `event_turn_off_try`
      if (input_new == !event_turn_off_try) {
          event_driver_return = true;
      }
  }
  
  if (counter_print++ == 200000) {
    Serial.println(input_new);
    counter_print = 0;
  }

  if (!force_turn_off && !scanning_low && event_turn_off_try) {
    startCounting(ms_in_low_start, scanning_low);

  } else if (!force_turn_off && scanning_low && event_turn_off_try) {
    countPulseDuration(ms_in_low_start);

  } else if (!force_turn_off && scanning_low && !event_turn_off_try) {
    pulseEnded(scanning_low, ms_in_low_start, force_turn_off, ms_disable_start);

  } else if (force_turn_off) {
    maintainLow(force_turn_off, ms_disable_start, event_driver_return);

  } else {
    idle();

  }
}

// -----------------------------------------------------------------------------

void startCounting(uint32_t& ms_in_low_start, bool& scanning_low) {
  scanning_low = true;
  ms_in_low_start = millis();
  Serial.println("[startCounting] begin");
}

// -----------------------------------------------------------------------------

void countPulseDuration(uint32_t& ms_in_low_start) {
  // ...
}

// -----------------------------------------------------------------------------

void pulseEnded(bool& scanning_low, uint32_t ms_in_low_start, bool& force_turn_off, uint32_t& ms_disable_start) {
  scanning_low = false;
  uint32_t duration_low = millis() - ms_in_low_start;
  Serial.print("[pulseEnded] duration_low: ");
  Serial.print(duration_low);
  if (duration_low >= DURATION_LOW_PULSE_MIN && duration_low <= DURATION_LOW_PULSE_MAX) {
    force_turn_off = true;
    digitalWrite(OUT, DISABLE);
    ms_disable_start = millis();
    Serial.print("\tforce_turn_off ENABLED");
  } else {
    Serial.print("\tno further action");
  }
  Serial.println();
  // reset duration timestamp, just in case
  ms_in_low_start = millis();
}

// -----------------------------------------------------------------------------

void maintainLow(bool& force_turn_off, uint32_t& ms_disable_start, bool event_driver_return) {
  static uint32_t last_ms_print = 0;
  
  if (force_turn_off == false) {
    Serial.println("[maintainLow] force_turn_off == false");
    return;
  }

  if (millis() - ms_disable_start > DURATION_DISABLE) {
    Serial.print("[maintainLow] time: ");
    Serial.print(millis() - ms_disable_start);
    Serial.println(" elapsed!");

    digitalWrite(OUT, ENABLE);
    force_turn_off = false;
    return;
  }

  uint32_t ms = millis();
  
  if ((last_ms_print != ms) && ((ms - ms_disable_start) % 50 == 0)) {
    Serial.print("[maintainLow] elapsed: ");
    Serial.println(millis() - ms_disable_start);
    last_ms_print = ms;
  }

  if (event_driver_return) {
    Serial.print("[maintainLow] event_driver_return, after: ");
    Serial.println(millis() - ms_disable_start);
    digitalWrite(OUT, ENABLE);
    force_turn_off = false;
    return;
  }
  
}

// -----------------------------------------------------------------------------

void idle() {
  digitalWrite(OUT, ENABLE);
}

// -----------------------------------------------------------------------------
