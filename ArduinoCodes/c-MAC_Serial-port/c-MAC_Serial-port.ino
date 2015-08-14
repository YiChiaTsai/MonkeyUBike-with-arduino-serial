#include <SoftwareSerial.h>  // 使用arduino內建(軟體序列埠)程式庫
SoftwareSerial BTSerial(10, 11);    // 設定軟體序列埠(接收腳RX,傳送腳TX)

boolean queryBaud = true;
String SerialCommand = "";
String ATReturn = "";

void setup()
{
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  BTSerial.begin(115200);             // 啟動軟體序列埠,設定傳輸速度
  Serial.begin(115200);
}

void loop()
{ 
  if (queryBaud){
    Serial.print("Hello, Start MAC Serial port\r\n");
    queryBaud = false;
  }
  
  if (BTSerial.available() > 0)  //當arduino+藍牙收到手機 資料
  {
    delay(5);
    while ( BTSerial.available() ){
      delay(1); // Because return value is too long
      char c = BTSerial.read();
      ATReturn += c;
    }
      Serial.print( ATReturn );
      ATReturn = "";
  }
  
  if (Serial.available() > 0) 
  {
    delay(5);
    while ( Serial.available() ){
      delay(3);
      char c = Serial.read();
      SerialCommand += c;
    }
    
    if( SerialCommand.length()==1 ){
      switch (SerialCommand[0])
      {
      case  'a' :     // 
        
        break;  
      case  'x' :     // Set pin13 as HIGH
        digitalWrite(13, HIGH);
        break; 
      case  'y' :     // Set pin13 as LOW
        digitalWrite(13, LOW);
        break;  
      }
      
      SerialCommand = "";
    }
    else{
      Serial.println(SerialCommand);
      
      char BTSerialValue[50] = {'\0'};
      SerialCommand.toCharArray(BTSerialValue, 50);
      BTSerial.write( BTSerialValue );
      
      SerialCommand = "";
    }
  }
}
