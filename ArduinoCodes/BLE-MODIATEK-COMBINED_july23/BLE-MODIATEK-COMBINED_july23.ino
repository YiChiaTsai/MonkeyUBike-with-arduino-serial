//1. Disconnect the both paired ble by at command!
//2. Retrive string from remote bluetooth.
//3. Install Two BLE and test code by SoftwareSerial.
#include <SoftwareSerial.h>  // 使用arduino內建(軟體序列埠)程式庫
SoftwareSerial BTSerialPer(10, 11);    // MLP-PERIPHERAL設定軟體序列埠(接收腳RX,傳送腳TX)
SoftwareSerial BTSerialCen(3, 4);    // MLP-CENTRAL
char input;                  // 接收序列埠值的變數input資料型態為字元

boolean queryBaud = true;  //AT+BAUD=?

String SerialCommandCen = "";
String ATReturnCen = "";
String ATReturnPer = "";

String BleName = "";
String BleAddr = "";
String BleState = "";

void setup()
{
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(13, HIGH);
  digitalWrite(12, HIGH);
  BTSerialPer.begin(115200);   // 啟動軟體序列埠,設定傳輸速度
  BTSerialCen.begin(115200); 
  Serial.begin(115200);
}

void loop()
{
  if (queryBaud){
    Serial.print("Hello\r\n");
    queryBaud = false;
  }
  
//  if( BleName == "" ){
//    writeLongMessage("AT+NAME\r\n");
//  }
//  if( BleAddr == "" ){
//    writeLongMessage("AT+LADDR\r\n"); 
//  }
//  if (BTSerialPer.available() > 0)  //當arduino+藍牙收到手機 資料
//  {
//    delay(5);
//    while ( BTSerialPer.available() ){
//      delay(10); // Because return value is too long
//      char c = BTSerialPer.read();
//      ATReturnPer += c;
//    }
//      //Serial.write( BTSerial.read() );     
////      char BTSerialValue[50] = {'\0'};
////      ATReturn.toCharArray(BTSerialValue, 50);
////      Serial.write( BTSerialValue );
//      retriveSubstring(ATReturnPer);
//      Serial.print( ATReturnPer );
//      ATReturnPer = "";
//  }

  seizeRSSI();
  
  if (BTSerialCen.available() > 0)  //當arduino+藍牙收到手機 資料
  {
    delay(5);
    while ( BTSerialCen.available() ){
      delay(1); // Because return value is too long
      char c = BTSerialCen.read();
      ATReturnCen += c;
    }
      //Serial.write( BTSerialCen.read() );     
//      char BTSerialCenValue[50] = {'\0'};
//      ATReturnCen.toCharArray(BTSerialCenValue, 50);
//      Serial.write( BTSerialCenValue );

//      Serial.println( ATReturnCen );
      
      notifyRSSI( ATReturnCen );
      ATReturnCen = "";
  }
  
//  if (Serial.available() > 0)  //check if there's any data sent from the local serial terminal, you can add the other applications here
//  { 
//    delay(5);
//    while ( Serial.available() ){
//      delay(3);
//      char c = Serial.read();
//      SerialCommandCen += c;
//    }
//    
//    if( SerialCommandCen.length()==1 ){
//      switch (SerialCommandCen[0])
//      {
//      case  'a' :     // 接收到 'a'，做您要的動作1
//        Serial.println("AT\r");
//        BTSerialCen.write("AT\r\n");
//        break;
//      case  'b' :     // 接收到 'b'，做您要的動作2
//        Serial.println("AT+NAME\r");
//        BTSerialCen.write("AT+NAME\r\n");
//        break;
//      case  'c' :     // 接收到 'b'，做您要的動作2
//        Serial.println("AT+PIN5865\r");
//        BTSerialCen.write("AT+PIN5865\r\n");
//        break;
//      case  'd' :     // 接收到 'b'，做您要的動作2
//        Serial.println("AT+BAUD\r");
//        BTSerialCen.write("AT+BAUD\r\n");
//        break;
//      case  'e' :     // 接收到 'b'，做您要的動作2
//        Serial.println("AT+ROLE\r");
//        BTSerialCen.write("AT+ROLE\r\n");
//        break;
//      case  'f' :     // 接收到 'b'，做您要的動作2
//        Serial.println("AT+RNAME\r");
//        BTSerialCen.write("AT+RNAME\r\n");
//        break;
//      case  'g' :     // 接收到 'b'，做您要的動作2
//        Serial.println("AT+STATE\r");
//        BTSerialCen.write("AT+STATE\r\n");
//        break;
//      case  'h' :     // 接收到 'b'，做您要的動作2
//        Serial.println("AT+LADDR\r");
//        BTSerialCen.write("AT+LADDR\r\n");
//        break;
//      case  'i' :     // 接收到 'b'，做您要的動作2
//        Serial.println("AT+INQM1,5,10\r");
//        BTSerialCen.write("AT+INQM1,5,10\r\n");
//        break;
//      case  'j' :     // 接收到 'b'，做您要的動作2
//        Serial.println("AT+LSP\r");
//        BTSerialCen.write("AT+LSP\r\n");
//        break;
//      case  'k' :     // 接收到 'b'，做您要的動作2
//        Serial.println("AT+AUTOINQ1\r");
//        BTSerialCen.write("AT+AUTOINQ1\r\n");
//        break;
//      case  'l' :     // 接收到 'b'，做您要的動作2
//        Serial.println("AT+AUTOCONN1\r");
//        BTSerialCen.write("AT+AUTOCONN1\r\n");
//        break;
//      case  'm' :     // 接收到 'b'，做您要的動作2
//        Serial.println("AT+IPSCAN\r");
//        BTSerialCen.write("AT+IPSCAN\r\n");
//        break;
//      case  'n' :     // 接收到 'b'，做您要的動作2
//        Serial.println("AT+WIOMODE\r");
//        BTSerialCen.write("AT+WIOMODE\r\n");
//        break;
//      case  'o' :     // 接收到 'b'，做您要的動作2
//        Serial.println("AT+CONNECT80,C1,BA,73,11,A9\r");
//        BTSerialCen.write("AT+CONNECT80,C1,BA,73,11,A9\r\n");
//        break;
//      case  'p' :     // 接收到 'b'，做您要的動作2
//        Serial.println("AT+RESET\r");
//        BTSerialCen.write("AT+RESET\r\n");
//        break;
//      case  'x' :     // 接收到 'b'，做您要的動作2
//        digitalWrite(13, HIGH);
//        delay(10);
//        digitalWrite(12, HIGH);
//        break; 
//      case  'y' :     // 接收到 'b'，做您要的動作2
//        digitalWrite(13, LOW);
//        delay(10);
//        digitalWrite(12, LOW);
//        break;  
//      }
//      
//      SerialCommandCen = "";
//    }
//    else{
//      Serial.println(SerialCommandCen);
//      //BTSerialCen.write(SerialCommandCen);
//      char BTSerialCenValue[50] = {'\0'};
//      SerialCommandCen.toCharArray(BTSerialCenValue, 50);
//      BTSerialCen.write( BTSerialCenValue );
//      
//      SerialCommandCen = "";
//    }   
//  }
 
}

