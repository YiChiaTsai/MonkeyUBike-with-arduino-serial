#include <SoftwareSerial.h>  // 使用arduino內建(軟體序列埠)程式庫
SoftwareSerial BTSerial(10, 11);    // 設定軟體序列埠(接收腳RX,傳送腳TX)

boolean queryBaud = true;
String SerialCommand = "";
String ATReturn = "";

void setup()
{
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  BTSerial.begin(115200);  // 啟動軟體序列埠,設定傳輸速度
  Serial.begin(115200);
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
        Serial.println("AT+NAMESTORE6-PERIPHERAL\r");
        BTSerial.write("AT+NAMESTORE6-PERIPHERAL\r\n");
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
      case  'f' :     // Set role as central1, peripheral0
        Serial.println("AT+ROLE\r");
        BTSerial.write("AT+ROLE\r\n");
        break;
      case  'g' :     // Get BLE address
        Serial.println("AT+LADDR\r");
        BTSerial.write("AT+LADDR\r\n");
        break;
      case  'h' :     // 0,3,25Get Inquire mode: mode, num, timeout
        Serial.println("AT+INQM\r");
        BTSerial.write("AT+INQM\r\n");
        break;
      case  'i' :     // (Bike which provides broadcast.) Set Inquire mode: mode, num, timeout
        Serial.println("AT+INQM0,8,5\r");
        BTSerial.write("AT+INQM0,8,5\r\n");
        break;
      case  'j' :     // MAC-CENTRAL MLP-PERIPHERAL (Store and Bind which provide connect.) Set Inquire mode: mode, num, timeout
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
      case  'm' :     // MAC-CENTRAL (Do set this!) Set autoinquire setting as false
        Serial.println("AT+AUTOINQ0\r");
        BTSerial.write("AT+AUTOINQ0\r\n");
        break;
      case  'n' :     // MAC-CENTRAL (Bike which provides broadcast.) Set autoconnect setting as false
        Serial.println("AT+AUTOCONN0\r");
        BTSerial.write("AT+AUTOCONN0\r\n");
        break;
      case  'o' :     // MLP-PERIPHERAL (Do not set this! But MLP-PERIPHERAL should set this!) Set autoinquire setting as true
        Serial.println("AT+AUTOINQ1\r");
        BTSerial.write("AT+AUTOINQ1\r\n");
        break;
      case  'p' :     // (Store which provides connect.) Set autoconnect setting as true
        Serial.println("AT+AUTOCONN1\rr");
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
      case  's' :     // Set bind address 80:C1:BE:26:18:98 80,C8,B2,58,5D,AF
        Serial.println("AT+BIND80,C8,B2,58,5D,AF\r");
        BTSerial.write("AT+BIND80,C8,B2,58,5D,AF\r\n");
        break;    
      case  't' :     // Get cmode state
        Serial.println("AT+CMODE\r");
        BTSerial.write("AT+CMODE\r\n");
        break; 
      case  'u' :     // For not bind mode, you must set as this. (specification is wrong!!!)
        Serial.println("AT+CMODE0\r");
        BTSerial.write("AT+CMODE0\r\n");
        break;  
      case  'v' :     // MAC-CENTRAL MLP-PERIPHERAL For bind mode, you must set as this.
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
