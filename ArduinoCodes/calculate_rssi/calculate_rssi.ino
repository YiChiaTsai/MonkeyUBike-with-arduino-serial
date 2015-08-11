//利用rssi來判斷靠近或是遠離
//use rssi to calculate the device leaving or coming

#include <SoftwareSerial.h>  // 使用arduino內建(軟體序列埠)程式庫
SoftwareSerial BTSerial(10, 11);    // 設定軟體序列埠(接收腳RX,傳送腳TX)
char input;                  // 接收序列埠值的變數input資料型態為字元
String configuration = "";
char tempCo;
int countO = 0;
String getReturn;
int finishReturn=0;
String mac = "";
int didconnect = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  BTSerial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(BTSerial.available()){
    while(){
      tempCo = (char)BTSerial.read();
      delay(5);
      if(tempCo == '\n'){
        break;
      }
      configuration += tempCo;
      delay(5);
    }
  }
  
  if(configuration != ""){
    Serial.println(configuration);
    if(configuration.substring(0,4)== "+INQ:"){
      
    }
  }
}
