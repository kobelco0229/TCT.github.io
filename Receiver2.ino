//บอร์ด 2 - Receiver เท่านั้น:
// Soil Moisture Sensor (ความชื้นดิน)
#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_message {
  int moistureRaw;
  float moisturePercent;
} struct_message;

struct_message incomingData;

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  memcpy(&incomingData, data, sizeof(incomingData));
  Serial.println("=== ได้รับข้อมูล ===");
  Serial.print("Raw: ");
  Serial.println(incomingData.moistureRaw);
  Serial.print("ความชื้น: ");
  Serial.print(incomingData.moisturePercent);
  Serial.println("%");
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFi.mode(WIFI_STA);
  Serial.print("Board2 MAC: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Receiver ready!");
}

void loop() {

}
