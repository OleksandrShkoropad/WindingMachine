#include "SettingItemBase.h"
#include <LiquidCrystal_I2C.h>

SettingItemBase::SettingItemBase()
{
    canEditValue = false;
    _isSelected = false;
}

SettingItemBase::~SettingItemBase()
{
}

void SettingItemBase::Init(uint8_t selectChar)
{
    this->selectChar = selectChar;
    // Serial.println("SettingItemBase Init");
}

void SettingItemBase::SetTitle(char title[])
{
    Serial.print(sizeof(title));
    for (int i = 0; i < 16; ++i)
    {
        if (i < strlen(title))
            _title[i] = title[i];
        else
            _title[i] = ' ';
    }
}

void SettingItemBase::SetSelected(bool value)
{
    _isSelected = value;
}

void SettingItemBase::Click()
{
    if (!_isSelected)
        return;
    
    if (_clickFunc)
        _clickFunc();
    else
        canEditValue = !canEditValue;
}

void SettingItemBase::Left()
{
}

void SettingItemBase::Right()
{
}

void SettingItemBase::UpdateVisual(LiquidCrystal_I2C &lcd, uint8_t line)
{
    Serial.println("UpdateVisual");
    lcd.setCursor(1, line);

    if (_isSelected && !canEditValue)
        lcd.write(byte(selectChar));
    else
        lcd.write(' ');

    lcd.setCursor(2, line);
    lcd.print(_title);
}

bool SettingItemBase::IsSelected()
{
    return _isSelected;
}

void SettingItemBase::OnClick(void (*clickFunc)(void))
{
    _clickFunc = clickFunc;
}

bool SettingItemBase::IsEditValue()
{
    return canEditValue;
}
