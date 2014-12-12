 //libraries
#include <LiquidCrystal.h>
#include "Timer.h"

Timer t;

// Define colour sensor LED pins
int ledArray[] = { 
  6,7,8};
LiquidCrystal lcd(12,11,5,4,3,2);

// boolean to know if the balance has been set
boolean balanceSet = false;

//place holders for colour detected
int red = 0;
int green = 0;
int blue = 0;

//floats to hold colour arrays
float colourArray[] = {
  0,0,0};
float whiteArray[] = {
  0,0,0};
float blackArray[] = {
  0,0,0};


//place holder for average
int avgRead;

void setup(){

  t.every(1000, takeReading);

  //setup the outputs for the colour sensor
  pinMode(6,OUTPUT);//red
  pinMode(7,OUTPUT);//green
  pinMode(8,OUTPUT);//blue
  //pin 9 goes to the buzzer
  pinMode(9, OUTPUT);
  //begin serial communication
  Serial.begin(9600);
  //initialize LCD
  lcd.begin(16, 2);
  lcd.clear();
}

void loop(){

  checkBalance();
  checkColour();
  printColour();
  t.update();
}

void checkBalance(){
  //check if the balance has been set, if not, set it
  if(balanceSet == false){
    setBalance();
  }
}

void setBalance(){
  //set white balance
  //clicking
  digitalWrite(9,HIGH);
  delay (100);
  digitalWrite(9,LOW);
  delay (200);
  digitalWrite(9,HIGH);
  delay (100);
  digitalWrite(9,LOW);

  lcd.clear();
  lcd.print("white balance");

  delay (100);
  delay(5000);//delay for five seconds, this gives us time to get a white sample in front of our sensor
  //scan the white sample.
  digitalWrite(9,HIGH);
  delay (100);
  digitalWrite(9,LOW);
  //go through each light, get a reading, set the base reading for each colour red, green, and blue to the white array
  for(int i = 0;i<=2;i++){
    digitalWrite(ledArray[i],HIGH);
    delay(100);
    getReading(5); //number is the number of scans to take for average, this whole function is redundant, one reading works just as well.
    whiteArray[i] = avgRead;
    digitalWrite(ledArray[i],LOW);
    lcd.clear();
    lcd.print("white set");
    delay(1000);
  }
  //done scanning white, now it will pulse blue to tell you that it is time for the black (or grey) sample.
  //set black balance

  lcd.clear();
  lcd.print("switch");
  delay (600);
  digitalWrite(9,HIGH);
  delay (100);
  digitalWrite(9,LOW);
  delay (100);
  digitalWrite(9,HIGH);
  delay (100);
  digitalWrite(9,LOW);
  delay (100);
  digitalWrite(9,HIGH);
  delay (100);
  digitalWrite(9,LOW);
  delay (100);

  lcd.clear();
  lcd.print("black balance");
  delay(5000);//wait for five seconds so we can position our black sample 
  digitalWrite(9,HIGH);
  delay (100);
  digitalWrite(9,LOW);
  //go ahead and scan, sets the colour values for red, green, and blue when exposed to black
  for(int i = 0;i<=2;i++){
    digitalWrite(ledArray[i],HIGH);
    delay(100);
    getReading(5);
    blackArray[i] = avgRead;
    //blackArray[i] = analogRead(2);
    digitalWrite(ledArray[i],LOW);
    lcd.clear();
    lcd.print("black set");
    delay(100);
  }
  //set boolean value so we know that balance is set
  balanceSet = true;
  //delay another 5 seconds to allow the human to catch up to what is going on

  delay(1000);
  digitalWrite(9,HIGH);
  delay (100);
  digitalWrite(9,LOW);
  delay (100);
  digitalWrite(9,HIGH);
  delay (100);
  digitalWrite(9,LOW);
  delay (100);
  digitalWrite(9,HIGH);
  delay (100);
  digitalWrite(9,LOW);
  delay (100);
  digitalWrite(9,HIGH);
  delay (100);
  digitalWrite(9,LOW);
  delay (100);
  lcd.clear();
  lcd.print("ready for action!");
  delay (2000);
  lcd.clear();
}

void checkColour(){
  for(int i = 0;i<=2;i++){
    digitalWrite(ledArray[i],HIGH);  //turn or the LED, red, green or blue depending which iteration
    delay(100);                      //delay to allow CdS to stabalize, they are slow
    getReading(5);                  //take a reading however many times
    colourArray[i] = avgRead;        //set the current colour in the array to the average reading
    float greyDiff = whiteArray[i] - blackArray[i];    //the highest possible return minus the lowest returns the area for values in between
    colourArray[i] = (colourArray[i] - blackArray[i])/(greyDiff)*255; //the reading returned minus the lowest value divided by the possible range multiplied by 255 will give us a value roughly between 0-255 representing the value for the current reflectivity(for the colour it is exposed to) of what is being scanned
    digitalWrite(ledArray[i],LOW);   //turn off the current LED
    delay(100);
  }
}
void getReading(int times){
  int reading;
  int tally=0;
  //take the reading however many times was requested and add them up
  for(int i = 0;i < times;i++){
    reading = analogRead(0);
    tally = reading + tally;
    delay(10);
  }
  //calculate the average and set it
  avgRead = (tally)/times;
}

