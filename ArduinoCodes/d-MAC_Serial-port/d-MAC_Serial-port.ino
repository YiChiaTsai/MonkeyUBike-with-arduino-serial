//#include <SoftwareSerial.h>  // 使用arduino內建(軟體序列埠)程式庫
//SoftwareSerial BTSerial(10, 11);    // 設定軟體序列埠(接收腳RX,傳送腳TX)
//SoftwareSerial BTSerialDATA(8, 9);

boolean queryBaud = true;
String SerialCommand = "";
String ATReturn = "";
String ATReturnDATA = "";

void setup()
{ 
  Serial1.begin(115200); //MAC-CENTRAL
  Serial2.begin(115200); //MAC-DATA
  Serial.begin(115200);
}

void loop()
{ 
  if (queryBaud){
    Serial.print("Hello, Start MAC Serial port\r\n");
    queryBaud = false;
  }
    
  if (Serial1.available() > 0)  //當arduino+藍牙收到手機 資料
  {
    delay(5);
    while ( Serial1.available() ){
      delay(1); // Because return value is too long
      char c = Serial1.read();
      ATReturn += c;
    }
      Serial.print( ATReturn );
      ATReturn = "";
  }
    
  if (Serial2.available() > 0)  //當arduino+藍牙收到手機 資料
  {
    delay(5);
    while ( Serial2.available() ){
      delay(1); // Because return value is too long
      char c = Serial2.read();
      ATReturnDATA += c;
    }
      Serial.print( ATReturnDATA );
      ATReturnDATA = "";
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
//      BTSerial.write( BTSerialValue );
      
      SerialCommand = "";
    }
  }
}
