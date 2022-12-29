#include <Arduino.h>

#include "esp_bt_main.h"
#include "esp_gap_ble_api.h"
#include "esp_bt_device.h"

uint8_t data[20] = "abcdefghijklmnop";//modify as needed

void setup() {
  Serial.begin(115200);

  btStart();
  esp_bluedroid_init();
  esp_bluedroid_enable();

  Serial.println(esp_err_to_name(esp_ble_gap_config_adv_data_raw(data, 20)));

  esp_ble_adv_params_t params;
  params.adv_filter_policy = ADV_FILTER_ALLOW_SCAN_WLST_CON_WLST;
  params.adv_int_max = 0x800;
  params.adv_int_min = 0x800;
  params.adv_type = ADV_TYPE_NONCONN_IND;
  params.channel_map = ADV_CHNL_37;
  params.own_addr_type = BLE_ADDR_TYPE_PUBLIC;
  Serial.println(esp_err_to_name(esp_ble_gap_start_advertising(&params)));

  const uint8_t *addr = esp_bt_dev_get_address();
  for (int i = 0; i < 6; i++) {
    Serial.println(addr[i]);
  }
}

void loop() { }
