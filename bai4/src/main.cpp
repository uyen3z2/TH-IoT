#include <WiFi.h>  

const char* ssid = "TP-Link_9430";
const char* password = "51563103";

// Thời gian timeout kết nối WiFi (ms)
const unsigned long WIFI_TIMEOUT = 15000;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.print("Đang kết nối tới WiFi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  unsigned long startTime = millis();

  // Chờ kết nối với timeout
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    if (millis() - startTime > WIFI_TIMEOUT) {
      Serial.println("\n❌ Không kết nối được WiFi");
      break;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ Kết nối WiFi thành công!");
    Serial.print("Địa chỉ IP của ESP32: ");
    Serial.println(WiFi.localIP());
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());
  }
}

void loop() {
  // Nếu mất kết nối WiFi, tự reconnect
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("⚠️ WiFi bị mất, đang thử kết nối lại...");
    WiFi.disconnect();
    WiFi.begin(ssid, password);

    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      if (millis() - startTime > WIFI_TIMEOUT) {
        Serial.println("\n❌ Không reconnect được WiFi");
        break;
      }
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\n✅ Kết nối WiFi lại thành công!");
      Serial.print("Địa chỉ IP: ");
      Serial.println(WiFi.localIP());
    }
  }

  // Hiển thị IP hiện tại mỗi 5 giây
  Serial.print("IP hiện tại: ");
  Serial.println(WiFi.localIP());
  delay(5000);
}
