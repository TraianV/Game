#include <LedControl.h> //  need the library
#include <LiquidCrystal.h>
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

bool joyMoved = false;
int maxObt = 4;
int state = 0;
int dif = 1;
unsigned int aim = 3;
int reading ;

int intshot = 700;
int lastshot = 0;

long score = 0;
unsigned life = 3;

//String msg = "ABC";


int npcx1;
int npcx2;
int npcx3;
int npcy1;
int npcy2;
int npcy3;
bool alive1 = false;
bool alive2 = false;
bool alive3 = false;


long respawn1;
long respawn2;
long respawn3;

unsigned int nrnpc = 0;
unsigned int tipnpc1;
unsigned int tipnpc2;
unsigned int tipnpc3;

int timelapsed1 = 0;
int timelapsed2 = 0;
int timelapsed3 = 0;

int interv;
int multiplayer;
int respawnTime = 3000;
int speedProj;
//int contorMsg = 0;

int highscore1;
int highscore2;
int highscore3;
const byte byteMask = 0xFF;
const int byteLength = 8;
unsigned int highscoreIndex = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(pinset, OUTPUT);
  
  pinMode(pinSW1, INPUT_PULLUP);
  
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, matrixBrightness); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  
  matrix[xPos][yPos] = 1;
  
  highscore1 = readHighscore1EEPROM(); 
  highscore2 = readHighscore2EEPROM(); 
  highscore3 = readHighscore3EEPROM(); 
  
  pinMode(pinSW, INPUT_PULLUP);
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16,2);
  // Print a message to the 
  lcd.print("Created by");
  lcd.setCursor(0,1);
  lcd.print("Visan Traian");
  delay(1000);
  clearLine(0);
  clearLine(1);
  lcd.setCursor(0,0);
  lcd.print("Meniu");
  lcd.setCursor(0,1);
  lcd.print("Start");
}

void loop() {
  transition();
  Serial.println(indexMenu);
  if(state == 2)
  {
    Game();
  }
} 


String optionMeniu()
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


void clearLine(int i)
{
  lcd.setCursor(0,i);
  lcd.print("                ");
  lcd.setCursor(0,i);
}


void toMeniu()
{
  clearLine(0);
  lcd.print("Meniu");
  clearLine(1);
  lcd.print(optionMeniu());
}


void transition()
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
  if(state == 0){
    if(indexMenu >0)
    {
      indexMenu--;
    }
    else
    {
      indexMenu = maxObt-1;
    }
    clearLine(1);
    lcd.print(optionMeniu());
    }

  if(state == 1)
  {
    if(dif >0)
    {
      dif--;
    }
    else
    {
      dif = 2;
    }
    clearLine(1);
    lcd.print(dificultate());
  }
  if(state == -1)
  {
    if(dif >0)
    {
      highscoreIndex--;
    }
    else
    {
      highscoreIndex = 2;
    }
    clearLine(1);
    showScore();
  }
  /*else if(state == 4)
  {
    if(msg[IndexMsg] > 'B')
    {
      msg[contorMsg]--;
    }
    else
    {
      msg[contorMsg] = 'Z';
    }
    lcd.setCursor(0,1);
    lcd.print(msg);
  }
  */
}


void moveUp(){
  if(state == 0){ 
    if(indexMenu < maxObt-1) 
    {
      indexMenu++;
    }
    else
    {
      indexMenu = 0;
    }
    clearLine(1);
    lcd.print(optionMeniu());
   }

   
  else if(state == 1)
  {
    if(dif < 3) 
    {
      dif++;
    }
    else
    {
      dif = 0;
    }
    clearLine(1);
    lcd.print(dificultate());
  }
  else if(state == -1)
  {
    if(dif < 3) 
    {
      highscoreIndex++;
    }
    else
    {
      highscoreIndex = 0;
    }
    clearLine(1);
    showScore();
  }
  /*else if(state == 4)
  {
    if(msg[contorMsg] < 'Z')
    {
      msg[contorMsg]++;
    }
    else
    {
      msg[contorMsg] = 'A';
    }
    lcd.setCursor(0,1);
    lcd.print(msg);
  }
  */
}


