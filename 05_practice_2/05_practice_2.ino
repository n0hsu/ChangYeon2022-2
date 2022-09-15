#define PIN_LED 7 //* 7th pin Control
unsigned int counter, toggle;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(115200); //* Initialize port
  while (!Serial) {
    ; //*wait connect
  }
  counter = 0;
  toggle = 0; //* Value - LED ON(7pin-GND)
  digitalWrite(PIN_LED, toggle); //* Write LED ON(7pin-GND)
}

void loop() {
  delay(1000); //* wait 1sec with LED ON
  while(counter < 10){
  	toggle = toggle_state(toggle); //* go to PWM 10 time
  	digitalWrite(PIN_LED, toggle); //* Write LED value
    delay(100);
  	++counter; // 10 time in 1-2sec
  }
  toggle = toggle_state(toggle); //* LED turn off after 2sec
  digitalWrite(PIN_LED, toggle); //* Write LED value(7pin High)
  while(1) {} // infinite loop ending
}

int toggle_state(int toggle) { //* toggle function
    toggle = !toggle; //* toggle LED value by NOT GATE.
	return toggle;
}

