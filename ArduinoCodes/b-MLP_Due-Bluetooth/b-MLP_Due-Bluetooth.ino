/*
HMC5883L_Example.ino - Example sketch for integration with an HMC5883L triple axis magnetometer.
 Copyright (C) 2013 BluLemonLabs (bluelemonlabs.blogspot.com)
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the version 3 GNU General Public License as
 published by the Free Software Foundation.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Reference the I2C Library
#include <Wire.h>
// Reference the HMC5883L Compass Library
#include <HMC5883L.h>

//#include <SoftwareSerial.h>  // 使用arduino內建(軟體序列埠)程式庫
//SoftwareSerial BTSerialPer(10, 11);    // MLP-PERIPHERAL設定軟體序列埠(接收腳RX,傳送腳TX) change to Serial2 on due
//SoftwareSerial BTSerialCen(8, 9);    // MLP-B1-CENTRAL change to Serial3 on due

boolean queryBaud = true;

String ATReturnPer = "";
String ATReturnCen = "";
String ATReturnUNOOld = "";
String ATReturnUNONew = "";

int counterRSSI = 0;
int arrayRSSI[3] = {0};
int averageRSSI = 0;

void setup()
{
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  pinMode(13, OUTPUT);           // set pin to input
  digitalWrite(13, HIGH);       // turn on pullup resistors
  Serial1.begin(115200);  //MLP-UNO
  //BTSerialPer.begin(115200);  // 啟動軟體序列埠,設定傳輸速度
  Serial2.begin(115200); //MLP-PERIPHERAL
  //BTSerialCen.begin(115200);
  Serial3.begin(115200); //MLP-CENTRAL
  Serial.begin(115200);
}

void loop()
{ 
  if (queryBaud){
    Serial.print("Hello, Start MLP Combined arduino\r\n");
    queryBaud = false;
  }
  
//  seizeAddrRSSI();
  if (Serial3.available() > 0)
  {
    delay(5);
    while ( Serial3.available() ){
      delay(1); // Because return value is too long
      char c = Serial3.read();
      ATReturnCen += c;
    }
      notifyAddrRSSI( ATReturnCen );
//      Serial.println( ATReturnCen );
      ATReturnCen = "";
  }
  
//  if (Serial2.available() > 0) 
//  {
//    delay(5);
//    while ( Serial2.available() ){
//      delay(1); // Because return value is too long
//      char c = Serial2.read();
//      ATReturnPer += c;
//    }
//      if(ATReturnPer == "x") {
//        digitalWrite(12, HIGH);
//        digitalWrite(11, LOW);
//      }
//      else if(ATReturnPer == "y") {
//        digitalWrite(12, LOW);
//        digitalWrite(11, LOW);
//      }
//      
//      Serial.println(ATReturnPer);
//      ATReturnPer = "";
//  }
  
//  if (Serial1.available() > 0) 
//  {
//    delay(100); //change the rate of receiving 
//    while ( Serial1.available() ){
//      delay(1);
//      char c = Serial1.read();
//      if(c == '!'){
//        ATReturnUNONew+= c;
//        break;
//      }
//      ATReturnUNONew += c;
//    }
////      if(ATReturnUNONew != ATReturnUNOOld)
////        Serial.println(ATReturnUNONew);
//        
//      if(ATReturnUNONew != ATReturnUNOOld)
//        writePerVariable(ATReturnUNONew);
//      
//      ATReturnUNOOld = ATReturnUNONew;
//      ATReturnUNONew = "";
//  }
}

void seizeAddrRSSI()
{
  delay(100);
  writeCenMessage("AT+INQ\r\n");
}

void notifyAddrRSSI( String ATReturnCen )
{
  //Bike1-New
  Serial.println(ATReturnCen);
  int indexINQ = ATReturnCen.indexOf("+INQ: 80:C8:D2:26:5B:BC,FF00,-");
    if( indexINQ != -1){
      indexINQ += 30;
      int RSSISignalInt = 0;
      RSSISignalInt = (ATReturnCen[indexINQ]-48)*10 + (ATReturnCen[indexINQ+1]-48);
      
        writePerMessage("Bike-RSSI: ");
        writePerVariable( String(RSSISignalInt) );
        writePerMessage("!");  
        
//        Serial.print("Bike-RSSI: ");
//        Serial.print( String(RSSISignalInt) );
//        Serial.println("!");
    }
  //Bike2-Old
  indexINQ = ATReturnCen.indexOf("+INQ: 80:C1:BA:72:DF:E4,FF00,-");
    if( indexINQ != -1){
      indexINQ += 30;
      int RSSISignalInt = 0;
      RSSISignalInt = (ATReturnCen[indexINQ]-48)*10 + (ATReturnCen[indexINQ+1]-48);
      
//      if( counterRSSI==3 )
//        counterRSSI = 0;
//        
//        arrayRSSI[0] = arrayRSSI[1];
//        arrayRSSI[1] = arrayRSSI[2];
//        arrayRSSI[2] = RSSISignalInt;
//         
//        averageRSSI = (arrayRSSI[0]+arrayRSSI[1]+arrayRSSI[2]) / 3;
      
        writePerMessage("Bike-RSSI: ");
        writePerVariable( String(RSSISignalInt) );
        writePerMessage("!");   
    }
  //Store1
    indexINQ = ATReturnCen.indexOf("+INQ: 80:C8:D2:28:30:93,FF00,-");
    if( indexINQ != -1){
      indexINQ += 30;
      int RSSISignalInt = 0;
      RSSISignalInt = (ATReturnCen[indexINQ]-48)*10 + (ATReturnCen[indexINQ+1]-48);
      
      writePerMessage("Sto1-RSSI: ");
      writePerVariable( String(RSSISignalInt) );
      writePerMessage("!");
    }
  //Store2
    indexINQ = ATReturnCen.indexOf("+INQ: 80:C7:DE:63:74:A2,FF00,-");
    if( indexINQ != -1){
      indexINQ += 30;
      int RSSISignalInt = 0;
      RSSISignalInt = (ATReturnCen[indexINQ]-48)*10 + (ATReturnCen[indexINQ+1]-48);
      
      writePerMessage("Sto2-RSSI: ");
      writePerVariable( String(RSSISignalInt) );
      writePerMessage("!");
    }
  //Store3
    indexINQ = ATReturnCen.indexOf("+INQ: 80:BC:72:42:65:98,FF00,-");
    if( indexINQ != -1){
      indexINQ += 30;
      int RSSISignalInt = 0;
      RSSISignalInt = (ATReturnCen[indexINQ]-48)*10 + (ATReturnCen[indexINQ+1]-48);
      
      writePerMessage("Sto3-RSSI: ");
      writePerVariable( String(RSSISignalInt) );
      writePerMessage("!");
    }
  //Store4
    indexINQ = ATReturnCen.indexOf("+INQ: 80:C1:BE:27:22:18,FF00,-");
    if( indexINQ != -1){
      indexINQ += 30;
      int RSSISignalInt = 0;
      RSSISignalInt = (ATReturnCen[indexINQ]-48)*10 + (ATReturnCen[indexINQ+1]-48);
      
      writePerMessage("Sto4-RSSI: ");
      writePerVariable( String(RSSISignalInt) );
      writePerMessage("!");
    }
  //Store5
    indexINQ = ATReturnCen.indexOf("+INQ: 80:BC:72:30:84:DD,FF00,-");
    if( indexINQ != -1){
      indexINQ += 30;
      int RSSISignalInt = 0;
      RSSISignalInt = (ATReturnCen[indexINQ]-48)*10 + (ATReturnCen[indexINQ+1]-48);
      
      writePerMessage("Sto5-RSSI: ");
      writePerVariable( String(RSSISignalInt) );
      writePerMessage("!");
    }
  //Store6
    indexINQ = ATReturnCen.indexOf("+INQ: 80:C8:B2:4E:C6:F9,FF00,-");
    if( indexINQ != -1){
      indexINQ += 30;
      int RSSISignalInt = 0;
      RSSISignalInt = (ATReturnCen[indexINQ]-48)*10 + (ATReturnCen[indexINQ+1]-48);
      
      writePerMessage("Sto6-RSSI: ");
      writePerVariable( String(RSSISignalInt) );
      writePerMessage("!");
    }
}

void writePerMessage(char* message){
  Serial2.write(message);
}
void writePerVariable(String variable){
  char BTSerialVariable[100] = {'\0'};
 
  variable.toCharArray(BTSerialVariable, 100);
  Serial2.write(BTSerialVariable);
}

void writeCenMessage(char* message){
  Serial3.write(message);
}
void writeCenVariable(String variable){
  char BTSerialVariable[100] = {'\0'};
 
  variable.toCharArray(BTSerialVariable, 100);
  Serial3.write(BTSerialVariable);
}
