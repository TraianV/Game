#include "LedControl.h" //  need the library
#include<LiquidCrystal.h>
#include <EEPROM.h>

const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;

const int RS = 7;
const int enable = 6;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;

int potValue ;
int indexMenu = 0;

LiquidCrystal lcd(RS,enable,d4,d5,d6,d7);

const int xPin = A0;
const int yPin = A1;

const int x1Pin = A2;
const int y1Pin = A3;

const int pinSW = 13;

const int pinset = 8;

const int pinSW1 = 9;
bool button2State = LOW;
bool lastButton2State = LOW;

unsigned int lastDebounce;
unsigned debounceDelay = 50;

const int valClar = 127;

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); //DIN, CLK, LOAD, No. DRIVER

byte matrixBrightness = 4;

byte xPos = 4;
byte yPos = 4;
byte xLastPos = 4;
byte yLastPos = 4;


const int minThreshold = 300;
const int maxThreshold = 700;

const byte moveInterval = 100;
unsigned long long lastMoved = 0;

const byte matrixSize = 8;
bool matrixChanged = true;

bool matrix[matrixSize][matrixSize] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}  
};

byte matrixByte[matrixSize] = {
  B00000000,
  B01000100,
  B00101000,
  B00010000,
  B00010000,
  B00010000,
  B00000000,
  B00000000
};


bool joyMoved = false;
int maxObt = 4;
int stare = 0;
int dif;
unsigned int aim = 3;
int reading ;

int intshot = 200;
int lastshot = 0;

long score = 0;
unsigned life = 3;

String msg = "ABC";
const byte byteMask = 0xFF;
const int byteLength = 8;

void setup() {
  Serial.begin(9600);
  
  pinMode(pinset,OUTPUT);
  
  pinMode(pinSW1, INPUT_PULLUP);
  
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, matrixBrightness); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  
  matrix[xPos][yPos] = 1;

  
  pinMode(pinSW, INPUT_PULLUP);
  // set up the LCD's number of columns and rows:
  lcd.begin(16,2);
  // Print a message to the 
  lcd.print("Created by");
  lcd.setCursor(0,1);
  lcd.print("Visan Traian");
  delay(1000);
  clearLinie(0);
  clearLinie(1);
  lcd.setCursor(0,0);
  lcd.print("Meniu");
  lcd.setCursor(0,1);
  lcd.print("Start");
}

void loop() {
  tranzitie();
  if(stare == 2)
  {
    Game();
    if(life == 0)
    {
      setName();
    }
  }
} 


String optiuneMeniu()
{
  
  if(indexMenu == 0)
  {
    return "Start";
  }
  else
    {
      if(indexMenu == 1)
      {
        return "Highscore";
      }
      else
        {
          if(indexMenu == 2)
          {
            return "Settings";
          }
          else{
            if(indexMenu == 3)
            {
              return "About";
            }
          }
        }
    }
    return "";
}


void clearLinie(int i)
{
  lcd.setCursor(0,i);
  lcd.print("                ");
  lcd.setCursor(0,i);
}


void toMeniu()
{
  lcd.clear();
  lcd.print("Meniu");
  lcd.setCursor(0,1);
  lcd.print(optiuneMeniu());
}


void tranzitie()
{
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);

  
  if(xValue < minThreshold && joyMoved == false)
  {
    moveDown();
    joyMoved = true;
  }
  if(xValue > maxThreshold && joyMoved == false)
  {
    moveUp();
    joyMoved = true;
  }
    
  if (yValue > maxThreshold && joyMoved == false)
  {
    moveLeft();
    joyMoved = true;
  }

  if (yValue < minThreshold && joyMoved == false)
  {
    moveRight(); 
    joyMoved = true;   
  }

  if(xValue >= minThreshold && xValue <= maxThreshold  &&  minThreshold <= yValue && yValue <= maxThreshold)
  {
    joyMoved = false;
  }
}


