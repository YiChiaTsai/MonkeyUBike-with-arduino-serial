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

#define OCTAVE_OFFSET 0

// These values can also be found as constants in the Tone library (Tone.h)
int notes[] = { 0,
262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494,
523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988,
1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976,
2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951
};

//char *song = "The Simpsons:d=4,o=5,b=160:c.6,e6,f#6,8a6,g.6,e6,c6,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c6,8c6,8c6,c6";
//char *song = "Indiana:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6";
//char *song = "TakeOnMe:d=4,o=4,b=160:8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5,8f#5,8e5,8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5";
//char *song = "Entertainer:d=4,o=5,b=140:8d,8d#,8e,c6,8e,c6,8e,2c.6,8c6,8d6,8d#6,8e6,8c6,8d6,e6,8b,d6,2c6,p,8d,8d#,8e,c6,8e,c6,8e,2c.6,8p,8a,8g,8f#,8a,8c6,e6,8d6,8c6,8a,2d6";
//char *song = "Muppets:d=4,o=5,b=250:c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,8a,8p,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,8e,8p,8e,g,2p,c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,a,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,d,8d,c";
//char *song = "Xfiles:d=4,o=5,b=125:e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,g6,f#6,e6,d6,e6,2b.,1p,g6,f#6,e6,d6,f#6,2b.,1p,e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,e6,2b.";
//char *song = "Looney:d=4,o=5,b=140:32p,c6,8f6,8e6,8d6,8c6,a.,8c6,8f6,8e6,8d6,8d#6,e.6,8e6,8e6,8c6,8d6,8c6,8e6,8c6,8d6,8a,8c6,8g,8a#,8a,8f";
//char *song = "20thCenFox:d=16,o=5,b=140:b,8p,b,b,2b,p,c6,32p,b,32p,c6,32p,b,32p,c6,32p,b,8p,b,b,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,g#,32p,a,32p,b,8p,b,b,2b,4p,8e,8g#,8b,1c#6,8f#,8a,8c#6,1e6,8a,8c#6,8e6,1e6,8b,8g#,8a,2b";
//char *song = "Bond:d=4,o=5,b=80:32p,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d#6,16d#6,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d6,16c#6,16c#7,c.7,16g#6,16f#6,g#.6";
//char *song = "MASH:d=8,o=5,b=140:4a,4g,f#,g,p,f#,p,g,p,f#,p,2e.,p,f#,e,4f#,e,f#,p,e,p,4d.,p,f#,4e,d,e,p,d,p,e,p,d,p,2c#.,p,d,c#,4d,c#,d,p,e,p,4f#,p,a,p,4b,a,b,p,a,p,b,p,2a.,4p,a,b,a,4b,a,b,p,2a.,a,4f#,a,b,p,d6,p,4e.6,d6,b,p,a,p,2b";
//char *song = "StarWars:d=4,o=5,b=45:32p,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#.6,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#6";
//char *song = "GoodBad:d=4,o=5,b=56:32p,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,d#,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,c#6,32a#,32d#6,32a#,32d#6,8a#.,16f#.,32f.,32d#.,c#,32a#,32d#6,32a#,32d#6,8a#.,16g#.,d#";
//char *song = "TopGun:d=4,o=4,b=31:32p,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,16f,d#,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,g#";
//char *song = "A-Team:d=8,o=5,b=125:4d#6,a#,2d#6,16p,g#,4a#,4d#.,p,16g,16a#,d#6,a#,f6,2d#6,16p,c#.6,16c6,16a#,g#.,2a#";
//char *song = "Flinstones:d=4,o=5,b=40:32p,16f6,16a#,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,d6,16f6,16a#.,16a#6,32g6,16f6,16a#.,32f6,32f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,a#,16a6,16d.6,16a#6,32a6,32a6,32g6,32f#6,32a6,8g6,16g6,16c.6,32a6,32a6,32g6,32g6,32f6,32e6,32g6,8f6,16f6,16a#.,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#6,16c7,8a#.6";
//char *song = "Jeopardy:d=4,o=6,b=125:c,f,c,f5,c,f,2c,c,f,c,f,a.,8g,8f,8e,8d,8c#,c,f,c,f5,c,f,2c,f.,8d,c,a#5,a5,g5,f5,p,d#,g#,d#,g#5,d#,g#,2d#,d#,g#,d#,g#,c.7,8a#,8g#,8g,8f,8e,d#,g#,d#,g#5,d#,g#,2d#,g#.,8f,d#,c#,c,p,a#5,p,g#.5,d#,g#";
//char *song = "Gadget:d=16,o=5,b=50:32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,32d#,32f,32f#,32g#,a#,d#6,4d6,32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,8d#";
//char *song = "Smurfs:d=32,o=5,b=200:4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8f#,p,8a#,p,4g#,4p,g#,p,a#,p,b,p,c6,p,4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8b,p,8f,p,4f#";
//char *song = "MahnaMahna:d=16,o=6,b=125:c#,c.,b5,8a#.5,8f.,4g#,a#,g.,4d#,8p,c#,c.,b5,8a#.5,8f.,g#.,8a#.,4g,8p,c#,c.,b5,8a#.5,8f.,4g#,f,g.,8d#.,f,g.,8d#.,f,8g,8d#.,f,8g,d#,8c,a#5,8d#.,8d#.,4d#,8d#.";
//char *song = "LeisureSuit:d=16,o=6,b=56:f.5,f#.5,g.5,g#5,32a#5,f5,g#.5,a#.5,32f5,g#5,32a#5,g#5,8c#.,a#5,32c#,a5,a#.5,c#.,32a5,a#5,32c#,d#,8e,c#.,f.,f.,f.,f.,f,32e,d#,8d,a#.5,e,32f,e,32f,c#,d#.,c#";
char *song = "MissionImp:d=16,o=6,b=95:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,a#,g,2d,32p,a#,g,2c#,32p,a#,g,2c,a#5,8c,2p,32p,a#5,g5,2f#,32p,a#5,g5,2f,32p,a#5,g5,2e,d#,8d";
#define isdigit(n) (n >= '0' && n <= '9')

