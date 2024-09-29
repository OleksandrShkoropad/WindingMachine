#define KEY 17  //енкодер KEY
#define S1 15   //енкодер S1
#define S2 16   //енкодер S2

#define DIR1 5 // на кнопку живлення -/+
#define DIR2 6 // на кнопку живлення +/-
#define TURN 3 //пін на датчик хола

#define MAIN_MENU_SIZE 5 //максимальна кількість пунктів меню

#include <EncButton.h>
#include <LiquidCrystal_I2C.h>
#include "MenuController.h"
#include "SettingItemBool.h"

LiquidCrystal_I2C lcd(0x27, 16, 2); // Налаштування дисплея
MenuController menuController(lcd); //контролер меню налаштувань
EncButton eb(S1, S2, KEY); //зміна енкодера
SettingItemBase* settingMenu[MAIN_MENU_SIZE]; //пункти меню

volatile int turns;
int _lastTurn;
bool _dirUp;
bool _dirDown;
bool _selectYes;

bool _revers = false;
bool _beep = false;
bool _forceUpdate = false;
char _buffer[5];

#pragma region CustomChars
byte charUp[8] = {
    B00000,
    B00100,
    B01110,
    B11111,
    B00100,
    B00100,
    B00100,
    B00000};

byte charDown[8] = {
    B00000,
    B00100,
    B00100,
    B00100,
    B11111,
    B01110,
    B00100,
    B00000};

byte charPause[8] = {
    B00000,
    B11011,
    B11011,
    B11011,
    B11011,
    B11011,
    B11011,
    B00000};

byte selectChar[8] = {
    B00000,
    B01000,
    B01100,
    B01110,
    B01100,
    B01000,
    B00000,
    B00000};
#pragma endregion CustomChars

void handleInterrupt()  
{
  turns++;
}

void StartMessage()
{
  lcd.setCursor(4, 0);
  lcd.print("Welcome");
  delay(200);
  lcd.print(".");
  delay(200);
  lcd.print(".");
  delay(200);
  lcd.print(".");
  delay(200);
  lcd.clear();
}

void UpdateTurns()
{
  Serial.print("UpdateTurns");
  lcd.setCursor(5, 0);
  lcd.print("TURNS:");
  lcd.setCursor(5, 1);

  sprintf(_buffer, "%05d", turns);
  lcd.print(_buffer);
}


void OnClickBackHandler(){
  Serial.print("OnClickBackHandler");
  menuController.Hide();
  //UpdateTurns();
  _forceUpdate = true;
}

void ShowResetTurnsWarning()
{
  lcd.setCursor(2, 0);
  lcd.print("Reset turns?");
  lcd.setCursor(3, 1);
  lcd.print("Yes");
  lcd.setCursor(11, 1);
  lcd.print("No");

  lcd.setCursor(2, 1);
  lcd.print(" ");

  lcd.setCursor(10, 1);
  lcd.print(" ");

  byte p = _selectYes ? 2 : 10;
  lcd.setCursor(p, 1);
  lcd.print(">");
}

void OnClickResetHandler(){
   Serial.print("OnClickResetHandler");
   menuController.Hide();
   ShowResetTurnsWarning();
   //turns = 0;
}



void setup()
{
  Serial.begin(115200);
  turns = 0;
  pinMode(DIR1, INPUT);
  pinMode(DIR2, INPUT);
  pinMode(TURN, INPUT);

  attachInterrupt(digitalPinToInterrupt(TURN), handleInterrupt, FALLING);

  lcd.init();
  lcd.backlight(); // Включаем подсветку дисплея
  lcd.createChar(0, charUp);
  lcd.createChar(1, charDown);
  lcd.createChar(2, charPause);
  lcd.createChar(3, selectChar);

  StartMessage();

  settingMenu[0] = new SettingItemBase();
  settingMenu[0]->SetTitle("Back");
  settingMenu[0]->OnClick(OnClickBackHandler);

  settingMenu[1] = new SettingItemBase();
  settingMenu[1]->SetTitle("Reset");
  settingMenu[1]->OnClick(OnClickResetHandler);

  settingMenu[2] = new SettingItemBool(_revers);
  settingMenu[2]->SetTitle("Revers");

  settingMenu[3] = new SettingItemBool(_beep);
  settingMenu[3]->SetTitle("Beep");

  settingMenu[4] = new SettingItemBase();
  settingMenu[4]->SetTitle("Volume");

  menuController.Init(settingMenu, MAIN_MENU_SIZE,3);
  UpdateTurns();
}





void ShowDirChar(int value)
{
  lcd.setCursor(12, 1);
  lcd.write(byte(value));
}

void UpdateDirection()
{
  if (_dirUp <= 0 && _dirDown <= 0)
  {
    ShowDirChar(2);
  }
  if (_dirUp > 0)
  {
    ShowDirChar(0);
  }
  if (_dirDown > 0)
  {
    ShowDirChar(1);
  }
}

void loop()
{
  eb.tick();

  _dirUp = digitalRead(DIR1);
  _dirDown = digitalRead(DIR2);

  if (eb.left())
  {
    Serial.println("Left");
    menuController.Left();
  }
  if (eb.right())
  {
    Serial.println("Right");
    menuController.Right();
  }
  if (eb.click())
  {
    Serial.println("Click");
    if (!menuController.IsActive())
    {
      menuController.Show();
    }
    else
    {
      menuController.Click();
    }
  }

  //Serial.print("menuController.IsActive=");
  //Serial.println(menuController.IsActive());


  if (menuController.IsActive())
  {
    menuController.UpdateScreen();
    //delay(100);
     //Serial.println("1 UpdateScreen main");
  }
  else
  {
    //Serial.println("2 main");
    //delay(100);
    UpdateDirection();
    if (_lastTurn != turns || _forceUpdate)
    {
      UpdateTurns();
      _lastTurn = turns;
      _forceUpdate = false;
    }
  }
}
