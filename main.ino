#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>

// Setup Display
#define SCREEN_I2C_ADDR 0x3C // I2C address for the OLED display
#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 64     // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);


class Player {
  public:
    int x ;

    void playerPos(){
      display.setTextSize(2);
      display.setCursor(x, 50);
      display.write("_");

    }
};


class DropBall {
public:
    int x;
    int y;

    void generate(int xInit, int yInit) {
        x = xInit;
        y = yInit;
        display.setTextSize(1);
        display.setCursor(xInit, yInit);
        display.write("*");
    }

    void moveY(int xInit, int yInit) {
        x = xInit;
        y = yInit;
        display.setTextSize(1);
        display.setCursor(xInit, yInit);
        display.write("*");
    }
};

// Object Decleration
DropBall balls;
Player player;

int led = 13;

//Console Controls
const int start = 2;
const int reset = 3;
const int xAxies = A0; //505
const int yAxies = A1; //528

//Menu Control
const int lowestPoint = 1;
const int highestPoint = 110;
int level = 1;
int points = 0;
int graphicsPoint;// 0 - Menu
                  // 1 - GAME
                  // 2 - PAUSE
                  // 3 - SETTINGS

//Game Controls
bool isGameOn;
char menus[][10] = {"START","SETTINGS","MORE"};
char settingsMenus[][10] = {"TOP SCORE","RESET","BACK"};
char pausedMenu[][10] = {"RESUME","RESTART","EXIT"};
int menuIndexSelect;
int menuIndexToCome;
//Settings
int startTimer = 3;
const int eepromAddress = 0;
int topscore = EEPROM.read(eepromAddress);

void setup() {
//LED indicator
  pinMode(led,OUTPUT);

//Input
  pinMode(reset,INPUT_PULLUP);
  pinMode(start,INPUT_PULLUP);

//Display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);


//Game Init
  player.x = 0;
  isGameOn = false;
  graphicsPoint = 0;
  menuIndexSelect = 0;
  menuIndexToCome = menuIndexSelect + 1;

  Serial.begin(9600);
  Serial.println("Serial Begin");

//EEPROM init
  EEPROM.begin();
  
}
void loop() {
  
  display.clearDisplay();
  if(!isGameOn && 0 == graphicsPoint){
    gameMenu();
  }else if(1 == graphicsPoint){
    startGame();
  }else if(2 == graphicsPoint){
    pauseGame();
  }else if(3 == graphicsPoint){
    settingsMenu();
  }else if(4 == graphicsPoint){
    more();
  }else if(5 == graphicsPoint){
    topScore();
  }else if(6 == graphicsPoint){
    resetGame();
  }

  display.display();
}


void gameMenu(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(20, 0);
  display.print("SLIDER");

  display.setCursor(0,20);
  display.print(">");
  display.print(menus[menuIndexSelect]);
  display.setCursor(0,40);
  display.print(" ");
  display.print(menus[menuIndexToCome]);
  menuControls();

  if(!digitalRead(start)){
    if(menus[menuIndexSelect] == menus[0]){
      startGameCount();
    }else if(menus[menuIndexSelect] == menus[1]){
      menuIndexSelect = 0;
      menuIndexToCome = 1;
      delay(1000);
      graphicsPoint = 3;
    }else if(menus[menuIndexSelect] == menus[2]){
      graphicsPoint = 4;
    }
    delay(500);
  }

}

//Settings Functions
void settingsMenu(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(20, 0);
  display.print("SETTINGS");

  display.setCursor(0,20);
  display.print(">");
  display.print(settingsMenus[menuIndexSelect]);
  display.setCursor(0,40);
  display.print(" ");
  display.print(settingsMenus[menuIndexToCome]);

  if(!digitalRead(start)){
    if(settingsMenus[menuIndexSelect] == settingsMenus[0]){
      menuIndexSelect = 0;
      graphicsPoint = 5;
    }else if(settingsMenus[menuIndexSelect] == settingsMenus[1]){
      menuIndexSelect = 0;
      menuIndexToCome = 1;
      graphicsPoint = 6;
    }else if(settingsMenus[menuIndexSelect] == settingsMenus[2]){
      menuIndexSelect = 0;
      menuIndexToCome = 1;
      graphicsPoint = 0;
    }
    delay(500);
  }
  menuControls();

}

void topScore(){
  display.setCursor(10,0);
  display.setTextSize(2);
  display.print("TOP SCORE");
  display.setCursor(45,20);
  display.setTextSize(3);
  display.print(topscore);
  display.setTextSize(1);
  display.setCursor(40,50);
  display.print("> ");
  display.print("BACK");

  if(!digitalRead(start)){
    menuIndexSelect = 0;
    menuIndexToCome = 1;
    graphicsPoint = 0;
    delay(500);
  }
}

