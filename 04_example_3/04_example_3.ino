#define PIN_LED 13
unsigned int count, toggle;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(115200); //* Initialize port
  while (!Serial) {
    ; //*wait connect
  }
  Serial.println("Hello World!");
  count = toggle = 0;
  digitalWrite(PIN_LED, toggle); //* set LED low
}

void loop() {
  Serial.println(++count); //* serial monitor counter
  toggle = toggle_state(toggle); //* go to toggle-state
  digitalWrite(PIN_LED, toggle); //* Write LED value
  delay(1000); //* wait 1sec
}

int toggle_state(int toggle) { //* toggle function
    toggle = !toggle; //* toggle LED value by NOT GATE.
	return toggle;
}