//void retriveSubstring(String ATReturnPer){
//  String ATReturnPerSubstring = ATReturnPer.substring(0,5);
//  if (ATReturnPerSubstring == "+NAME") 
//    BleName = ATReturnPer.substring(6,ATReturnPer.indexOf('\n')+1);
//   
//  if (ATReturnPerSubstring == "+LADD") 
//    BleAddr = ATReturnPer.substring(7,ATReturnPer.indexOf('\n')+1);
//}

void seizeRSSI(){
  delay(100);
  writeCenMessage("AT+INQ\r\n");
//  writeLongMessage("\nHey Sir, you have already connected to me, and my Name is\n");
//  writeLongVariable( BleName );
//  
//  writeLongMessage("Then My Mac Address is\n");
//  writeLongVariable( BleAddr );
//  
//  writeLongMessage("I am approaching behind you on the right, please watch out.\n");
//  writeLongMessage("Show your Monkeylectric pro with NCCUCS logo.\n\n\n");
  
//  digitalWrite(13, LOW);
//  delay(3000);
//  digitalWrite(13, HIGH);
}
void notifyRSSI( String ATReturnCen ){
  int indexINQ = ATReturnCen.indexOf("+INQ: 80:C1:BA:72:DF:E4,FF00,-");
    if( indexINQ != -1){
      indexINQ += 30;
      int RSSISignalInt = 0;
      RSSISignalInt = (ATReturnCen[indexINQ]-48)*10 + (ATReturnCen[indexINQ+1]-48);
      
      writePerMessage( "RSSI: ");
      writePerVariable( String(RSSISignalInt) );
      if( RSSISignalInt>70 && RSSISignalInt<100 )
        writePerMessage(" => Far!");
      else if( RSSISignalInt>0 && RSSISignalInt<70 )
        writePerMessage(" => Near!");
    }
}

void writePerMessage(char* message){
  BTSerialPer.write(message);
}
void writePerVariable(String variable){
  char BTSerialVariable[100] = {'\0'};
 
  variable.toCharArray(BTSerialVariable, 100);
  BTSerialPer.write(BTSerialVariable);
}

void writeCenMessage(char* message){
  BTSerialCen.write(message);
}
void writeCenVariable(String variable){
  char BTSerialVariable[100] = {'\0'};
 
  variable.toCharArray(BTSerialVariable, 100);
  BTSerialCen.write(BTSerialVariable);
}
