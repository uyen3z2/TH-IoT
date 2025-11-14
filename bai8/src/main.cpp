#include <Arduino.h> 
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
 
// Replace with your network credentials
const char* ssid = "TP-Link_9430";
const char* password = "51563103"; 
bool led1State = 0;
const int led1Pin = 2; 
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
 
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Websocket Web Server Websocket</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
<title>ESP Web Server</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
</head>
<body>
  <div class="topnav">
    <h1>ESP WebSocket Server</h1>
  </div>
  <div class="content">
    <div class="card">
      <h2>IOT UNETI</h2>
      <p class="state1">LED1: <span id="state1">%STATE1%</span></p>
      <p><button id="button1" class="button1">BUTTON1</button></p>
    </div>
  </div>
<script>
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {
    let state1 = "---";
    let rcv_str = event.data;

    if (rcv_str.includes("1")){
      state1 = "ON";
    }else{
      if (rcv_str.includes("0")){
        state1 = "OFF";
      }
    }
    document.getElementById('state1').innerHTML = state1;
  }
  function onLoad(event) {
    initWebSocket();
    initButton();
  }
  function initButton() {
    document.getElementById('button1').addEventListener('click', toggle1);
  }
  function toggle1(){
    websocket.send('toggle1');
  }
</script>
</body>
</html>
)rawliteral";

void notifyClients() {
  ws.textAll(String(led1State));
}
 
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "toggle1") == 0) {
      led1State = !led1State;
      notifyClients();
    }
  }
}
 
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}
 
void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}
 
String processor(const String& var){
  Serial.println(var);
  if(var == "STATE1"){
    if (led1State){
      return "ON";
    }
    else{
      return "OFF";
    }
  }
}
 
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
 
  pinMode(led1Pin, OUTPUT);
  digitalWrite(led1Pin, LOW);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());
 
  initWebSocket();
 
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
 
  // Start server
  server.begin();
}
 
void loop() {
  ws.cleanupClients();
  digitalWrite(led1Pin, led1State);
}