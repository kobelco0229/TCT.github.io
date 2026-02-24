//บอร์ด 1 - Sender เท่านั้น:
// Soil Moisture Sensor (ความชื้นดิน)
//uint8_t broadcastAddress[] = {0xD8, 0xBC, 0x38, 0xF9, 0x09, 0xBC}; address2
#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0xD8, 0xBC, 0x38, 0xF9, 0x09, 0xBC};

#define MOISTURE_PIN 34  // ขา Analog ที่ต่อกับ sensor

typedef struct struct_message {
  int moistureRaw;      // ค่า raw จาก sensor
  float moisturePercent; // ค่าเปอร์เซ็นต์ความชื้น
} struct_message;

struct_message myData;

void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Failed");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFi.mode(WIFI_STA);
  Serial.print("Board1 MAC: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  peerInfo.ifidx = WIFI_IF_STA;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  Serial.println("Sender ready!");
}

void loop() {
  int raw = analogRead(MOISTURE_PIN);
  
  // ปรับค่า min/max ตาม sensor จริงของคุณ
  // แห้งสุด = 4095, เปียกสุด = 1500 (ลองวัดแล้วปรับ)
  float percent = map(raw, 4095, 1500, 0, 100);
  percent = constrain(percent, 0, 100);

  myData.moistureRaw = raw;
  myData.moisturePercent = percent;

  Serial.print("Raw: ");
  Serial.print(raw);
  Serial.print(" | Moisture: ");
  Serial.print(percent);
  Serial.println("%");

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }

  delay(2000);
}