void moveRight(){
  if(state == 0){
    if(indexMenu == 0)
    {
      state = 2;
      start();
    }
    if(indexMenu == 1)
    {
      state = -1;
      state_1();
    }
    if(indexMenu == 2)
    {
      state = 1;
      state1();
    }
    if(indexMenu == 3)
    {
      state_3();
    }
  }
  else if(state == 1)
    {
      state = 2;
      start();  
    }
  /*else if(state == 4)
  {
    contorMsg++;
    if(contorMsg  > 2)
    {
      savehighscore();
    }
  }
  */
}

void state_3(){
  clearLine(0);
  lcd.print("Created by");
  clearLine(1);
  lcd.print("Visan Traian");
}

void moveLeft(){
  if(state  ==  0)
  {
    toMeniu();
  }
  else if(state == 1)
  {
    state = 0;
    toMeniu();
  }
  else if(state == -1)
  {
    state = 0;
    indexMenu = 1;;
    toMeniu();
  }
  else if(state == -2)
  {
    state = 0;
    toMeniu();
  }
  else if(state == -3)
  {
    state = 0;
    toMeniu();
  }
  /*else if(state == 4  && contorMsg > 0)
  {
    contorMsg--;
  }
  */
}

void state1(){
  clearLine(0);
  lcd.print("Dificulty?");
  dif = 1;
  clearLine(1);
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

void showScore()
{
  if(highscoreIndex == 0)
  {
    lcd.print("1: ");
    lcd.print(highscore1);
  }
  else if(highscoreIndex == 1)
  {
    lcd.print("2: ");
    lcd.print(highscore2);
  }
  else
  {
    lcd.print("3: ");
    lcd.print(highscore3);
  }
}

void state_1()
{
  theBest();
}
void theBest()
{
  clearLine(0);
  lcd.print("THE BEST SCORES");
  clearLine(1);
  showScore();
}
void start()
{
  life = 3;
  score = 0;
  clearLine(0);
  lcd.print("THE GAME");
  clearLine(1);
  lcd.print("IS LOADING");
  delay(1500);
  clearLine(0);
  lcd.print("Lives: ");
  lcd.setCursor(7,0);
  lcd.print(life);
  clearLine(1);
  lcd.print("Score: ");
  lcd.setCursor(7,1);
  lcd.print(score);
} 
void Game(){
  reading = digitalRead(pinSW1);
  //updateByteMatrix();
  if (millis() - lastMoved > moveInterval) {
   // game logic
     updatePositions();
     lastMoved = millis();
   }
   if(reading != lastButton2State){
    lastDebounce = millis();
  }
  if((millis() - lastDebounce ) > debounceDelay)
  {
    if(reading != button2State)
    {
      button2State = reading;
      if(button2State == LOW)
      {
        shoot(xPos, yPos, aim);
      }
    }
  }
  
  lastButton2State = reading;
  if (matrixChanged == true) {
    // matrix display logic
     updateMatrix();
     matrixChanged = false;
  }
  if(nrnpc == 0)
  {
    verifdif();
  }
  else
  {
    npcbehave();
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
  
}

void shoot(int xst,int yst,int caim){
   if(caim == 1)
      {
        xst--;
      }
      if(caim == 2)
      {
        xst--;
        yst++;
      }
      if(caim == 3)
      {
        yst++;
      }
      if(caim == 4)
      {
        xst++;
        yst++;
      }
      if(caim == 5)
      {
        xst++;
      }
      if(caim == 6)
      {
        xst++;
        yst--;
      }
      if(caim == 7)
      {
        yst--;
      }
      if(caim == 8)
      {
        xst--;
        yst--;
      }
      if(xst >= 0 &&  xst < matrixSize  &&  yst  >=  0 &&  yst < matrixSize)
      {
        matrix[xst][yst] = 1;
        updateMatrix();
        if(xst  ==  npcx1  &&  yst ==  npcy1  &&  alive1 == true)
        {
            score = score + (100 * multiplayer);
            respawn1 = millis();
            alive1 = false;
            matrix[npcx1][npcy1] = 0;
            updateMatrix();
        }
        if(xst  ==  npcx2  &&  yst ==  npcy2  &&  alive2 == true)
        {
            score = score + (100 * multiplayer);
            respawn2 = millis();
            alive2 = false;
            matrix[npcx2][npcy2] = 0;
            updateMatrix();
        }
        if(xst  ==  npcx3  &&  yst ==  npcy3  &&  alive3 == true)
        {
            score = score + (100 * multiplayer);
            respawn3 = millis();
            alive3 = false;
            matrix[npcx3][npcy3] = 0;
            updateMatrix();
        }
      }
  while(xst >= 0 &&  xst < matrixSize  && yst  >=  0 &&  yst < matrixSize)
  {  
      matrix[xst][yst] = 0;
      updateMatrix();
      
      if(caim == 1)
      {
        xst--;
      }
      if(caim == 2)
      {
        xst--;
        yst++;
      }
      if(caim == 3)
      {
        yst++;
      }
      if(caim == 4)
      {
        xst++;
        yst++;
      }
      if(caim == 5)
      {
        xst++;
      }
      if(caim == 6)
      {
        xst++;
        yst--;
      }
      if(caim == 7)
      {
        yst--;
      }
      if(caim == 8)
      {
        xst--;
        yst--;
      }
      if(xst >= 0 &&  xst < matrixSize && yst  >=  0 &&  yst < matrixSize)
      {
        matrix[xst][yst] = 1;
        updateMatrix();
      }
      if(xst  ==  npcx1  &&  yst ==  npcy1  &&  alive1 == true)
      {
        score += (100 * multiplayer);
        lcd.setCursor(7,1);
        lcd.print(score);
        respawn1 = millis();
        alive1 = false;
        matrix[npcx1][npcy1] = 0;
        updateMatrix();
      }
      if(xst  ==  npcx2  &&  yst ==  npcy2  &&  alive2 == true)
      {
        score += (100 * multiplayer);
        lcd.setCursor(7,1);
        lcd.print(score);
        respawn2 = millis();
        alive2 = false;
        matrix[npcx2][npcy2] = 0;
        updateMatrix();
      }
      if(xst  ==  npcx3  &&  yst ==  npcy3  &&  alive3 == true)
      {
        score += (100 * multiplayer);
        lcd.setCursor(7,1);
        lcd.print(score);
        respawn3 = millis();
        alive3 = false;
        matrix[npcx3][npcy3] = 0;
        updateMatrix();
      }
   }
}

void verifdif()
{
  if(dif==0)
  {
    generatenpcez();
  }
  else if (dif == 1)
  {
    generatenpcmed();
  }
  else
  {
    generatenpchard();
  }
}
void generatenpcez()
{
    nrnpc = 1;
    tipnpc1 = 1;  
    interv = 3000;
    multiplayer = 1;
    speedProj = 200;
    
    npcx1 = matrixSize/2-1;
    npcy1 = matrixSize-1;
    alive1 = true;
    
    matrix[npcx1][npcy1] = 1;
    updateMatrix();
}
void generatenpcmed()
{
    nrnpc = 2;
    tipnpc1 = 2;
    tipnpc2 = 2;
    interv = 2000;
    multiplayer = 2;
    speedProj = 100;
    
    npcx1 = matrixSize/2-1;
    npcy1 = matrixSize-1;
    alive1 = true;
    
    npcx2 = matrixSize-1;
    npcy2 = matrixSize/2;
    alive2 = true;
    
    matrix[npcx1][npcy1] = 1;
    matrix[npcx2][npcy2] = 1;
    updateMatrix();
}
void generatenpchard()
{
    nrnpc = 3;
    tipnpc1 = 2;
    tipnpc2 = 2;
    tipnpc3 = 3;
    interv = 1500;
    multiplayer = 3;
    speedProj = 75;
    
    npcx1 = matrixSize/2-1;
    npcy1 = matrixSize-1;
    alive1 = true;
    
    npcx2 = matrixSize-1;
    npcy2 = matrixSize/2 - 1;
    alive2 = true;
    
    npcx3 = 0;
    npcy3 = matrixSize/2 - 1;
    alive3 = true;
    
    matrix[npcx1][npcy1] = 1;
    matrix[npcx2][npcy2] = 1;
    matrix[npcx3][npcy3] = 1;
    updateMatrix();
}
void npcbehave()
{
  if(dif == 0)
  {
    timelapsed1 = millis();
    npctip1();
  }
  else if(dif == 1)
  {
    npctip2(npcx1,npcy1,timelapsed1,alive1,respawn1);
    npctip2(npcx2,npcy2,timelapsed2,alive2,respawn2);
  }
  else
  {
    npctip2(npcx1,npcy1,timelapsed1,alive1,respawn1);
    npctip2(npcx2,npcy2,timelapsed2,alive2,respawn2);
    npctip3(npcx3,npcy3,timelapsed3,alive3,respawn3);
  }
}
void npctip1()
{
  
  long directionNpc1 = random(9);
  if(millis()-timelapsed1>interv  &&  alive1 == true)
  {
    if(xPos ==  npcx1)
    {
      if(yPos > npcy1)
      {
        npcshoot(npcx1, npcy1, 3);
      }
      else if(yPos  < npcy1)
      {
        npcshoot(npcx1, npcy1, 7);
      }
    }
    else if(yPos ==  npcy1)
    {
      if(xPos > npcx1)
      {
        npcshoot(npcx1, npcy1, 5);
      }
      else if(xPos  < npcx1)
      {
        npcshoot(npcx1, npcy1, 1);
      }
    }
    else
    { 
      if(xPos > npcx1 &&  yPos  > npcy1)
      {
        npcshoot(npcx1, npcy1, 4);
      }
      if(xPos > npcx1 &&  yPos  < npcy1)
      {
        npcshoot(npcx1, npcy1, 6);
      }
      if(xPos < npcx1 &&  yPos  > npcy1)
      {
        npcshoot(npcx1, npcy1, 2);
      }
      if(xPos < npcx1 &&  yPos  < npcy1)
      {
        npcshoot(npcx1, npcy1, 8);
      }
    }
    timelapsed1 = millis();
    npcmove(npcx1,npcy1,directionNpc1);
  }
  else if(alive1 == false)
  {
    if(millis()-respawn1 > respawnTime  )
    {
      alive1 = true;
      npcx1= matrixSize/2 - 1;
      npcy1 = matrixSize-1;
      matrix[npcx1][npcy1] = 1;
      updateMatrix();
    }
  }
}
void npctip2(int &xnpc, int &ynpc,int &timelapsed, bool &alive, long &respawn)
{
  unsigned randnr = random(9);
  if(millis()-timelapsed>interv &&  alive == true)
    {
      if(xPos ==  xnpc)
      {
        if(yPos > ynpc)
        {
          npcshoot(xnpc, ynpc, 3);
        }
        else if(yPos  < npcy1)
        {
          npcshoot(xnpc, ynpc, 7);
        }
      }
      else if(yPos ==  ynpc)
      {
        if(xPos > xnpc)
        {
        npcshoot(xnpc, ynpc, 5);
        }
        else if(xPos  < xnpc)
        {
          npcshoot(xnpc, ynpc, 1);
        }
      }
      else  if(xPos + yPos ==  xnpc + ynpc  ||  yPos - xPos == ynpc - xnpc)
      { 
        if(xPos > xnpc &&  yPos  > ynpc &&  xPos + yPos ==  xnpc + ynpc)
        {
          npcshoot(xnpc, ynpc, 4);
        }
        if(xPos > xnpc &&  yPos  < ynpc &&  xPos + yPos ==  xnpc + ynpc)
        {
          npcshoot(xnpc, ynpc, 6);
        }
        if(xPos < xnpc &&  yPos  > ynpc &&  yPos - xPos == ynpc - xnpc)
        {
          npcshoot(xnpc, ynpc, 2);
        }
        if(xPos < xnpc &&  yPos  < ynpc &&  yPos - xPos == ynpc - xnpc)
        {
          npcshoot(xnpc, ynpc, 8);
        }
      }
      else
      {
        if(ynpc - 1 ==  yPos)
        {
            npcmove(xnpc, ynpc, 7);
        }
        else if(ynpc + 1 ==  yPos)
        {
          npcmove(xnpc, ynpc, 3);
        }
        else if(xnpc + 1 ==  xPos)
        {
          npcmove(xnpc, ynpc, 5);
        }
        else if(xnpc - 1 ==  xPos)
        {
          npcmove(xnpc, ynpc, 1);
        }
        else if(xPos + yPos ==  xnpc + 1 + ynpc){
          npcmove(xnpc, ynpc, 3);
        }
        else if(xPos + yPos ==  xnpc - 1 + ynpc){
          npcmove(xnpc, ynpc, 7);
        }
        else if(yPos - xPos == ynpc - xnpc + 1){
          npcmove(xnpc, ynpc, 5);
        }
        else if(yPos - xPos == ynpc - xnpc - 1){
          npcmove(xnpc, ynpc, 1);
        }
        else
        {
          npcmove(xnpc, ynpc, randnr);
        }
      }
      timelapsed = millis();
    }
    else if(alive == false)
    {
      if(millis()-respawn > respawnTime)
      {
        alive = true;
        matrix[xnpc][ynpc] = 1;
        updateMatrix();
      }
  }
}
void npctip3(int &xnpc, int &ynpc,int &timelapsed, bool &alive, long &respawn)
{
  unsigned randnr = random(9);
  if(millis()-timelapsed>interv)
    {
      if(xPos ==  xnpc)
      {
        if(yPos > ynpc)
        {
          npcshoot(xnpc, ynpc, 3);
          npcmove(xnpc, ynpc, 3);
        }
        else if(yPos  < npcy1)
        {
          npcshoot(xnpc, ynpc, 7);
          npcmove(xnpc, ynpc, 7);
        }
      }
      else if(yPos ==  ynpc)
      {
        if(xPos > xnpc)
        {
        npcshoot(xnpc, ynpc, 5);
        npcmove(xnpc, ynpc, 5);
        }
        else if(xPos  < xnpc)
        {
          npcshoot(xnpc, ynpc, 1);
          npcmove(xnpc, ynpc, 1);
        }
      }
      else  if(xPos + yPos ==  xnpc + ynpc  ||  yPos - xPos == ynpc - xnpc)
      { 
        if(xPos > xnpc &&  yPos  > ynpc &&  xPos + yPos ==  xnpc + ynpc)
        {
          npcshoot(xnpc, ynpc, 4);
          npcmove(xnpc, ynpc, 4);
        }
        if(xPos > xnpc &&  yPos  < ynpc &&  xPos + yPos ==  xnpc + ynpc)
        {
          npcshoot(xnpc, ynpc, 6);
          npcmove(xnpc, ynpc, 6);
        }
        if(xPos < xnpc &&  yPos  > ynpc &&  yPos - xPos == ynpc - xnpc)
        {
          npcshoot(xnpc, ynpc, 2);
          npcmove(xnpc, ynpc, 2);
        }
        if(xPos < xnpc &&  yPos  < ynpc &&  yPos - xPos == ynpc - xnpc)
        {
          npcshoot(xnpc, ynpc, 8);
          npcmove(xnpc, ynpc, 8);
        }
      }
      else
      {
        if(ynpc - 1 ==  yPos)
        {
            npcmove(xnpc, ynpc, 7);
        }
        else if(ynpc + 1 ==  yPos)
        {
          npcmove(xnpc, ynpc, 3);
        }
        else if(xnpc + 1 ==  xPos)
        {
          npcmove(xnpc, ynpc, 5);
        }
        else if(xnpc - 1 ==  xPos)
        {
          npcmove(xnpc, ynpc, 1);
        }
        else if(xPos + yPos ==  xnpc + 1 + ynpc){
          npcmove(xnpc, ynpc, 3);
        }
        else if(xPos + yPos ==  xnpc - 1 + ynpc){
          npcmove(xnpc, ynpc, 7);
        }
        else if(yPos - xPos == ynpc - xnpc + 1){
          npcmove(xnpc, ynpc, 5);
        }
        else if(yPos - xPos == ynpc - xnpc - 1){
          npcmove(xnpc, ynpc, 1);
        }
        else
        {
          npcmove(xnpc, ynpc, randnr);
        }
      }
      timelapsed = millis();
    }
    else if(alive == false)
    {
      if(millis()-respawn > respawnTime)
      {
        alive = true;
        matrix[xnpc][ynpc] = 1;
        updateMatrix();
      }
  }
}
void npcmove(int &xnpc, int &ynpc, int npcaim)
{
  if(npcaim !=  0 ||  (xnpc != xPos &&  ynpc  != yPos))
  {
    matrix[xnpc][ynpc] = 0;
  }
  if(npcaim == 1  &&  xnpc >= 0)
  {
    xnpc--;
  }
  if(npcaim == 2  &&  xnpc > 0 &&  ynpc < matrixSize - 1)
  {
    xnpc--;
    ynpc++;
  }
  if(npcaim == 3  && ynpc < matrixSize - 1 )
  {
    ynpc++;
  }
  if(npcaim == 4  &&  xnpc  < matrixSize - 1  &&  ynpc  < matrixSize - 1)
  {
    xnpc++;
    ynpc++;
  }
  if(npcaim == 5  &&  xnpc  < matrixSize - 1)
  {
    xnpc++;
  }
  if(npcaim == 6  &&  xnpc  < matrixSize - 1  &&  ynpc  >  0)
  {
    xnpc++;
    ynpc--;
  }
  if(npcaim == 7  &&  ynpc > 0)
  {
    ynpc--;
  }
  if(npcaim == 8  &&  xnpc  > 0  &&  ynpc  >  0)
  {
    xnpc--;
    ynpc--;
  }
  if(npcaim !=  0)
  {
    matrix[xnpc][ynpc] = 1;
    updateMatrix();
  }
}
void npcshoot(int xnpc, int ynpc, int npcaim)
{ 
  int delayer = millis();
  if(npcaim == 1)
  {
    xnpc--;
  }
  if(npcaim == 2)
  {
    xnpc--;
    ynpc++;
  }
  if(npcaim == 3)
  {
    ynpc++;
  }
  if(npcaim == 4)
  {
    xnpc++;
    ynpc++;
  }
  if(npcaim == 5)
  {
    xnpc++;
  }
  if(npcaim == 6)
  {
    xnpc++;
    ynpc--;
  }
  if(npcaim == 7)
  {
    ynpc--;
  }
  if(npcaim == 8)
  {
    xnpc--;
    ynpc--;
  }
  updatePositions();
  if(xnpc >= 0 &&  xnpc < matrixSize && ynpc  > 0 &&  ynpc < matrixSize)
  {
    matrix[xnpc][ynpc] = 1;
    updateMatrix();
  }
  
  if(xnpc == xPos  &&  ynpc == yPos)
  {
    life--;
    lcd.setCursor(7,0);
    lcd.print(life);
    if(life == 0)
    {
      //setName();
      saveHighscore();
    }
  }
  while(millis() - delayer < speedProj)
  {
    ;
  }
  delayer = millis();
  
  while(xnpc >= 0 &&  xnpc < matrixSize && ynpc  > 0 &&  ynpc < matrixSize)
  {    
    matrix[xnpc][ynpc] = 0;
    updateMatrix();
    if(npcaim == 1)
    {
      xnpc--;
    }
    if(npcaim == 2)
    {
      xnpc--;
      ynpc++;
    }
    if(npcaim == 3)
    {
      ynpc++;
    }
    if(npcaim == 4)
    {
      xnpc++;
      ynpc++;
    }
    if(npcaim == 5)
    {
      xnpc++;
    }
    if(npcaim == 6)
    {
      xnpc++;
      ynpc--;
    }
    if(npcaim == 7)
    {
      ynpc--;
    }
    if(npcaim == 8)
    {
      xnpc--;
      ynpc--;
    }
    updatePositions();
    if(xnpc >= 0 &&  xnpc < matrixSize && ynpc  > 0 &&  ynpc < matrixSize)
    {
      matrix[xnpc][ynpc] = 1;
      updateMatrix();
    }
   
    if(xnpc == xPos  &&  ynpc == yPos)
    {
      life--;
      lcd.setCursor(7,0);
      lcd.print(life);
      if(life == 0)
        {
          //setName();
          saveHighscore();
        }
      
    }
  while(millis() - delayer < speedProj)
  {
    ;
  }
  delayer = millis();
  }
}

/*void setName(){
  clearLine(0);
  state = 4;
  lcd.print("Your Name:");
  clearLine(1);
  lcd.print(msg);
}
*/

void writeHighscore1NameEEPROM(char name[]) 
{
  byte firstByte = name[0] & byteMask;
  byte secondByte = name[1] & byteMask;
  byte thirdByte = name[2] & byteMask;
  EEPROM.update(2, firstByte);
  EEPROM.update(3, secondByte);
  EEPROM.update(4, thirdByte);
}

void writeHighscore1EEPROM(int score) 
{
  byte firstByte = (score >> byteLength) & byteMask;
  byte secondByte = score & byteMask;
  EEPROM.update(0, firstByte);
  EEPROM.update(1, secondByte);
}

void writeHighscore2EEPROM(int score) 
{
  byte firstByte = (score >> byteLength) & byteMask;
  byte secondByte = score & byteMask;
  EEPROM.update(5, firstByte);
  EEPROM.update(6, secondByte);
}

void writeHighscore3EEPROM(int score) 
{
  byte firstByte = (score >> byteLength) & byteMask;
  byte secondByte = score & byteMask;
  EEPROM.update(10, firstByte);
  EEPROM.update(11, secondByte);
}

int readHighscore1NameEEPROM() 
{
  byte firstByte = EEPROM.read(2);
  byte secondByte = EEPROM.read(3);
  byte thirdByte = EEPROM.read(4);
  return ((firstByte << byteLength) << byteLength) + (secondByte << byteLength) + thirdByte;
}

int readHighscore1EEPROM() 
{
  byte firstByte = EEPROM.read(0);
  byte secondByte = EEPROM.read(1);
  return (firstByte << byteLength) + secondByte;
}

int readHighscore2EEPROM() 
{
  byte firstByte = EEPROM.read(5);
  byte secondByte = EEPROM.read(6);
  return (firstByte << byteLength) + secondByte;
}

int readHighscore3EEPROM() 
{
  byte firstByte = EEPROM.read(10);
  byte secondByte = EEPROM.read(11);
  return (firstByte << byteLength) + secondByte;
}
void saveHighscore()
{
  state = 0;
  updateScore();
  emptyMatrix();
  toMeniu();
}
void updateScore()
{
  if (score > highscore1)
  {
    highscore3 = highscore2;
    highscore2 = highscore1;
    highscore1 = score;
    writeHighscore1EEPROM(highscore1);
    writeHighscore2EEPROM(highscore2);
    writeHighscore3EEPROM(highscore3);
  }
  else if (score > highscore2)
  {
    highscore3 = highscore2;
    highscore2 = score;
    writeHighscore2EEPROM(highscore2);
    writeHighscore3EEPROM(highscore3);
  }
  else if (score > highscore3)
  {
    
    highscore3 = score;
    writeHighscore3EEPROM(highscore3);
  }  
}
void emptyMatrix()
{
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      matrix[row][col] = 0;
    }
  }
  updateMatrix();
}
