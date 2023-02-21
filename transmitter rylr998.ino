/*Transmitter devices gas detector sensor MQ-2, data sensor transmitted via lora Reyax rylr998*/

#include <SoftwareSerial.h>

#define MQ A0 //mq-2 pin
#define RX 6 //rx pin
#define TX 7 //tx pin
//#define piezoPin 5
#define recv 0 //address receiver
SoftwareSerial mySerial(RX,TX);
int sensorValue;
void setup() 
    {
  pinMode(MQ, INPUT);
    
  Serial.begin(9600);
  mySerial.begin(9600);
  delay(500);
  mySerial.println("AT+IPR=9600");//set baudrate module to 9600
  delay(500);
  mySerial.println("AT+BAND=923000000");//set band to 923MHZ
  delay(500);
  mySerial.println("AT+ADDRESS=1");//set address module to 1
  delay(500);
  mySerial.println("AT+NETWORKID=18"); //set network id to 18
 delay(1500);
    }

void loop() 
 {
 sensorValue =  analogRead(MQ);
  if (sensorValue >= 0)
      {
    delay(200);
    int a = sensorValue;
    String myStr;
    myStr=String(a);     
    String data = myStr ;
    commands(data, recv);
        }
    else
    {
    Serial.println("sensor error");
    }
}

void commands(String data, int address) {
String myString = "AT+SEND=" + String(address) + "," + String(data.length()) + "," + data + "\r\n";
char* buf = (char*) malloc(sizeof(char) * myString.length() + 1);
Serial.println(myString);
myString.toCharArray(buf, myString.length() + 1);
mySerial.write(buf);
free(buf);
    }