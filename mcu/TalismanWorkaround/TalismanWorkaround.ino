#define IN  4
#define OUT 5

#define DURATION_LOW_PULSE_MIN (50u)
#define DURATION_LOW_PULSE_MAX (1000u)

#define DURATION_DISABLE  (5000u)
#define DISABLE           (HIGH)
#define ENABLE            (LOW)

// -----------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  pinMode(IN, INPUT);
  pinMode(OUT, OUTPUT);
  
  Serial.println("Start");
}

// -----------------------------------------------------------------------------

void loop() {
//  while (1) {
//      digitalWrite(OUT, HIGH);
//      delay(3000);
//      digitalWrite(OUT, LOW);
//      delay(3000);
//  }
  static bool scanning_low = false;
  static uint32_t ms_in_low_start = 0;
  static uint32_t ms_disable_start = 0;
  static bool force_turn_off = false;

  static int counter_print = 0;

  bool input_low = digitalRead(IN) == HIGH;

  if (counter_print++ == 50) {
      Serial.println(digitalRead(IN));
      counter_print = 0;
  }
  
  if (!force_turn_off && !scanning_low && input_low) {
      startCounting(ms_in_low_start, scanning_low);
      
  } else if (!force_turn_off && scanning_low && input_low) {
      countPulseDuration(ms_in_low_start);
      
  } else if (!force_turn_off && scanning_low && !input_low) {
      pulseEnded(scanning_low, ms_in_low_start, force_turn_off, ms_disable_start);
      
  } else if (force_turn_off) {
      maintainLow(force_turn_off, ms_disable_start);
      
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

void maintainLow(bool& force_turn_off, uint32_t& ms_disable_start) {
    if (force_turn_off == false) {
        Serial.println("[maintainLow] force_turn_off == false");
        return;
    }
    if (millis() - ms_disable_start > DURATION_DISABLE) {
        Serial.print("[maintainLow] time: ");
        Serial.print(DURATION_DISABLE);
        Serial.println(" elapsed!");
        
        digitalWrite(OUT, ENABLE);
        force_turn_off = false;
    }
}

// -----------------------------------------------------------------------------

void idle() {
    digitalWrite(OUT, ENABLE);
}

// -----------------------------------------------------------------------------
