#include <homekit/homekit.h>
#include <homekit/characteristics.h>

void my_accessory_identify(homekit_value_t _value) {
printf("accessory identify\n");
}

// Switch (HAP section 8.38)
// required: ON
// optional: NAME

// format: bool; HAP section 9.70; write the .setter function to get the switch-event sent from iOS Home APP.
homekit_characteristic_t cha_switch_on1 = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t cha_switch_on2 = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t cha_switch_on3 = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t cha_switch_on4 = HOMEKIT_CHARACTERISTIC_(ON, false);

// format: string; HAP section 9.62; max length 64
homekit_characteristic_t cha_name_1 = HOMEKIT_CHARACTERISTIC_(NAME, "Switch01");
homekit_characteristic_t cha_name_2 = HOMEKIT_CHARACTERISTIC_(NAME, "Switch02");
homekit_characteristic_t cha_name_3 = HOMEKIT_CHARACTERISTIC_(NAME, "Switch03");
homekit_characteristic_t cha_name_4 = HOMEKIT_CHARACTERISTIC_(NAME, "Switch04");

homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_switch, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {

          HOMEKIT_CHARACTERISTIC(NAME, "Switch01"),
          HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Arduino HomeKit"),
          HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "0123457"),
          HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266/ESP32"),
          HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
          HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
          NULL

          
        }),
    HOMEKIT_SERVICE(SWITCH, .primary=true, .characteristics=(homekit_characteristic_t*[]){
                        HOMEKIT_CHARACTERISTIC(NAME, "Switch01"),
      &cha_switch_on1,
      NULL
    }),
        NULL
    }),
    HOMEKIT_ACCESSORY(.id=2, .category=homekit_accessory_category_switch, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {

          
          HOMEKIT_CHARACTERISTIC(NAME, "Switch02"),
          HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Arduino HomeKit"),
          HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "0123458"),
          HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266/ESP32"),
          HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
          HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
          NULL

          
             
        }),
    HOMEKIT_SERVICE(SWITCH, .primary=true, .characteristics=(homekit_characteristic_t*[]){
                        HOMEKIT_CHARACTERISTIC(NAME, "Switch02"),
      &cha_switch_on2,
      NULL
    }),
        NULL
    }),
    HOMEKIT_ACCESSORY(.id=3, .category=homekit_accessory_category_switch, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {

          
          HOMEKIT_CHARACTERISTIC(NAME, "Switch03"),
          HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Arduino HomeKit"),
          HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "0123459"),
          HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266/ESP32"),
          HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
          HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
          NULL

          
             
        }),
    HOMEKIT_SERVICE(SWITCH, .primary=true, .characteristics=(homekit_characteristic_t*[]){
                        HOMEKIT_CHARACTERISTIC(NAME, "Switch03"),
      &cha_switch_on3,
      NULL
    }),
        NULL
    }),
    HOMEKIT_ACCESSORY(.id=4, .category=homekit_accessory_category_switch, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {

          
          HOMEKIT_CHARACTERISTIC(NAME, "Switch04"),
          HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Arduino HomeKit"),
          HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "0123454"),
          HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266/ESP32"),
          HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
          HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
          NULL

          
             
        }),
    HOMEKIT_SERVICE(SWITCH, .primary=true, .characteristics=(homekit_characteristic_t*[]){
                        HOMEKIT_CHARACTERISTIC(NAME, "Switch04"),
      &cha_switch_on4,
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
