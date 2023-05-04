#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "secrets.h"

#define LED_PIN D4
#define MOTOR_PIN D1
#define VALVE_POS D2
#define CLOSED false // Switch both values if it doesn't match with the expected result
#define OPEN true

ESP8266WebServer server(80);

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
    yield(); // avoiding Watchdog
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

String SendHTML(){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP8266 Smart Water Valve</h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";

  if(valve_state)
  {ptr +="<p>Valve State: ON</p><a class=\"button button-off\" href=\"/close_valve\">OFF</a>\n";}
  else
  {ptr +="<p>Valve State: OFF</p><a class=\"button button-on\" href=\"/open_valve\">ON</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

void handle_OnConnect(){
  server.send(200, "text/html", SendHTML());
}

void handle_valve_off() {
  valve_state = CLOSED;
  server.send(200, "text/html", SendHTML());
}

void handle_valve_on() {
  valve_state = OPEN;
  server.send(200, "text/html", SendHTML());
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(VALVE_POS, INPUT_PULLUP);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(MOTOR_PIN, LOW);

  perform_self_test();

Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.on("/open_valve", handle_valve_on);
  server.on("/close_valve", handle_valve_off);
  server.onNotFound(handle_NotFound);

  server.begin();
}

void loop() {
  set_valve_to(valve_state);
  // put your main code here, to run repeatedly:+
  toggle_led();
  server.handleClient();
}
