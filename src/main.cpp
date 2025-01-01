#define CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#define KEY 14 // енкодер KEY
#define S1 16  // енкодер S1
#define S2 15  // енкодер S2

#define DIR1 4 // на кнопку живлення -/+
#define DIR2 5 // на кнопку живлення +/-
#define TURN 2 // пін на датчик хола

#define BOOZER 8 // піщалка

#define MAIN_MENU_SIZE 7 // максимальна кількість пунктів меню

#include <EncButton.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include "MenuController.h"
#include "SettingItemBool.h"
#include "SettingItemInt.h"
#include "WarningMessage.h"
#include "SoundController.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);           // Налаштування дисплея
MenuController menuController(lcd);           // контролер меню налаштувань
EncButton eb(S1, S2, KEY);                    // Енкодер
SettingItemBase *settingMenu[MAIN_MENU_SIZE]; // пункти меню
WarningMessage warningMessage(lcd);           // вікно підтвердження скиду витків
SoundController soundController(BOOZER);      // контролер звуків

volatile uint32_t turns;
uint32_t _lastTurn;
bool _dirUp;
bool _dirDown;
bool _selectYes;

bool _forceUpdate = false;
char _buffer[6];
int _settingsAddress = 0;

struct Settings
{
  bool revers = false;
  bool beep = false;
  bool menuSound = false;
  uint16_t stepBeep = 0;
};
Settings _settings;

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
  int8_t _dirValue = 0;
  if (_dirUp)
    _dirValue = 1;
  if (_dirDown)
    _dirValue = -1;

  if (_settings.revers)
    _dirValue *= -1;

  if ((_dirValue > 0 && turns < 99999))
    turns++;
  else if (_dirValue < 0 && turns > 0)
  {
    turns--;
  }

  if (_dirValue != 0 && _settings.beep)
    soundController.Play(SoundController::M_TURN);

  if (_settings.stepBeep > 0 && _settings.stepBeep <= turns && turns % _settings.stepBeep == 0)
  {
    soundController.Play(SoundController::M_STEP);
  }
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
  if (_settings.stepBeep > 0)
  {
    lcd.setCursor(0, 1);
    lcd.print(_settings.stepBeep);
  }

  lcd.setCursor(5, 0);
  lcd.print("TURNS:");
  lcd.setCursor(5, 1);

  sprintf(_buffer, "%05lu", turns);
  lcd.print(_buffer);
}

void OnClickBackHandler()
{
  Serial.print("OnClickBackHandler");
  menuController.Hide();
  _forceUpdate = true;
}

void OnClickResetHandler()
{
  Serial.print("OnClickResetHandler");
  menuController.Hide();
  warningMessage.Show();
}

void OnClickSaveHandler()
{
  menuController.Hide();
  _forceUpdate = true;
  EEPROM.put(_settingsAddress, _settings);
}

void OnYesClickHandler()
{
  turns = 0;
  _forceUpdate = true;
}

void OnNoClickHandler()
{
  menuController.Show();
}

void setup()
{
  Serial.begin(115200);
  EEPROM.get(_settingsAddress, _settings);
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

  settingMenu[2] = new SettingItemBool(_settings.revers);
  settingMenu[2]->SetTitle("Revers");

  settingMenu[3] = new SettingItemBool(_settings.beep);
  settingMenu[3]->SetTitle("Beep");

  settingMenu[4] = new SettingItemInt(_settings.stepBeep, 0, 999);
  settingMenu[4]->SetTitle("Step beep");

  settingMenu[5] = new SettingItemBool(_settings.menuSound);
  settingMenu[5]->SetTitle("Menu sound");

  settingMenu[6] = new SettingItemBase();
  settingMenu[6]->SetTitle("Save");
  settingMenu[6]->OnClick(OnClickSaveHandler);

  menuController.Init(settingMenu, MAIN_MENU_SIZE, 3);

  warningMessage.Init(3);
  warningMessage.OnClick(OnYesClickHandler, OnNoClickHandler);

  soundController.Init();
  soundController.Play(soundController.M_START);
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

void PlayPressSound()
{
  if (_settings.menuSound)
    soundController.Play(SoundController::M_PRESS);
}

void loop()
{
  eb.tick();
  soundController.Update();

  _dirUp = digitalRead(DIR1);
  _dirDown = digitalRead(DIR2);

  if (eb.left())
  {
    Serial.println("Left");
    menuController.Left();
    warningMessage.Left();
    PlayPressSound();
  }
  if (eb.right())
  {
    Serial.println("Right");
    menuController.Right();
    warningMessage.Right();
    PlayPressSound();
  }
  if (eb.click())
  {
    Serial.println("Click");
    PlayPressSound();

    if (!menuController.IsActive() && !warningMessage.IsActive())
    {
      menuController.Show();
    }
    else if (warningMessage.IsActive())
    {
      warningMessage.Click();
    }
    else
    {
      menuController.Click();
    }
  }

  if (menuController.IsActive())
  {
    menuController.UpdateScreen();
  }
  else if (warningMessage.IsActive())
  {
    warningMessage.UpdateScreen();
  }
  else
  {
    UpdateDirection();
    if (_lastTurn != turns || _forceUpdate)
    {
      UpdateTurns();
      _lastTurn = turns;
      _forceUpdate = false;
    }
  }
}
