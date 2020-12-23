#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"
//include the Arduino library for your real sensor here, e.g. <DHT.h>
#include <ESP8266WebServer.h>
  #include <OneWire.h>
  #include <DallasTemperature.h>


  // Data wire is plugged into port D2 on the ESP8266
  #define ONE_WIRE_BUS D2
  
  // Setup a oneWire instance to communicate with any OneWire devices
  OneWire oneWire(ONE_WIRE_BUS);
  
  // Pass our oneWire reference to Dallas Temperature.
  DallasTemperature sensors(&oneWire);
#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);


  float tempSensor1;
  
  uint8_t sensor1[8] = { 0x28, 0x50, 0x74, 0x8D, 0x3C, 0x19, 0x01, 0x63 };

void setup() {
  Serial.begin(115200);
  wifi_connect(); // in wifi_info.h
  my_homekit_setup();
 sensors.begin();
    sensors.requestTemperatures();
    tempSensor1 = sensors.getTempC(sensor1);
}

void loop() {
  my_homekit_loop();
  delay(10);
}

//==============================
// Homekit setup and loop
//==============================

// access your homekit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_current_temperature;

static uint32_t next_heap_millis = 0;
static uint32_t next_report_millis = 0;

void my_homekit_setup() {
  arduino_homekit_setup(&config);
}

void my_homekit_loop() {
  arduino_homekit_loop();
  const uint32_t t = millis();
  if (t > next_report_millis) {
    // report sensor values every 10 seconds
    next_report_millis = t + 10 * 1000;
    my_homekit_report();
  }
  if (t > next_heap_millis) {
    // show heap info every 5 seconds
    next_heap_millis = t + 5 * 1000;
    LOG_D("Free heap: %d, HomeKit clients: %d",
        ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

  }
}

void my_homekit_report() {
  sensors.begin();
    sensors.requestTemperatures();
    tempSensor1 = sensors.getTempC(sensor1);
  float temperature_value = tempSensor1; // FIXME, read your real sensor here.
  cha_current_temperature.value.float_value = temperature_value;
  LOG_D("Current temperature: %.1f", temperature_value);
  homekit_characteristic_notify(&cha_current_temperature, cha_current_temperature.value);
}
//
//int random_value(int min, int max) {
//  return min + random(max - min);
//}
