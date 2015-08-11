//1. Disconnect the both paired ble by at command!
//2. Retrive string from remote bluetooth.
//3. Install Two BLE and test code by SoftwareSerial.
#include <SoftwareSerial.h>  // 使用arduino內建(軟體序列埠)程式庫
SoftwareSerial BTSerial(10, 11);    // 設定軟體序列埠(接收腳RX,傳送腳TX)

boolean queryBaud = true;  //AT+BAUD=?

String SerialCommand = "";
String ATReturn = "";

String BleName = "";
String BleAddr = "";
String BleState = "";

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
    Serial.print("Hello\r\n");    
    queryBaud = false;
  }
  
  if( BleName == "" ){
    writeLongMessage("AT+NAME\r\n");
  }
  if( BleAddr == "" ){
    writeLongMessage("AT+LADDR\r\n"); 
  }
  
  if (BTSerial.available() > 0)  //當arduino+藍牙收到手機 資料
  {
    delay(5);
    while ( BTSerial.available() ){
      delay(10); // Because return value is too long
      char c = BTSerial.read();
      ATReturn += c;
    }
      //Serial.write( BTSerial.read() );     
//      char BTSerialValue[50] = {'\0'};
//      ATReturn.toCharArray(BTSerialValue, 50);
//      Serial.write( BTSerialValue );
      retriveSubstring(ATReturn);
      Serial.print( ATReturn );
      ATReturn = "";
  }
  
  WhenConnected();            
}

void retriveSubstring(String ATReturn){
  String ATReturnSubstring = ATReturn.substring(0,5);
  if (ATReturnSubstring == "+NAME") 
    BleName = ATReturn.substring(6,ATReturn.indexOf('\n')+1);
   
  if (ATReturnSubstring == "+LADD") 
    BleAddr = ATReturn.substring(7,ATReturn.indexOf('\n')+1);
}
void WhenConnected(){
  delay(1000);
  writeLongMessage("\nHey Sir, you have already connected to me, and my Name is\n");
  writeLongVariable( BleName );
  
  writeLongMessage("Then My Mac Address is\n");
  writeLongVariable( BleAddr );
  
  writeLongMessage("I am approaching behind you on the right, please watch out.\n");
  writeLongMessage("Show your Monkeylectric pro with NCCUCS logo.\n\n\n");
  
//  digitalWrite(13, LOW);
//  delay(3000);
//  digitalWrite(13, HIGH);
}

void writeLongMessage(char* message){
   BTSerial.write(message);
   delay(1000);
}
void writeLongVariable(String variable){
   char BTSerialBeacon[100] = {'\0'};
 
   variable.toCharArray(BTSerialBeacon, 100);
   BTSerial.write( BTSerialBeacon );
   delay(1000);
}
