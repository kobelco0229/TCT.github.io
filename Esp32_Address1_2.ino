#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0x34, 0x86, 0x5D, 0xFC, 0x26, 0xB4}; //1
//uint8_t broadcastAddress[] = {0xD8, 0xBC, 0x38, 0xF9, 0x09, 0xBC};//2
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;

struct_message myData;

void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Failed");
}

void setup() {
  Serial.begin(115200);
  delay(1000); // เพิ่มจาก 100 เป็น 1000
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  
  Serial.print("ESP32 MAC: ");
  Serial.println(WiFi.macAddress());
  
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
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
  
  Serial.println("Setup complete!");
}

void loop() {
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());

  strcpy(myData.a, "THIS IS A CHAR");
  myData.b = random(1, 20);
  myData.c = 1.2;
  myData.d = false;
  
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
  
  if (result == ESP_OK) {
    Serial.println("Sent successfully");
  } else {
    Serial.print("Error sending: ");
    Serial.println(result);
  }
  
  delay(2000);
}