void moveDown(){
  if(stare == 0){
  if(indexMenu >0)
    {
      indexMenu--;
    }
    else
    {
      indexMenu = maxObt-1;
    }
    clearLinie(1);
    lcd.print(optiuneMeniu());
    }

  if(stare == 1)
  {
    if(dif >0)
    {
      dif--;
    }
    else
    {
      dif = 2;
    }
    clearLinie(1);
    lcd.print(dificultate());
  }
}


void moveUp(){
  if(stare == 0){ 
    if(indexMenu < maxObt-1) 
    {
      indexMenu++;
    }
    else
    {
      indexMenu = 0;
    }
    clearLinie(1);
    lcd.print(optiuneMeniu());
   }

   
  if(stare == 1)
  {
    if(dif < 3) 
    {
      dif++;
    }
    else
    {
      dif = 0;
    }
    clearLinie(1);
    lcd.print(dificultate());
  }
}


void moveRight(){
  if(stare == 0){
    if(indexMenu == 0)
    {
      stare = 1;
      stare1();
    }
    if(indexMenu == 1)
    {
      stare = -1;
      stare_1();
    }
    if(indexMenu == 2)
    {
      stare = -2;
      stare_2();
    }
    if(indexMenu == 3)
    {
      stare_3();
    }
  }
  else
    if(stare == 1)
    {
      stare = 2;
      start();  
    }
}
void stare_2(){
  notYeti();
}
void stare_3(){
  clearLinie(0);
  lcd.print("Created by");
  clearLinie(1);
  lcd.print("Visan Traian");
}

void moveLeft(){
  if(stare  ==  0)
  {
    toMeniu();
  }
  if(stare == 1)
  {
    stare = 0;
    toMeniu();
  }
  if(stare == -1)
  {
    stare = 0;
    toMeniu();
  }
  if(stare == -2)
  {
    stare = 0;
    toMeniu();
  }
  if(stare == -3)
  {
    stare = 0;
    toMeniu();
  }
}


void stare1(){
  clearLinie(0);
  lcd.print("Dificulty?");
  dif = 1;
  clearLinie(1);
  lcd.print(dificultate());
}


String dificultate()
{
  if(dif ==0){
    return "NO";
  }
  else{
    if(dif == 1){
      return "MEH";
    }
    else
      {
        return "YES";
      }
  }
}
void notYeti()
{
  clearLinie(0);
  lcd.print("PLEASE WAIT");
  clearLinie(1);
  lcd.print("In DEVELOPMENT");
}
void stare_1()
{
  theBest();
}
void theBest()
{
  clearLinie(0);
  lcd.print("THE BEST SCORES");
  clearLinie(1);
  lcd.print("NO SCORES YET");
}
void start()
{
  clearLinie(0);
  lcd.print("THE GAME");
  clearLinie(1);
  lcd.print("IS LOADING");
  delay(1500);
  clearLinie(0);
  lcd.print("Lives: ");
  lcd.setCursor(7,0);
  lcd.print(life);
  clearLinie(1);
  lcd.print("Score: ");
  lcd.setCursor(15
  ,1);
  lcd.print(score);
} 
void Game(){
  //updateByteMatrix();
  if (millis() - lastMoved > moveInterval) {
   // game logic
     updatePositions();
     lastMoved = millis();
   }
   if (matrixChanged == true) {
    // matrix display logic
     updateMatrix();
     matrixChanged = false;
  }
}
void generateFood() {
  // lastFoodPos = currentPos;
  // newFoodPos = random(ceva);
  // matrix[lastFoodPos] = 0;
  // matrix[newFoodPos] = 1;
  matrixChanged = true;
}

void updateByteMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    lc.setRow(0, row, matrixByte[row]);
  }
}

void updateMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrix[row][col]);
    }
  }
}

