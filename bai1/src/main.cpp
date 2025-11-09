//Lập trình “Giao tiếp nút nhấn và cổng COM”

#include <Arduino.h>
const int buttonPin = 17;  // định nghĩa chân nút nhấn
const int ledPin =  16;    // định nghĩa chân led
// biến lưu trạng thái nút nhấn
int buttonState = 0;
 
void setup() {
  Serial.begin(115200);  
  // khởi tạo chân nút nhấn là input
  pinMode(buttonPin, INPUT);
  // khởi tạo chân nút nhấnn là output
  pinMode(ledPin, OUTPUT);
}
void loop() {
  // đọc giá trị nút nhấn 
  buttonState = digitalRead(buttonPin);
  //in lên màn hình serial
  Serial.println(buttonState);
  //ghi giá trị nút nhấn vào led
  digitalWrite(ledPin, buttonState);
}