const int tonePin = 13;  // arbitrary for arduino boards, set this to whatever you want
boolean queryBaud = true;

String ATReturnPer = "";
String ATReturnCen = "";
String ATReturnUNO = "";

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
  
  seizeAddrRSSI();
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
  
  if (Serial2.available() > 0) 
  {
    delay(5);
    while ( Serial2.available() ){
      delay(1); // Because return value is too long
      char c = Serial2.read();
      ATReturnPer += c;
    }
      if(ATReturnPer == "x") {
        digitalWrite(12, HIGH);
        digitalWrite(11, LOW);
      }
      else if(ATReturnPer == "y") {
        digitalWrite(12, LOW);
        digitalWrite(11, LOW);
      }
      
      Serial.println(ATReturnPer);
      ATReturnPer = "";
  }
  
  if (Serial1.available() > 0) 
  {
    delay(100);
    while ( Serial1.available() ){
      delay(1);
      char c = Serial1.read();
      if(c == '!'){
        ATReturnUNO += c;
        break;
      }
      ATReturnUNO += c;
    }
//      Serial.print(ATReturnUNO);
//      writePerMessage("Deg1-Degr: ");
//      writePerVariable(  );
//      writePerMessage("!");
      
      ATReturnUNO = "";
  }
}

void seizeAddrRSSI()
{
//  delay(10);
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
//      Serial.print("Sto1-RSSI: ");
//      Serial.print( String(RSSISignalInt) );
//      Serial.println("!");
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
//      Serial.print("Sto2-RSSI: ");
//      Serial.print( String(RSSISignalInt) );
//      Serial.println("!");
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
//      Serial.print("Sto3-RSSI: ");
//      Serial.print( String(RSSISignalInt) );
//      Serial.println("!");
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
//      Serial.print("Sto4-RSSI: ");
//      Serial.print( String(RSSISignalInt) );
//      Serial.println("!");
    }
  //Store5
    indexINQ = ATReturnCen.indexOf("+INQ: ,FF00,-");
    if( indexINQ != -1){
      indexINQ += 30;
      int RSSISignalInt = 0;
      RSSISignalInt = (ATReturnCen[indexINQ]-48)*10 + (ATReturnCen[indexINQ+1]-48);
      
      writePerMessage("Sto5-RSSI: ");
      writePerVariable( String(RSSISignalInt) );
      writePerMessage("!");
    }
  //Music
    indexINQ = ATReturnCen.indexOf("+INQ: ,FF00,-");
    if( indexINQ != -1){
      play_rtttl(song);
      indexINQ += 30;
      int RSSISignalInt = 0;
      RSSISignalInt = (ATReturnCen[indexINQ]-48)*10 + (ATReturnCen[indexINQ+1]-48);
      
      writePerMessage("Musi-RSSI: ");
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

void play_rtttl(char *p)
{
  // Absolutely no error checking in here

  byte default_dur = 4;
  byte default_oct = 6;
  int bpm = 63;
  int num;
  long wholenote;
  long duration;
  byte note;
  byte scale;

  // format: d=N,o=N,b=NNN:
  // find the start (skip name, etc)

  while(*p != ':') p++;    // ignore name
  p++;                     // skip ':'

  // get default duration
  if(*p == 'd')
  {
    p++; p++;              // skip "d="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    if(num > 0) default_dur = num;
    p++;                   // skip comma
  }

  Serial.print("ddur: "); Serial.println(default_dur, 10);

  // get default octave
  if(*p == 'o')
  {
    p++; p++;              // skip "o="
    num = *p++ - '0';
    if(num >= 3 && num <=7) default_oct = num;
    p++;                   // skip comma
  }

  Serial.print("doct: "); Serial.println(default_oct, 10);

  // get BPM
  if(*p == 'b')
  {
    p++; p++;              // skip "b="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    bpm = num;
    p++;                   // skip colon
  }

  Serial.print("bpm: "); Serial.println(bpm, 10);

  // BPM usually expresses the number of quarter notes per minute
  wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)

  Serial.print("wn: "); Serial.println(wholenote, 10);


  // now begin note loop
  while(*p)
  {
    // first, get note duration, if available
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    
    if(num) duration = wholenote / num;
    else duration = wholenote / default_dur;  // we will need to check if we are a dotted note after

    // now get the note
    note = 0;

    switch(*p)
    {
      case 'c':
        note = 1;
        break;
      case 'd':
        note = 3;
        break;
      case 'e':
        note = 5;
        break;
      case 'f':
        note = 6;
        break;
      case 'g':
        note = 8;
        break;
      case 'a':
        note = 10;
        break;
      case 'b':
        note = 12;
        break;
      case 'p':
      default:
        note = 0;
    }
    p++;

    // now, get optional '#' sharp
    if(*p == '#')
    {
      note++;
      p++;
    }

    // now, get optional '.' dotted note
    if(*p == '.')
    {
      duration += duration/2;
      p++;
    }
  
    // now, get scale
    if(isdigit(*p))
    {
      scale = *p - '0';
      p++;
    }
    else
    {
      scale = default_oct;
    }

    scale += OCTAVE_OFFSET;

    if(*p == ',')
      p++;       // skip comma for next note (or we may be at the end)

    // now play the note

//    if( music == true){
      if(note){
        Serial.print("Playing: ");
        Serial.print(scale, 10); Serial.print(' ');
        Serial.print(note, 10); Serial.print(" (");
        Serial.print(notes[(scale - 4) * 12 + note], 10);
        Serial.print(") ");
        Serial.println(duration, 10);
        //tone(tonePin, notes[(scale - 4) * 12 + note]);
        delay(duration);
        //noTone(tonePin); 
      }
      else
      {
        Serial.print("Pausing: ");
        Serial.println(duration, 10);
        delay(duration);
      }
//    }
  }
}