void resetGame(){
  display.setCursor(30,50);
  display.setTextSize(1);
  display.print("RESETING...");
  display.display();
  EEPROM.write(eepromAddress, 0);
  delay(5000);
  int eepromCheck = EEPROM.read(eepromAddress);
  display.clearDisplay();
  display.setCursor(30, 50);
  if(eepromCheck == 0){
    display.print("DONE!!");
    topscore = eepromCheck;
  }else{
    display.print("FAILED!!");
  }
  display.display();
  delay(2000);
  graphicsPoint = 0;
}

void gameSpeed(){
  if(10 > points){
    int level = 5;
  }
  if(20 > points){
    int level = 10;
  }
}

void more() {
  display.setCursor(0,0);
  display.setTextSize(1);
  display.print("find More at:");
  display.setCursor(0,20);
  display.print("www.yannkabambi.com");
  display.setCursor(40,50);
  display.print("> ");
  display.print("BACK");

  if(!digitalRead(start)){
    menuIndexSelect = 0;
    menuIndexToCome = 1;
    graphicsPoint = 0;
    delay(500);
  }

}

void startGameCount(){
  int count = startTimer;
  menuIndexSelect = 0;
  menuIndexToCome = 1;
  while(true){
    display.clearDisplay();
    display.setTextSize(3);
    display.setCursor(60, 20);
    display.print(count);
    display.display();
    count -=1;
    delay(1000);
    if(0 == count){
      graphicsPoint = 1;
      isGameOn = true;
      break;
    }
  }
  display.clearDisplay();
  display.setTextSize(3);
  display.setCursor(20, 20);
  display.print("START");
  display.display();
  delay(1000);
}

void startGame() {
  generateEnemy();
  displayPoints();
  gameControls();
  player.playerPos();
  delay(100/level);

}

void pauseGame(){

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(40, 0);
  display.print("GAME");
  display.setCursor(30, 20);
  display.print("PAUSED");

  display.setTextSize(1);
  display.setCursor(40,40);
  display.print(">");
  display.print(pausedMenu[menuIndexSelect]);
  display.setCursor(40,50);
  display.print(" ");
  display.print(pausedMenu[menuIndexToCome]);
  menuControls();

  //re-enter game
  if(!digitalRead(start) && pausedMenu[menuIndexSelect] == pausedMenu[0] ){
    menuIndexSelect = 0;
    menuIndexToCome = 1;
    graphicsPoint = 1;
    delay(1000);
  }else if(!digitalRead(start) && pausedMenu[menuIndexSelect] == pausedMenu[1]){
    points = 0;
    startGameCount();
    delay(500);
  }else if(!digitalRead(start) && pausedMenu[menuIndexSelect] == pausedMenu[2]){
    graphicsPoint = 0;
    isGameOn = false;
    menuIndexSelect = 0;
    menuIndexToCome = 1;
    delay(500);
  }
  
}

void menuControls(){
  if(analogRead(yAxies) > 528 ){
    if(2 == menuIndexSelect){
      menuIndexSelect = 0;
      menuIndexToCome = menuIndexSelect + 1;
    }else{
      menuIndexSelect +=1;
      if(menuIndexSelect + 1 == 3){
        menuIndexToCome = 0;
      }else{
        menuIndexToCome = menuIndexSelect + 1;
      }
    }

    delay(500);
  }
  
}

void gameControls(){
  if(isGameOn && graphicsPoint == 1 && !digitalRead(start)) {
    graphicsPoint = 2;
    delay(1000);
  }

  if((analogRead(xAxies) < 505) && isGameOn) {
    if(lowestPoint < player.x){
      player.x -=10;
    }
  }
  if((analogRead(xAxies) > 505) && isGameOn) {
    if(highestPoint > player.x){
      player.x +=10;
    }
  }

  if((balls.x >= player.x-10 && balls.x <= player.x+15) && balls.y == 50){
    points +=1;
  }
}

void displayPoints() {
  display.setTextSize(1);
  display.setCursor(55, 0);
  display.print(points);
  display.write("pts");
  if(topscore < points){
    EEPROM.write(eepromAddress, points);
    topscore = points;
  }
}

void generateEnemy() {
  balls.y = balls.y + 5;
  balls.moveY(balls.x, balls.y);

  if (60 == balls.y) {
      balls.y = random(-10, -50);
      balls.x = random(SCREEN_WIDTH);
  }
}