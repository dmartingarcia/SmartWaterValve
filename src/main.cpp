#include <Arduino.h>

#define LED_PIN D4
#define MOTOR_PIN D1
#define VALVE_POS D2
#define CLOSED false // Switch both values if it doesn't match with the expected result
#define OPEN true

bool led_status = false;
double rotation_time;
boolean valve_state = CLOSED;

void toggle_led( ){
  led_status = !led_status;
  digitalWrite(LED_PIN, led_status);
}

boolean read_valve_state(){
  return digitalRead(VALVE_POS);
}

void check_pin(int pin){
  digitalWrite(pin, HIGH);
  delay(1000);
  digitalWrite(pin, LOW);
}

void check_led(){
  check_pin(LED_PIN);
}

void move_valve(boolean expected_status, int delay_time){
   while(read_valve_state() != expected_status){
    digitalWrite(MOTOR_PIN, true);
    delay(delay_time);
  }
  digitalWrite(MOTOR_PIN, false);
}

void get_time_between_rotations(){
  boolean current_state = digitalRead(VALVE_POS);
  //reset pos
  move_valve(!current_state, 10);

  double start_time = millis();
  digitalWrite(MOTOR_PIN, true);
  delay(500);

  move_valve(current_state, 10);
  rotation_time = millis() - start_time;
}

void set_valve_to(boolean state){
  while(read_valve_state() == state){
    digitalWrite(MOTOR_PIN, true);
  }
  delay(300);

  digitalWrite(MOTOR_PIN, false);
}

void perform_self_test(){
  Serial.println("SET ROTATION TIME");
  get_time_between_rotations();
  Serial.print("Rotation time: ");
  Serial.println(rotation_time);
}

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(VALVE_POS, INPUT_PULLUP);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(MOTOR_PIN, LOW);

  perform_self_test();
}

void loop() {
  set_valve_to(valve_state);
  // put your main code here, to run repeatedly:+
  delay(1000);
  toggle_led();
}
