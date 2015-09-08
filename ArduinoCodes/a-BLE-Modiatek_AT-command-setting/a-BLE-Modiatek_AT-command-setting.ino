#include <SoftwareSerial.h>  // 使用arduino內建(軟體序列埠)程式庫
SoftwareSerial BTSerial(10, 11);    // 設定軟體序列埠(接收腳RX,傳送腳TX)

boolean queryBaud = true;
String SerialCommand = "";
String ATReturn = "";

void setup()
{
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  BTSerial.begin(9600);  // 啟動軟體序列埠,設定傳輸速度
  Serial.begin(9600);
}

void loop()
{
  if (queryBaud){
    Serial.print("Hello, Start AT Command!\r\n");
    queryBaud = false;
  }
  
  if (BTSerial.available() > 0)
  {
    delay(5);
    while ( BTSerial.available() ){
      delay(1); // Because return value is too long, we need delay to collect data.
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
      case  'a' :     // Test connecting
        Serial.println("AT\r");
        BTSerial.write("AT\r\n");
        break;
      case  'b' :     // Set ble's name CENTRAL PERIPHERAL
        Serial.println("AT+NAMEMAC-DATA\r");
        BTSerial.write("AT+NAMEMAC-DATA\r\n");
        break;
      case  'c' :     // Set password
        Serial.println("AT+PIN835865\r");
        BTSerial.write("AT+PIN835865\r\n");
        break;
      case  'd' :     // Set baud rate as 115200
        Serial.println("AT+BAUD8\r");
        BTSerial.write("AT+BAUD8\r\n");
        break;
      case  'e' :     // Set role as central
        Serial.println("AT+BAUD\r");
        BTSerial.write("AT+BAUD\r\n");
        break;
      case  'f' :     // Set role as peripheral
        Serial.println("AT+ROLE0\r");
        BTSerial.write("AT+ROLE0\r\n");
        break;
      case  'g' :     // Get BLE address
        Serial.println("AT+LADDR\r");
        BTSerial.write("AT+LADDR\r\n");
        break;
      case  'h' :     // Get Inquire mode: mode, num, timeout
        Serial.println("AT+INQM0,3,25\r");
        BTSerial.write("AT+INQM0,3,25\r\n");
        break;
      case  'i' :     // (Bike which provides broadcast.) Set Inquire mode: mode, num, timeout
        Serial.println("AT+INQM0,8,5\r");
        BTSerial.write("AT+INQM0,8,5\r\n");
        break;
      case  'j' :     // (Store and Bind which provide connect.) Set Inquire mode: mode, num, timeout
        Serial.println("AT+INQM2,8,5\r");
        BTSerial.write("AT+INQM2,8,5\r\n");
        break;  
      case  'k' :     // Get autoinquire setting
        Serial.println("AT+AUTOINQ\r");
        BTSerial.write("AT+AUTOINQ\r\n");
        break;
      case  'l' :     // Get autoconnect setting     
        Serial.println("AT+AUTOCONN\r");
        BTSerial.write("AT+AUTOCONN\r\n");
        break;
      case  'm' :     // (Do set this!) Set autoinquire setting as false
        Serial.println("AT+AUTOINQ0\r");
        BTSerial.write("AT+AUTOINQ0\r\n");
        break;
      case  'n' :     // (Bike which provides broadcast.) Set autoconnect setting as false
        Serial.println("AT+AUTOCONN0\r");
        BTSerial.write("AT+AUTOCONN0\r\n");
        break;
      case  'o' :     // (Do not set this! But MLP-PERIPHERAL should set this!) Set autoinquire setting as true
        Serial.println("AT+AUTOINQ1\r");
        BTSerial.write("AT+AUTOINQ1\r\n");
        break;
      case  'p' :     // (Store which provides connect.) Set autoconnect setting as true
        Serial.println("AT+AUTOCONN1\r");
        BTSerial.write("AT+AUTOCONN1\r\n");
        break;
      case  'q' :     // Start inquiring nearby BLE
        Serial.println("AT+INQ\r");
        BTSerial.write("AT+INQ\r\n");
        break;
      case  'r' :     // Get bind address
        Serial.println("AT+BIND\r");
        BTSerial.write("AT+BIND\r\n");
        break;
      case  's' :     // Set bind address
        Serial.println("AT+BIND80,C1,BE,26,18,98\r");
        BTSerial.write("AT+BIND80,C1,BE,26,18,98\r\n");
        break;    
      case  't' :     // Get cmode state
        Serial.println("AT+CMODE\r");
        BTSerial.write("AT+CMODE\r\n");
        break; 
      case  'u' :     // For not bind mode, you must set as this. (specification is wrong!!!)
        Serial.println("AT+CMODE0\r");
        BTSerial.write("AT+CMODE0\r\n");
        break;  
      case  'v' :     // For bind mode, you must set as this.
        Serial.println("AT+CMODE1\r");
        BTSerial.write("AT+CMODE1\r\n");
        break;
      case  'w' :     // Clear all bind and memory address: u+v
        Serial.println("AT+CLEAR\r");
        BTSerial.write("AT+CLEAR\r\n");
        break;    
      case  'x' :     // Clear all bind and memory address: u+v
        Serial.println("AT+RESETPDL\r");
        BTSerial.write("AT+RESETPDL\r\n");
        break;
      case  'y' :     // Set pin13 as HIGH
        digitalWrite(13, HIGH);
        break;  
      case  'z' :     // Set pin13 as LOW
        digitalWrite(13, LOW);
        break; 
      }
      
      SerialCommand = "";
    }
    else{
      Serial.println( SerialCommand );
      
      char BTSerialValue[50] = {'\0'};
      SerialCommand.toCharArray(BTSerialValue, 50);
      BTSerial.write( BTSerialValue );
      
      SerialCommand = "";
    }
  }
}

//        Serial.println("AT+WIOMODE\r");
//        BTSerial.write("AT+WIOMODE\r\n");

//        Serial.println("AT+RESET\r");
//        BTSerial.write("AT+RESET\r\n");

//        //When Ready state, it will work.
//        Serial.println("AT+CONNECT80,C1,BA,70,E3,B5\r");
//        BTSerial.write("AT+CONNECT80,C1,BA,70,E3,B5\r\n");

//        Serial.println("AT+LSP\r");
//        BTSerial.write("AT+LSP\r\n");

//        Serial.println("AT+RNAME80,C1,BA,70,E3,B5\r");
//        BTSerial.write("AT+RNAME80,C1,BA,70,E3,B5\r\n");

//        Serial.println("AT+STATE\r");
//        BTSerial.write("AT+STATE\r\n");
