#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include "index.h"

#define CMD_STOP 0
#define CMD_FORWARD 1
#define CMD_BACKWARD 2
#define CMD_LEFT 4
#define CMD_RIGHT 8

#define ENA_PIN 14  // The ESP32 pin GPIO14 connected to the ENA pin L298N
#define IN1_PIN 27  // The ESP32 pin GPIO27 connected to the IN1 pin L298N
#define IN2_PIN 26  // The ESP32 pin GPIO26 connected to the IN2 pin L298N
#define IN3_PIN 25  // The ESP32 pin GPIO25 connected to the IN3 pin L298N
#define IN4_PIN 33  // The ESP32 pin GPIO33 connected to the IN4 pin L298N
#define ENB_PIN 32  // The ESP32 pin GPIO32 connected to the ENB pin L298N

const char* ssid = "kity_wifi_24";     // CHANGE IT
const char* password = "kity_secret_a1b2c3";  // CHANGE IT

AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);  // WebSocket server on port 81

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3]);
      }
      break;
    case WStype_TEXT:
      //Serial.printf("[%u] Received text: %s\n", num, payload);
      String angle = String((char*)payload);
      int command = angle.toInt();
      Serial.print("command: ");
      Serial.println(command);

      switch (command) {
        case CMD_STOP:
          Serial.println("Stop");
          CAR_stop();
          break;
        case CMD_FORWARD:
          Serial.println("Move Forward");
          CAR_moveForward();
          break;
        case CMD_BACKWARD:
          Serial.println("Move Backward");
          CAR_moveBackward();
          break;
        case CMD_LEFT:
          Serial.println("Turn Left");
          CAR_turnLeft();
          break;
        case CMD_RIGHT:
          Serial.println("Turn Right");
          CAR_turnRight();
          break;
        default:
          Serial.println("Unknown command");
      }

      break;
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(ENA_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);
  pinMode(ENB_PIN, OUTPUT);

  digitalWrite(ENA_PIN, HIGH);  // set full speed
  digitalWrite(ENB_PIN, HIGH);  // set full speed

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize WebSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  // Serve a basic HTML page with JavaScript to create the WebSocket connection
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("Web Server: received a web page request");
    String html = HTML_CONTENT;  // Use the HTML content from the servo_html.h file
    request->send(200, "text/html", html);
  });

  server.begin();
  Serial.print("ESP32 Web Server's IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  webSocket.loop();
  // TO DO: Your code here
}

void CAR_moveForward() {
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
}

void CAR_moveBackward() {
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, HIGH);
}

void CAR_turnLeft() {
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
}

void CAR_turnRight() {
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
}

void CAR_stop() {
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
}


// Import required libraries
// #include <WiFi.h>
// #include <AsyncTCP.h>
// #include <ESPAsyncWebServer.h>

// // Replace with your network credentials
// const char* ssid = "kity_wifi_24";
// const char* password = "kity_secret_a1b2c3";

// bool ledState = 0;
// const int ledPin = 33;

// // Create AsyncWebServer object on port 80
// AsyncWebServer server(80);
// AsyncWebSocket ws("/ws");

//   const char index_html[] PROGMEM = R"rawliteral(
//   <!DOCTYPE html>
//   <html>
//     <head>
//       <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
//       <title>ESP32 WebSocket Server</title>
//       <style>
//       html{font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
//       body{margin-top: 50px;}
//       h1{color: #444444;margin: 50px auto;}
//       p{font-size: 19px;color: #888;}
//       #state{font-weight: bold;color: #444;}
//       .switch{margin:25px auto;width:80px}
//       .toggle{display:none}
//       .toggle+label{display:block;position:relative;cursor:pointer;outline:0;user-select:none;padding:2px;width:80px;height:40px;background-color:#ddd;border-radius:40px}
//       .toggle+label:before,.toggle+label:after{display:block;position:absolute;top:1px;left:1px;bottom:1px;content:""}
//       .toggle+label:before{right:1px;background-color:#f1f1f1;border-radius:40px;transition:background .4s}
//       .toggle+label:after{width:40px;background-color:#fff;border-radius:20px;box-shadow:0 2px 5px rgba(0,0,0,.3);transition:margin .4s}
//       .toggle:checked+label:before{background-color:#4285f4}
//       .toggle:checked+label:after{margin-left:42px}
//       </style>
//     </head>
//     <body>
//       <h1>ESP32 WebSocket Server</h1>
//       <div class="switch">
//         <input id="toggle-btn" class="toggle" type="checkbox" %CHECK%>
//         <label for="toggle-btn"></label>
//       </div>
//       <p>On-board LED: <span id="state">%STATE%</span></p>

//       <script>
//       window.addEventListener('load', function() {
//       var websocket = new WebSocket(`ws://${window.location.hostname}/ws`);
//       websocket.onopen = function(event) {
//         console.log('Connection established');
//       }
//       websocket.onclose = function(event) {
//         console.log('Connection died');
//       }
//       websocket.onerror = function(error) {
//         console.log('error');
//       };
//       websocket.onmessage = function(event) {
//         if (event.data == "1") {
//         document.getElementById('state').innerHTML = "ON";
//         document.getElementById('toggle-btn').checked = true;
//         }
//         else {
//         document.getElementById('state').innerHTML = "OFF";
//         document.getElementById('toggle-btn').checked = false;
//         }
//       };
      
//       document.getElementById('toggle-btn').addEventListener('change', function() { websocket.send('toggle'); });
//       });
//     </script>
//     </body>
//   </html>
//   )rawliteral";

