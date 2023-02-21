/*receiver device gas detector system, data sensor transmitted via lora frequency 923 mhz with reyax RYLR993.*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>

const byte rx = 14; 
const byte tx = 12;
const byte bz = 13; //buzzer
const byte red = 15; //led red
const byte grn = 0; //led green
SoftwareSerial loraReceive (rx, tx);
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
String ppm;
int minim = 10; //min set ppm value
int maxim = 200; //max set ppm value
int i=0;

void setup() {
    pinMode(bz, OUTPUT);
    pinMode(red, OUTPUT); 
    pinMode(grn, OUTPUT);
    Serial.begin(9600);
    loraReceive.begin(9600);
    
 delay(1000);
 loraReceive.println("AT+OPMODE=1");//set module to properitary mode
 delay(500);  
 loraReceive.println("AT+BAND=915000000");//set band to 923MHZ
 delay(500);
 loraReceive.println("AT+ADDRESS=0");//set address module to 0
  //networkid default = 18, no need set up
 delay(1000);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(1000); 
  // intro static display
  display.clearDisplay();
  display.drawRect(0, 0, 126, 62, WHITE);
  display.setCursor(6,5);  
  display.setTextSize(3);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(35, 15);
  display.println("I.O");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 45);
  display.println("Ibnuwani Official");
  display.display();
  delay(3000);
}

//check incoming data
void incomingData()
    {
  if (loraReceive.available() > 0) {
    String inString = loraReceive.readString();     
    Serial.print("Received Data : ");
    Serial.println(inString); 
    String DataIn = getValue(inString, ',', 2);
    
    ppm = getValue(DataIn, '%', 0);
    displayData();
     }       
  }  

void displayData() 
    { 
//create rectangle on diplay
 display.clearDisplay();
 display.drawRect(1, 30, 125, 32, WHITE);
 display.setCursor(1,35);
    
//header display
     display.setTextSize(2);  
     display.setTextColor(BLACK, WHITE);
     display.setCursor(0,0);
     display.println("MONITORING");
//subheader display    
     display.setTextSize(1);  
     display.setTextColor( WHITE);
     display.setCursor(30,20);
     display.println("GAS STATION");
//show value gas detector       
     display.setTextSize(1);
     display.setTextColor(WHITE);
     display.setCursor(5,35);
     display.print("ppm = ");   
     display.setTextSize(1);
     display.setCursor(45,35);
     display.println(ppm); 
     display.display();
     delay(250); 
//convert string to int         
    String myString = ppm; 
    int myInt = myString.toInt();
    
//normal condition      
    if((myInt > minim ) &&(myInt < maxim))
        {
    digitalWrite(grn, HIGH);
    digitalWrite(red, LOW); 
    sirene_off();
    display.setTextSize(1);
    display.setCursor(5,45);
    display.println("system normal");  
    display.display();
    delay(100);   
}
//when gas or smoke detected    
   else if(myInt > maxim){
   digitalWrite(red, HIGH);
   digitalWrite(grn, LOW);    
 sirene_on();
 display.setTextSize(1);
 display.setCursor(5,45);    
 display.println("Gas Detected!");  
 display.display();
 delay(100);  
        }
//when sensor error reading
    else if (myInt < 10)
    {
  digitalWrite(red, HIGH);
  digitalWrite(grn, LOW);
  sirene_on();
  display.setTextSize(1);
  display.setCursor(5,45);
  display.println("sensor error!");
  display.display();   
  delay(100);  
        } 
    }

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;
 
    for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
            found++;
    strIndex[0] = strIndex[1] + 1;
    strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void sirene_on()
    {
tone(bz, 1000);
    delay(400);
    }

void sirene_off()
{
digitalWrite(bz,LOW);
    }

void loop() 
    {
  incomingData();
}