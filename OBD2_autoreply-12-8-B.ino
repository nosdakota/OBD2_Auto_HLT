

#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX


 //Rpm variables
 
  
volatile byte REV;       //  make a spot to store revolutions
unsigned long int rpm;       //  define rpm 
unsigned long time;         //  define space to store time of revolution
long prevtimeR = 0;        //define idle time for RPM

  //Speed variables

volatile byte REVS;       //  make a spot to store revolutions
unsigned long int rpmS;       //  define rpm 
unsigned long timeS;         //  define space to store time of revolution
long prevtimeS = 0;        //define idle time for Speed


String txTPS ="";
String txCAT ="";
String txMAP ="";
String txTMP ="";
String txRPMH ="";
String txRPML ="";
String txRPM0 ="41 0C 00 00";
String txSPD ="";
String txSPD0 ="41 0D 00";


void setup()
{ 

  
  mySerial.begin(9600);
  Serial.begin (9600);
    
 attachInterrupt(0, RPMCount, RISING);     //  creates an interrupt on int0 (pin2)
  REV = 0;      //  reset all variables to 0
  rpm = 0;
  time = 0;
  attachInterrupt(1, RPMSCount, RISING);     //  creates an interrupt on int1 (pin3)
  REVS = 0;      //  reset all variables to 0
  rpmS = 0;
  timeS = 0;
}


void loop()

{
  //-------------------------------TPS------------------------------//
float tpsv = analogRead(A0);            //Read Sensor TPS voltage
int tps = map(tpsv, 0, 1023, 0, 255);   //Remap  8bit? may be able to read directly
 Serial.write ("TPS");
 Serial.println (A0);
 Serial.println(tps);                     //debug
 

   txTPS = String((tps),HEX);           //convert to HEX string
   mySerial.write("41 11 ");              //format for ELM327
  mySerial.print(txTPS);                //data transmit
  mySerial.write(0x0D);                 //carriage return
 // mySerial.write(0x0D);                 //carriage return
  mySerial.write(0x3E);                 //New line
  //------------------------------TMP-----------------------------//
  float tmpv = analogRead(A1);            //Read Sensor TPS voltage
int tmp = map(tmpv, 0, 1023, 0, 255);   //Remap  8bit? may be able to read directly
 Serial.write ("TMP");
 Serial.println (A1);
 Serial.println(tmp);                     //debug
 

   txTMP = String((tmp),HEX);           //convert to HEX string
   mySerial.write("41 05 ");              //format for ELM327
  mySerial.print(txTMP);                //data transmit
  mySerial.write(0x0D);                 //carriage return
 // mySerial.write(0x0D);                 //carriage return
  mySerial.write(0x3E);                 //New line
 
   //------------------------------CAT-----------------------------//
  float catv = analogRead(A2);            //Read Sensor CAT voltage
int cat = map(catv, 0, 1023, -40, 13000);   //Remap  8bit? may be able to read directly
 Serial.write ("CAT");
 Serial.println (A2);
 Serial.println(cat);                     //debug
 

   txCAT = String((cat),HEX);
   mySerial.write("41 3C ");              //format for ELM327
  mySerial.print(txCAT);                //data transmit
  mySerial.write(0x0D);                 //carriage return
  //mySerial.write(0x0D);                 //carriage return
  mySerial.write(0x3E);                 //New line

 
  //-------------------------------RPM--------------------------//

long currtimeR = millis();              // Current arduino time
 long idletimeR = currtimeR - prevtimeR;        //  Calculate idle time
   if(REV >=5 )                         //  update rate, every 5 interrupts
   {
    rpm = 8*(60000/(millis() - time)*REV);       // Calculate RPM and multiply by 4 for ELM327 format 
                                                 
     time = millis();                            
     REV = 0;
     
  txRPMH = String(highByte(rpm),HEX);
  txRPML = String(lowByte(rpm),HEX);
   
  prevtimeR = currtimeR;                        // Reset Idletime
  }
      
     if(idletimeR >1000 )                    //  If there are no reading for 1 second execute 0 RPM print
  {
     
  Serial.println(txRPM0);               //Print 0 RPM string
    mySerial.print(txRPM0);               //Print 0 RPM string
    mySerial.write(0x0D);                 //carriage return
    mySerial.write(0x3E);                  //New line
      }   
  else
 {
       Serial.println(rpm,DEC);
    mySerial.write("41 0C ");                       //format for ELM327
    mySerial.print(txRPMH);                       //Print the RPM string
    mySerial.write(" "); 
    mySerial.print(txRPML); 
     mySerial.write(0x0D);                 //carriage return
     mySerial.write(0x3E);                 //New line
   }
 //-------------------------------Speed--------------------------//

long currtimeS = millis();              // Current arduino time
 long idletimeS = currtimeS - prevtimeS;        //  Calculate idle time
   if(REVS >=10 )                         //  update rate, every 10 interrupts
   {
    rpmS =((3600000/(millis() - timeS)*REVS)/4494);       // Calculate Speed in KM/h for ELM327 format  
                                                 
     timeS = millis();                            
     REVS = 0;
  txSPD = String(rpmS,HEX);
   prevtimeS = currtimeS;                        // Reset Idletime
  }
 
      if(idletimeS >500 )                    //  If there are no reading for 1/2 second execute 0 SPD0 print
   {
     
   Serial.println(txSPD0);               //Print 0 RPM string
    mySerial.print(txSPD0);               //Print 0 RPM string
     mySerial.write(0x0D);                 //carriage return
     mySerial.write(0x3E);                  //New line 
   }   
   else
   {
    Serial.println(rpmS,DEC);
    mySerial.write("41 0D ");                       //format for ELM327
    mySerial.print(txSPD);                        //Print the RPM string
      mySerial.write(0x0D);                 //carriage return
    //mySerial.write(0x0D);                 //carriage return
     mySerial.write(0x3E);                 //New line
   }
   
     
 }
 void RPMCount()                         // count and increase number stored in REV 
 {
   REV++;                                    
 }
 void RPMSCount()                         // count and increase number stored in REV 
 {
   REVS++;                                    
 }



