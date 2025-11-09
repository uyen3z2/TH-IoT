// Bật tắt LED sử dụng Ngắt Timer
#include <Arduino.h>

const int ledPin =  2;      
int ledState = LOW;   
hw_timer_t* timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// hàm xử lý ngắt
void IRAM_ATTR onTimer() {    
  portENTER_CRITICAL_ISR(&timerMux);
  ledState = !ledState;
  digitalWrite(ledPin, ledState);  
  portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() { 
  Serial.begin(115200); 
  pinMode(ledPin, OUTPUT);  

  // khởi tạo timer với prescaler 80
  timer = timerBegin(0, 80, true); 
  // gán ISR cho timer
  timerAttachInterrupt(timer, &onTimer, true); 
  // set chu kỳ 1s
  timerAlarmWrite(timer, 1000000, true); 
  timerAlarmEnable(timer);
}

void loop() { 
  // do nothing
}