void updatePositions() {
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);

  int xDirec = analogRead(x1Pin);
  int yDirec = analogRead(y1Pin);

  reading = digitalRead(pinSW1);
  
  xLastPos = xPos;
  yLastPos = yPos;
  
  if (xValue < minThreshold) {
    
    if (xPos > 0) {
      xPos--;
    }
    else {
      xPos = matrixSize - 1;
    }
  }

  if (xValue > maxThreshold) {
    if (xPos < matrixSize - 1) {
      xPos++;
    } 
    else {
      xPos = 0;
    }
  }

  if (yValue > maxThreshold) {
    if (yPos > 0) {
      yPos--;
    }
    else {
      yPos = matrixSize - 1;
    }
  }

  if (yValue < minThreshold) {
    
    if (yPos < matrixSize - 1) {
      yPos++;
    } 
    else {
      yPos = 0;
    }
  }
  if (xPos != xLastPos || yPos != yLastPos) {
    matrixChanged = true;
    matrix[xLastPos][yLastPos] = 0;
    matrix[xPos][yPos] = 1;
  }

  if (xDirec < minThreshold &&  yDirec > maxThreshold) {
    aim = 8;
  }
  if (xDirec < minThreshold &&  yDirec < minThreshold) {
    aim = 2;
  }
  if (xDirec < minThreshold &&  (yDirec < maxThreshold  &&  yDirec > minThreshold)) {
    aim = 1;
  }
  if (xDirec > maxThreshold &&  yDirec > maxThreshold) {
    aim = 6;
  }

  if (xDirec > maxThreshold &&  yDirec < minThreshold) {
    aim = 4; 
  }
  if (xDirec > maxThreshold &&  (yDirec < maxThreshold  &&  yDirec > minThreshold)) {
    aim = 5;
  }
  if ((xDirec < maxThreshold  &&  xDirec > minThreshold)  &&  yDirec > maxThreshold) {
    aim = 7;
  }
  if ((xDirec < maxThreshold  &&  xDirec > minThreshold)  &&  yDirec < minThreshold) {
    aim = 3;
  }
  if(reading != lastButton2State){
    lastDebounce = millis();
  }
  if((millis() - lastDebounce ) > debounceDelay)
  {
    if(reading != button2State)
    {
      button2State = reading;
      if(button2State == HIGH)
      {
        shoot();
        Serial. println("da");
      }
    }
  }
  lastButton2State = reading;
}

void shoot(){
  int xst = xPos;
  int yst = yPos;
  int caim = aim;
  int ok = 1;
  
  while(ok == 1){
    if(millis() - lastshot > intshot)
    {
      if(caim == 1)
      {
        yst++;
      }
      if(caim == 2)
      {
        xst++;
        yst++;
      }
      if(caim == 3)
      {
        xst++;
      }
      if(caim == 4)
      {
        xst++;
        yst--;
      }
      if(caim == 5)
      {
        yst--;
      }
      if(caim == 6)
      {
        xst--;
        yst--;
      }
      if(caim == 7)
      {
        xst--;
      }
      if(caim == 8)
      {
        xst--;
        yst++;
      }
      if(xst == 0 ||  xst == matrixSize - 1  ||  yst  ==  0 ||  yst == matrixSize - 1){
        ok = 0;
      }
      matrix[xst][yst] = 1;
      lastshot = millis();
    }
    matrix[xst][yst] = 0;
  }
}

void writeEEPROM(int score) 
{
  byte firstByte = (score >> byteLength) & byteMask;
  byte secondByte = score & byteMask;
  EEPROM.update(0, firstByte);
  EEPROM.update(1, secondByte);
}

int readEEPROM() 
{
  byte firstByte = EEPROM.read(0);
  byte secondByte = EEPROM.read(1);
  return (firstByte << byteLength) + secondByte;
}

void setName(){
  clearLinie(0);
      lcd.print("Your Name:");
      clearLinie(1);
      lcd.print(msg);
}
