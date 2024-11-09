#include "SettingItemInt.h"

SettingItemInt::SettingItemInt(uint16_t &value, uint16_t min, uint16_t max) : _value(value),_min(min),_max(max)
{
}

SettingItemInt::~SettingItemInt()
{
}

void SettingItemInt::Left()
{
    if(_value>_min)_value--;
}

void SettingItemInt::Right()
{
    if(_value<_max)_value++;
}

void SettingItemInt ::UpdateVisual(LiquidCrystal_I2C &lcd, uint8_t line)
{
    SettingItemBase::UpdateVisual(lcd, line);
    // Serial.println("UpdateVisual bool");

    lcd.setCursor(12, line);
    if (canEditValue)
    {
        lcd.write(byte(selectChar));
    }
    else
    {
        lcd.write(' ');
    }

    lcd.setCursor(13, line);
    lcd.print(_value);
}

uint16_t SettingItemInt::GetValue()
{
    return _value;
}
