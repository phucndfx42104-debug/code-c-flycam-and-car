#include "BluetoothSerial.h"
BluetoothSerial BT;
char cmd;

const int Pin1 = 12;      // Chân tín hiệu ESC
const int Pin2 = 13;
const int Pin3 = 14;
const int Pin4 = 15;
const int freq = 50;        // Tần số 50Hz (chu kỳ 20ms)
const int resolution = 16;  // Độ phân giải 16-bit (0-65535)

int tocdo = 0;

void setup() 
{
  ledcAttach(Pin1, freq, resolution);
  ledcAttach(Pin2, freq, resolution);
  ledcAttach(Pin3, freq, resolution);
  ledcAttach(Pin4, freq, resolution);
  // Gửi tín hiệu 1000us (3277) để khởi động (ARM) ESC
  ledcWrite(Pin1, 3277);
  ledcWrite(Pin2, 3277);
  ledcWrite(Pin3, 3277);
  ledcWrite(Pin4, 3277);
  delay(2000); // Đợi 2 giây để ESC sẵn sàng
  //Phát Bluetooth");
  BT.begin("FLY");
  Serial.begin(115200);  

}

void loop()
{
  if(BT.available())
  {
    cmd = BT.read();
    Serial.println(cmd);
  }

  if(cmd == 'S')  
  {
    ledcWrite(Pin1, tocdo);
    ledcWrite(Pin2, tocdo);
    ledcWrite(Pin3, tocdo);
    ledcWrite(Pin4, tocdo);
  }
  if(cmd == 'F')  //Tiến  
  {
    ledcWrite(Pin1, tocdo - 50);
    ledcWrite(Pin2, tocdo - 50);
    ledcWrite(Pin3, tocdo + 50);
    ledcWrite(Pin4, tocdo + 50);
  }
  if(cmd == 'B')  //Lùi
  {
    ledcWrite(Pin1, tocdo + 50);
    ledcWrite(Pin2, tocdo + 50);
    ledcWrite(Pin3, tocdo - 50);
    ledcWrite(Pin4, tocdo - 50);
  }
  if(cmd == 'L')  //Nghiêng Trái
  {
    ledcWrite(Pin1, tocdo - 50);
    ledcWrite(Pin2, tocdo + 50);
    ledcWrite(Pin3, tocdo - 50);
    ledcWrite(Pin4, tocdo + 50);
  }
  if(cmd == 'R')  //Nghiêng Phải
  {
    ledcWrite(Pin1, tocdo + 50);
    ledcWrite(Pin2, tocdo - 50);
    ledcWrite(Pin3, tocdo + 50);
    ledcWrite(Pin4, tocdo - 50);
  }
  if(cmd == 'G')  //Xoay Trái
  {
    ledcWrite(Pin1, tocdo - 50);
    ledcWrite(Pin2, tocdo + 50);
    ledcWrite(Pin3, tocdo - 50);
    ledcWrite(Pin4, tocdo + 50);
  }
  if(cmd == 'I')  //Xoay Phải
  {
    ledcWrite(Pin1, tocdo - 50);
    ledcWrite(Pin2, tocdo + 50);
    ledcWrite(Pin3, tocdo - 50);
    ledcWrite(Pin4, tocdo + 50);
  }

  if(cmd == '0')  tocdo = 0;
  if(cmd == '1')  tocdo = 3277;
  if(cmd == '2')  tocdo = 3687; 
  if(cmd == '3')  tocdo = 4097;
  if(cmd == '4')  tocdo = 4507;
  if(cmd == '5')  tocdo = 4917;
  if(cmd == '6')  tocdo = 5327;
  if(cmd == '7')  tocdo = 5737;
  if(cmd == '8')  tocdo = 6147; 
  if(cmd == '9')  tocdo = 6350; 
  if(cmd == 'q')  tocdo = 6554;

}
/*
Mỗi loại ESC có thể có sai số nhỏ. Đôi khi, để đạt tốc độ tối đa thực sự ổn định,
cần thực hiện bước "ESC Calibration" (căn chỉnh hành trình ga) để ESC "học" được
chính xác dải xung (ví dụ: đâu là điểm 1000µs, đâu là điểm 2000µs
từ tay điều khiển hoặc vi điều khiển của bạn).
===================================================================================
        L(P1)        R(P2)      
            \        /        
              \____/
              |    |                  Sơ đồ chiều và chân tín hiệu
              |____|                  L: Quay ngược chiều kim đồng hồ
             /      \                 R: Quay thuận
            /        \ 
        R(P3)          L(P4)
====================================================================================
*/






