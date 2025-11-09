//Lập trình “ESP32 Analog Input đọc giá trị biến trở và in ra màn hình Serial”

#include <Arduino.h> 
// định nghĩa chân kết nối ADC
const int potPin = 34; 
// giá trị đo được
int potValue = 0;
 
void setup() {
  Serial.begin(9600);
  delay(1000);
}
 
void loop() {
  // Đọc giá trị analog
  potValue = analogRead(potPin);
  Serial.println(potValue);
  delay(500);
}