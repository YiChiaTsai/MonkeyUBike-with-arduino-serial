//1. Disconnect the both paired ble by at command!
//2. Retrive string from remote bluetooth.
//3. Install Two BLE and test code by SoftwareSerial.
#include <SoftwareSerial.h>  // 使用arduino內建(軟體序列埠)程式庫
SoftwareSerial BTSerial(10, 11);    // 設定軟體序列埠(接收腳RX,傳送腳TX)
char input;                  // 接收序列埠值的變數input資料型態為字元

int16_t abc=16666;
int qq = 0;
int initDelay = 0;
boolean startScan = false;
boolean connectedandTesting = false;  //true;
int connectedandTestingQ = 0;
boolean queryBaud = true;  //AT+BAUD=?

String SerialCommand = "";
String ATReturn = "";

void setup()
{
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  BTSerial.begin(9600);             // 啟動軟體序列埠,設定傳輸速度
  Serial.begin(9600);
}

void loop()
{
  
  if (queryBaud){
    Serial.print("Hello\r\n");
    queryBaud = false;
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
      Serial.print( ATReturn );
      
    
//      if( ATReturn.substring(0,4) == "Show"){
//        digitalWrite(13, LOW);
//        delay(100); 
//        digitalWrite(13, HIGH);
//      }    
      
      ATReturn = "";
  }
  
  if (Serial.available() > 0)  //check if there's any data sent from the local serial terminal, you can add the other applications here
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
      case  'a' :     // 接收到 'a'，做您要的動作1
        Serial.println("AT\r");
        BTSerial.write("AT\r\n");
        break;
      case  'b' :     // 接收到 'b'，做您要的動作2 設定名字
        Serial.println("AT+NAMEclientLin1\r");
        BTSerial.write("AT+NAMEclientLin1\r\n");
        break;
      case  'c' :     // 接收到 'b'，做您要的動作2
        Serial.println("AT+PIN835865\r");
        BTSerial.write("AT+PIN835865\r\n");
        break;
      case  'd' :     // 接收到 'b'，做您要的動作2 設定包瑞
        Serial.println("AT+BAUD4\r");
        BTSerial.write("AT+BAUD4\r\n");
        break;
      case  'e' :     // 接收到 'b'，做您要的動作2 設定主從 0從 1主
        Serial.println("AT+ROLE0\r");
        BTSerial.write("AT+ROLE0\r\n");
        break;
      case  'f' :     // 接收到 'b'，做您要的動作2  查詢藍牙名稱
        Serial.println("AT+RNAME80,C1,BA,73,11,A9\r");
        BTSerial.write("AT+RNAME80,C1,BA,73,11,A9\r\n");
        break;
      case  'g' :     // 接收到 'b'，做您要的動作2  查詢狀態
        Serial.println("AT+STATE\r");
        BTSerial.write("AT+STATE\r\n");
        break;
      case  'h' :     // 接收到 'b'，做您要的動作2  設定本機地址
        Serial.println("AT+LADDR\r");
        BTSerial.write("AT+LADDR\r\n");
        break;
      case  'i' :     // 接收到 'b'，做您要的動作2 設定一句RSSI 最大值來連接，只對主有效
        Serial.println("AT+INQM2,5,10\r");
        BTSerial.write("AT+INQM2,5,10\r\n");
        break;
      case  'j' :     // 接收到 'b'，做您要的動作2  設定已配对设备列表已配对设备列表
        Serial.println("AT+LSP\r");
        BTSerial.write("AT+LSP\r\n");
        break;
      case  'k' :     // 接收到 'b'，做您要的動作2  設定是否自動搜索 1要 0不要 從廣播
        Serial.println("AT+AUTOINQ1\r");
        BTSerial.write("AT+AUTOINQ1\r\n");
        break;
      case  'l' :     // 接收到 'b'，做您要的動作2  同上
        Serial.println("AT+INQ\r");
        BTSerial.write("AT+INQ\r\n");
        break;
      case  'm' :     // 接收到 'b'，做您要的動作2
        Serial.println("AT+IPSCAN\r");
        BTSerial.write("AT+IPSCAN\r\n");
        break;
      case  'n' :     // 接收到 'b'，做您要的動作2
        Serial.println("AT+WIOMODE\r");
        BTSerial.write("AT+WIOMODE\r\n");
        break;
      case  'o' :     // 接收到 'b'，做您要的動作2
        Serial.println("AT+CONNECT80,C1,BA,73,11,A9\r");
        BTSerial.write("AT+CONNECT80,C1,BA,73,11,A9\r\n");
        break;
      case  'p' :     // 接收到 'b'，做您要的動作2
        Serial.println("AT+RESET\r");
        BTSerial.write("AT+RESET\r\n");
        break;
      case  'x' :     // 接收到 'b'，做您要的動作2
        digitalWrite(13, HIGH);
        break; 
      case  'y' :     // 接收到 'b'，做您要的動作2
        digitalWrite(13, LOW);
        break;
      case  'z':
        Serial.println("AT+UUID\r");
        BTSerial.write("AT+UUID\r\n");
      }
      
      SerialCommand = "";
    }
    else{
      Serial.println(SerialCommand);
      //BTSerial.write(SerialCommand);
      char BTSerialValue[50] = {'\0'};
      SerialCommand.toCharArray(BTSerialValue, 50);
      BTSerial.write( BTSerialValue );
      
      SerialCommand = "";
    }
    
  }
}