// void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
//   AwsFrameInfo *info = (AwsFrameInfo*)arg;
//   if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
//     data[len] = 0;
//     if (strcmp((char*)data, "toggle") == 0) {
//       ledState = !ledState;
//       ws.textAll(String(ledState));
//     }
//   }
// }

// void eventHandler(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
//   switch (type) {
//     case WS_EVT_CONNECT:
//       Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
//       break;
//     case WS_EVT_DISCONNECT:
//       Serial.printf("WebSocket client #%u disconnected\n", client->id());
//       break;
//     case WS_EVT_DATA:
//       handleWebSocketMessage(arg, data, len);
//       digitalWrite(ledPin, ledState);
//       break;
//     case WS_EVT_PONG:
//     case WS_EVT_ERROR:
//       break;
//   }
// }

// String processor(const String& var){
//   if(var == "STATE"){
//       return ledState ? "ON" : "OFF";
//   }
//   if(var == "CHECK"){
//     return ledState ? "checked" : "";
//   }
//   return String();
// }

// void setup(){
//   // Serial port for debugging purposes
//   Serial.begin(115200);

//   pinMode(ledPin, OUTPUT);
//   digitalWrite(ledPin, LOW);

//   Serial.print("Connecting to ");
//   Serial.println(ssid);
  
//   // Connect to Wi-Fi
//   WiFi.begin(ssid, password);
  
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(1000);
//     Serial.print(".");
//   }
  
//   Serial.println("");
//   Serial.println("Connected..!");
//   Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

//   ws.onEvent(eventHandler);
//   server.addHandler(&ws);

//   // Route for root / web page
//   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
//     request->send_P(200, "text/html", index_html, processor);
//   });

//   // Start server
//   server.begin();
// }

// void loop() {
//   ws.cleanupClients();
// }


// /*********
//   Rui Santos
//   Complete project details at https://RandomNerdTutorials.com/esp32-async-web-server-espasyncwebserver-library/
//   The above copyright notice and this permission notice shall be included in all
//   copies or substantial portions of the Software.
// *********/

// // Import required libraries
// #include <WiFi.h>
// #include <AsyncTCP.h>
// #include <ESPAsyncWebServer.h>

// // Replace with your network credentials
// const char* ssid = "kity";
// const char* password = "kity_secret";

// const char* PARAM_INPUT_1 = "output";
// const char* PARAM_INPUT_2 = "state";

// // Create AsyncWebServer object on port 80
// AsyncWebServer server(80);

// const char index_html[] PROGMEM = R"rawliteral(
// <!DOCTYPE HTML><html>
// <head>
//   <title>ESP Web Server</title>
//   <meta name="viewport" content="width=device-width, initial-scale=1">
//   <link rel="icon" href="data:,">
//   <style>
//     html {font-family: Arial; display: inline-block; text-align: center;}
//     h2 {font-size: 3.0rem;}
//     p {font-size: 3.0rem;}
//     body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
//     .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
//     .switch input {display: none}
//     .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px}
//     .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px}
//     input:checked+.slider {background-color: #b30000}
//     input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
//   </style>
// </head>
// <body>
//   <h2>ESP Web Server</h2>
//   %BUTTONPLACEHOLDER%
// <script>function toggleCheckbox(element) {
//   var xhr = new XMLHttpRequest();
//   if(element.checked){ xhr.open("GET", "/update?output="+element.id+"&state=1", true); }
//   else { xhr.open("GET", "/update?output="+element.id+"&state=0", true); }
//   xhr.send();
// }
// </script>
// </body>
// </html>
// )rawliteral";

// // Replaces placeholder with button section in your web page
// String processor(const String& var){
//   //Serial.println(var);
//   if(var == "BUTTONPLACEHOLDER"){
//     String buttons = "";
//     buttons += "<h4>Output - GPIO 2</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"2\" " + outputState(2) + "><span class=\"slider\"></span></label>";
//     buttons += "<h4>Output - GPIO 4</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"4\" " + outputState(4) + "><span class=\"slider\"></span></label>";
//     buttons += "<h4>Output - GPIO 33</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"33\" " + outputState(33) + "><span class=\"slider\"></span></label>";
//     return buttons;
//   }
//   return String();
// }

// String outputState(int output){
//   if(digitalRead(output)){
//     return "checked";
//   }
//   else {
//     return "";
//   }
// }

// void setup(){
//   // Serial port for debugging purposes
//   Serial.begin(115200);

//   pinMode(2, OUTPUT);
//   digitalWrite(2, LOW);
//   pinMode(4, OUTPUT);
//   digitalWrite(4, LOW);
//   pinMode(33, OUTPUT);
//   digitalWrite(33, LOW);
  
//   // Connect to Wi-Fi
//   WiFi.softAP(ssid, password);  

//   IPAddress IP = WiFi.softAPIP();
//   Serial.print("\nAP IP address: ");
//   Serial.println(IP);

//   // Route for root / web page
//   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
//     request->send_P(200, "text/html", index_html, processor);
//   });

//   // Send a GET request to <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
//   server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
//     String inputMessage1;
//     String inputMessage2;
//     // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
//     if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
//       inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
//       inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
//       digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
//     }
//     else {
//       inputMessage1 = "No message sent";
//       inputMessage2 = "No message sent";
//     }
//     Serial.print("GPIO: ");
//     Serial.print(inputMessage1);
//     Serial.print(" - Set to: ");
//     Serial.println(inputMessage2);
//     request->send(200, "text/plain", "OK");
//   });

//   // Start server
//   server.begin();
// }

// void loop() {

// }