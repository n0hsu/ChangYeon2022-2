#include <Servo.h>

// Arduino pin assignment

#define PIN_IRsensor 3 // IR sensor
#define PIN_LED   9   // LED active-low
#define PIN_SERVO 10 // servo motor

#define _DUTY_MIN 553  // servo full clock-wise position (0 degree)
#define _DUTY_NEU 1476 // servo neutral position (90 degree)
#define _DUTY_MAX 2399 // servo full counter-clockwise position (180 degree)

#define _DIST_MIN 100.0   // minimum distance to be measured (unit: mm)
#define _DIST_MAX 250.0   // maximum distance to be measured (unit: mm)

#define _EMA_ALPHA 0.4    // EMA weight of new sample (range: 0 to 1)

#define LOOP_INTERVAL 45   // Loop Interval (unit: msec)

Servo myservo;
// global variables
float  dist_ema, dist_prev = _DIST_MAX; // unit: mm
unsigned long last_loop_time;   // unit: msec
float _DUTY_BY_RANGE; //* 0 to 100

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);

  dist_prev = _DIST_MIN; // raw distance output from USS (unit: mm)
  
  Serial.begin(1000000);
}

void loop()
{
  unsigned long time_curr = millis();
  float  dist_raw;
  int IR_value, duty;

  // wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;

  IR_value = analogRead(PIN_IRsensor);
  dist_raw = (6762.0/(IR_value-9)-4.0)*10.0 - 60.0;  // Convert IR sensor value into distance

  // we need distance range filter here !!!
  if (dist_raw < _DIST_MIN) {
    dist_raw = dist_prev;           // cut lower than minimum
    digitalWrite(PIN_LED, 1);       // LED OFF
  } else if (dist_raw > _DIST_MAX) {
    dist_raw = dist_prev;           // Cut higher than maximum
    digitalWrite(PIN_LED, 1);       // LED OFF
  } else {    // In desired Range
    digitalWrite(PIN_LED, 0);       // LED ON      
 dist_prev = dist_raw;
  }

  dist_ema = (_EMA_ALPHA)*dist_raw + (1-_EMA_ALPHA)*dist_ema;  // EMA filter 

  _DUTY_BY_RANGE = (_DUTY_MIN+(_DUTY_MAX-_DUTY_MIN))*((dist_ema-_DIST_MIN)/(_DIST_MAX - _DIST_MIN));
  
  if (dist_ema < _DIST_MIN) {
    myservo.writeMicroseconds(_DUTY_MIN);
  } else if (dist_ema > _DIST_MAX) {
    myservo.writeMicroseconds(_DUTY_MAX);
  } else {   
    myservo.writeMicroseconds(_DUTY_BY_RANGE);
  }
  
  Serial.print("Min:");    Serial.print(_DIST_MIN);
  Serial.print("IR: "); Serial.print(IR_value);
  Serial.print(",dist:");  Serial.print(dist_raw);
  Serial.print(",ema:");  Serial.print(dist_ema);
  Serial.print(",Servo:"); Serial.print(_DUTY_BY_RANGE);  
  Serial.print(",Max:");   Serial.print(_DIST_MAX);
  Serial.println("");
}
