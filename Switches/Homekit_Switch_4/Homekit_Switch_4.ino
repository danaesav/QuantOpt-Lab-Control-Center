#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

void setup() {
Serial.begin(115200);
wifi_connect(); // in wifi_info.h
homekit_storage_reset(); // to remove the previous HomeKit pairing storage when you first run this new HomeKit example
my_homekit_setup();
}

void loop() {
my_homekit_loop();
delay(10);
}

//==============================
// HomeKit setup and loop
//==============================

// access your HomeKit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_switch_on1;
extern "C" homekit_characteristic_t cha_switch_on2;
extern "C" homekit_characteristic_t cha_switch_on3;
extern "C" homekit_characteristic_t cha_switch_on4;
static uint32_t next_heap_millis = 0;

#define PIN_SWITCH1 D1 //6 1
#define PIN_SWITCH2 D2 //5 2
#define PIN_SWITCH3 D6 //1 6
#define PIN_SWITCH4 D7 //2 7

//Called when the switch value is changed by iOS Home APP
void cha_switch_on_setter1(const homekit_value_t value) {
bool on = value.bool_value;
cha_switch_on1.value.bool_value = on; //sync the value
LOG_D("Switch: %s", on ? "ON" : "OFF");
digitalWrite(PIN_SWITCH1, on ? LOW : HIGH);
}
void cha_switch_on_setter2(const homekit_value_t value) {
bool on = value.bool_value;
cha_switch_on2.value.bool_value = on; //sync the value
LOG_D("Switch: %s", on ? "ON" : "OFF");
digitalWrite(PIN_SWITCH2, on ? LOW: HIGH);
}
void cha_switch_on_setter3(const homekit_value_t value) {
bool on = value.bool_value;
cha_switch_on3.value.bool_value = on; //sync the value
LOG_D("Switch: %s", on ? "ON" : "OFF");
digitalWrite(PIN_SWITCH3, on ? LOW : HIGH);
}
void cha_switch_on_setter4(const homekit_value_t value) {
bool on = value.bool_value;
cha_switch_on4.value.bool_value = on; //sync the value
LOG_D("Switch: %s", on ? "ON" : "OFF");
digitalWrite(PIN_SWITCH4, on ? LOW : HIGH);
}


void my_homekit_setup() {
  pinMode(PIN_SWITCH1, OUTPUT);
  digitalWrite(PIN_SWITCH1, HIGH);
  pinMode(PIN_SWITCH2, OUTPUT);
  digitalWrite(PIN_SWITCH2, HIGH);
  pinMode(PIN_SWITCH3, OUTPUT);
  digitalWrite(PIN_SWITCH3, HIGH);
  pinMode(PIN_SWITCH4, OUTPUT);
  digitalWrite(PIN_SWITCH4, HIGH);
  
  cha_switch_on1.setter = cha_switch_on_setter1;
  cha_switch_on2.setter = cha_switch_on_setter2;
  cha_switch_on3.setter = cha_switch_on_setter3;
  cha_switch_on4.setter = cha_switch_on_setter4;
  arduino_homekit_setup(&config);

}

void my_homekit_loop() {
  arduino_homekit_loop();
  const uint32_t t = millis();
  if (t > next_heap_millis) {
    // show heap info every 5 seconds
    next_heap_millis = t + 5 * 1000;
    LOG_D("Free heap: %d, HomeKit clients: %d",
    ESP.getFreeHeap(), arduino_homekit_connected_clients_count());
  }
}
