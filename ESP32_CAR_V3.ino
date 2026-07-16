
#include <WiFi.h>
#include <WebServer.h>
#include "BluetoothSerial.h"

BluetoothSerial BT;
WebServer server(80);

// ===== Motor pins =====
#define ENA 25
#define IN1 26
#define IN2 27
#define IN3 14
#define IN4 12
#define ENB 33

int speedMotor = 150;
char cmd='S';

const char* WIFI_SSID="YOUR_WIFI";
const char* WIFI_PASS="YOUR_PASSWORD";

const char webpage[] PROGMEM = R"HTML(
<!doctype html><html><body>
<h2>ESP32 CAR V3</h2>
<button onclick="fetch('/cmd?c=F')">Forward</button>
<button onclick="fetch('/cmd?c=B')">Back</button>
<button onclick="fetch('/cmd?c=L')">Left</button>
<button onclick="fetch('/cmd?c=R')">Right</button>
<button onclick="fetch('/cmd?c=S')">Stop</button>
</body></html>
)HTML";

void stopMotor(){
  digitalWrite(IN1,LOW); digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW); digitalWrite(IN4,LOW);
  analogWrite(ENA,0); analogWrite(ENB,0);
}
void forward(){
  digitalWrite(IN1,HIGH); digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH); digitalWrite(IN4,LOW);
  analogWrite(ENA,speedMotor); analogWrite(ENB,speedMotor);
}
void backward(){
  digitalWrite(IN1,LOW); digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW); digitalWrite(IN4,HIGH);
  analogWrite(ENA,speedMotor); analogWrite(ENB,speedMotor);
}
void left(){
  digitalWrite(IN1,LOW); digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH); digitalWrite(IN4,LOW);
  analogWrite(ENA,speedMotor); analogWrite(ENB,speedMotor);
}
void right(){
  digitalWrite(IN1,HIGH); digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW); digitalWrite(IN4,HIGH);
  analogWrite(ENA,speedMotor); analogWrite(ENB,speedMotor);
}

void handleCmd(){
  if(server.hasArg("c")){
    char c=server.arg("c")[0];
    switch(c){
      case 'F': forward(); break;
      case 'B': backward(); break;
      case 'L': left(); break;
      case 'R': right(); break;
      default: stopMotor();
    }
  }
  server.send(200,"text/plain","OK");
}

void setup(){
  Serial.begin(115200);
  pinMode(IN1,OUTPUT); pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT); pinMode(IN4,OUTPUT);
  pinMode(ENA,OUTPUT); pinMode(ENB,OUTPUT);
  stopMotor();

  BT.begin("ESP32_CAR_V3");

  WiFi.begin(WIFI_SSID,WIFI_PASS);
  while(WiFi.status()!=WL_CONNECTED){delay(500);}
  server.on("/",[](){server.send_P(200,"text/html",webpage);});
  server.on("/cmd",handleCmd);
  server.begin();

  Serial.println(WiFi.localIP());
}

void loop(){
  if(BT.available()){
    cmd=BT.read();
    switch(cmd){
      case 'F': forward(); break;
      case 'B': backward(); break;
      case 'L': left(); break;
      case 'R': right(); break;
      default: stopMotor();
    }
  }
  server.handleClient();
}
