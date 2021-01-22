#include <homekit/homekit.h>
#include <homekit/characteristics.h>

void my_accessory_identify(homekit_value_t _value) {
  printf("accessory identify\n");
}

// Switch (HAP section 8.38)
// required: ON
// optional: NAME

// format: bool; HAP section 9.70; write the .setter function to get the switch-event sent from iOS Home APP.
homekit_characteristic_t cha_switch_on = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t cha_switch_on2 = HOMEKIT_CHARACTERISTIC_(ON, false);

// format: string; HAP section 9.62; max length 64
homekit_characteristic_t cha_name = HOMEKIT_CHARACTERISTIC_(NAME, "Switch");
homekit_characteristic_t cha_name_2 = HOMEKIT_CHARACTERISTIC_(NAME, "Switch 2");

homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_switch, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {

          HOMEKIT_CHARACTERISTIC(NAME, "Switch 1"),
          HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Danae's Arduino HomeKit"),
          HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "5432107"),
          HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266"),
          HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
          HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
          NULL

          
        }),
    HOMEKIT_SERVICE(SWITCH, .primary=true, .characteristics=(homekit_characteristic_t*[]){
                        HOMEKIT_CHARACTERISTIC(NAME, "Switch 1"),
      &cha_switch_on,
      NULL
    }),
        NULL
    }),
    HOMEKIT_ACCESSORY(.id=2, .category=homekit_accessory_category_switch, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {

          
          HOMEKIT_CHARACTERISTIC(NAME, "Switch 2"),
          HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Danae's Arduino HomeKit"),
          HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "5432108"),
          HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266"),
          HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
          HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
          NULL
        }),
    HOMEKIT_SERVICE(SWITCH, .primary=true, .characteristics=(homekit_characteristic_t*[]){
                        HOMEKIT_CHARACTERISTIC(NAME, "Switch 2"),
      &cha_switch_on2,
      NULL
    }),
        NULL
    }),
    NULL
};

homekit_server_config_t config = {
    .accessories = accessories,
    .password = "111-11-111"
};