//prints the colour in the colour array, in the next step, we will send this to processing to see how good the sensor works.
void printColour(){
  Serial.print("R = ");
  Serial.println(int(colourArray[0]));
  Serial.print("G = ");
  Serial.println(int(colourArray[1]));
  Serial.print("B = ");
  Serial.println(int(colourArray[2]));
  Serial.println(" ");
  //delay(2000);
}

void takeReading()
{
  int redVal = (colourArray[0]);
  int greenVal = (colourArray[1]);
  int blueVal = (colourArray[2]);


  int redRange = map(redVal,10, 272, 0, 100);
  int greenRange = map(greenVal,0, 312, 0, 100);
  int blueRange = map(blueVal,0, 270, 0, 100);

  lcd.clear();
  Serial.print("red range = ");
  Serial.println(redRange);
  Serial.print("green range = ");
  Serial.println(greenRange);
  Serial.print("blue range = ");
  Serial.println(blueRange);
  Serial.println(" ");

  //open
  if(redRange > 140 && greenRange > 140 && greenRange >140){
    Serial.println ("nothing");
    lcd.clear();
    lcd.print("no object");
    lcd.setCursor(0,1);
    lcd.print("detected");
  }
  //white
  if(redRange < 140 && greenRange < 140 && greenRange < 140
    &&redRange >= 75 && greenRange >= 75 && blueRange >= 75){
    lcd.clear();
    lcd.print("object is");
    lcd.setCursor(0,1);
    lcd.print("white");
  } 
  //black
  if(redRange < 20 && greenRange < 20 && blueRange <20){
    lcd.clear();
    lcd.print("object is");
    lcd.setCursor(0,1);
    lcd.print("black");
  }

  //red
  if(redRange < 140 && greenRange < 140 && greenRange < 140
    &&redRange >= 50 
    && greenRange < 50 
    && blueRange < 50){
    lcd.clear();
    lcd.print("object is");
    lcd.setCursor(0,1);
    lcd.print("red");
  }
  //orange
  if(redRange < 140 && greenRange < 140 && greenRange < 140
    &&redRange >= 50 
    && greenRange >= 50 && greenRange < 100
    && blueRange < 50){
    lcd.clear();
    lcd.print("object is");
    lcd.setCursor(0,1);
    lcd.print("orange");
  }
  //yellow
  if(redRange < 140 && greenRange < 140 && greenRange < 140
    &&redRange >= 75 
    && greenRange >= 75 
    && blueRange <50){
    lcd.clear();
    lcd.print("object is");
    lcd.setCursor(0,1);
    lcd.print("yellow");
  }
  //chartreuse
  if(redRange < 140 && greenRange < 140 && greenRange < 140
    &&redRange >= 75 && redRange <100
    && greenRange >= 75 
    && blueRange <50){
    lcd.clear();
    lcd.print("object is");
    lcd.setCursor(0,1);
    lcd.print("chartreuse");
  }
  //green
  if(redRange < 140 && greenRange < 140 && greenRange < 140
    &&redRange < 50 
    && greenRange >= 50 
    && blueRange < 50){
    lcd.clear();
    lcd.print("object is");
    lcd.setCursor(0,1);
    lcd.print("green");
  }
  //aquamarine
  if(redRange < 140 && greenRange < 140 && greenRange < 140
    &&redRange < 50 
    && greenRange >=50 
    && blueRange >=50 && blueRange <100){
    lcd.clear();
    lcd.print("object is");
    lcd.setCursor(0,1);
    lcd.print("aquamarine");
  }
  //cyan
  if(redRange < 140 && greenRange < 140 && greenRange < 140
    &&redRange < 50 
    && greenRange >= 50
    && blueRange >=50 ){
    lcd.clear();
    lcd.print("object is");
    lcd.setCursor(0,1);
    lcd.print("cyan");
  }
  //azure
  if(redRange < 140 && greenRange < 140 && greenRange < 140
    &&redRange < 50 
    && greenRange >= 50 && greenRange < 100
    && blueRange >= 50){
    lcd.clear();
    lcd.print("object is");
    lcd.setCursor(0,1);
    lcd.print("azure");
  }
  //blue
  if(redRange < 140 && greenRange < 140 && greenRange < 140
    &&redRange < 50 
    && greenRange < 50 
    && blueRange >= 50){
    lcd.clear();
    lcd.print("object is");
    lcd.setCursor(0,1);
    lcd.print("blue");
  }
  //violet
  if(redRange < 140 && greenRange < 140 && greenRange < 140
    &&redRange >= 50 && redRange < 100 
    && greenRange < 50 
    && blueRange >= 50){
    lcd.clear();
    lcd.print("object is");
    lcd.setCursor(0,1);
    lcd.print("violet");
  }
  //magenta
  if(redRange < 140 && greenRange < 140 && greenRange < 140
    &&redRange >= 50  
    && greenRange < 50 
    && blueRange >= 50){
    lcd.clear();
    lcd.print("object is");
    lcd.setCursor(0,1);
    lcd.print("magenta");
  }
  //rose
  if(redRange < 140 && greenRange < 140 && greenRange < 140
    &&redRange >= 50 
    && greenRange < 50 
    && blueRange >=50 && blueRange >100){
    lcd.clear();
    lcd.print("object is");
    lcd.setCursor(0,1);
    lcd.print("rose");
  }

}
















