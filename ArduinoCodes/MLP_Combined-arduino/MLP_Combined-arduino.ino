#include <SoftwareSerial.h>  // 使用arduino內建(軟體序列埠)程式庫
SoftwareSerial BTSerialPer(10, 11);    // MLP-PERIPHERAL設定軟體序列埠(接收腳RX,傳送腳TX)
SoftwareSerial BTSerialCen(8, 9);    // MLP-B1-CENTRAL

boolean queryBaud = true;

String ATReturnPer = "";
String ATReturnCen = "";

int counterRSSI = 0;
int arrayRSSI[3] = {0};
int averageRSSI = 0;

void setup()
{
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  BTSerialPer.begin(115200);  // 啟動軟體序列埠,設定傳輸速度
  BTSerialCen.begin(115200);
  Serial.begin(115200);
}

void loop()
{
  if (queryBaud){
    Serial.print("Hello, Start MLP Combined arduino\r\n");
    queryBaud = false;
  }
  
  seizeAddrRSSI();
  if (BTSerialCen.available() > 0)
  {
    delay(5);
    while ( BTSerialCen.available() ){
      delay(1); // Because return value is too long
      char c = BTSerialCen.read();
      ATReturnCen += c;
    }
      notifyAddrRSSI( ATReturnCen );
      ATReturnCen = "";
  }
}

void seizeAddrRSSI()
{
  delay(10);
  writeCenMessage("AT+INQ\r\n");
}

void notifyAddrRSSI( String ATReturnCen )
{
  //Bike1
  int indexINQ = ATReturnCen.indexOf("+INQ: 80:C1:BA:72:DF:E4,FF00,-");
    if( indexINQ != -1){
      indexINQ += 30;
      int RSSISignalInt = 0;
      RSSISignalInt = (ATReturnCen[indexINQ]-48)*10 + (ATReturnCen[indexINQ+1]-48);
      
      if( counterRSSI==3 )
        counterRSSI = 0;
        
        arrayRSSI[0] = arrayRSSI[1];
        arrayRSSI[1] = arrayRSSI[2];
        arrayRSSI[2] = RSSISignalInt;
         
        averageRSSI = (arrayRSSI[0]+arrayRSSI[1]+arrayRSSI[2]) / 3;
      
        writePerMessage("RSSI: ");
        writePerVariable( String(averageRSSI) );
                  
        if( averageRSSI>75 )
          writePerMessage(" => BikeFar!");
        else if( averageRSSI<=75 )
          writePerMessage(" => BikeNear!");        
    }
  //Store1
    indexINQ = ATReturnCen.indexOf("+INQ: 80:C1:BE:27:22:18,FF00,-");
    if( indexINQ != -1){
      indexINQ += 30;
      int RSSISignalInt = 0;
      RSSISignalInt = (ATReturnCen[indexINQ]-48)*10 + (ATReturnCen[indexINQ+1]-48);
      
      writePerMessage("RSSI: ");
      writePerVariable( String(RSSISignalInt) );
     
      if( RSSISignalInt<=75 )
        writePerMessage(" => Store1Appear!");
      else
        writePerMessage("!");
    }
  //Store2
    indexINQ = ATReturnCen.indexOf("+INQ: 80:BC:72:42:65:98,FF00,-");
    if( indexINQ != -1){
      indexINQ += 30;
      int RSSISignalInt = 0;
      RSSISignalInt = (ATReturnCen[indexINQ]-48)*10 + (ATReturnCen[indexINQ+1]-48);
      
      writePerMessage("RSSI: ");
      writePerVariable( String(RSSISignalInt) );
     
      if( RSSISignalInt<=75 )
        writePerMessage(" => Store2Appear!");
      else
        writePerMessage("!");
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
