#include "SettingItemBool.h"

SettingItemBool::SettingItemBool(bool &value):_value(value)
{
   
}

SettingItemBool::~SettingItemBool()
{
}

void SettingItemBool::Left(){
    _value=!_value;
}

void SettingItemBool::Right(){
     _value=!_value;
}

void SettingItemBool :: UpdateVisual(LiquidCrystal_I2C &lcd, uint8_t line) {
    SettingItemBase::UpdateVisual(lcd,line);
    Serial.println("UpdateVisual bool");

    lcd.setCursor(12, line);
    if(canEditValue){
        lcd.write(byte(selectChar));
    }else{
        lcd.write(' ');
    }

    lcd.setCursor(13, line);
    if (_value)
        lcd.print("On");
    else
        lcd.print("Off");
}

bool SettingItemBool::GetValue()
{
    return _value;
}
