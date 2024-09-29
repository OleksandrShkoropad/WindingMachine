#include "WarningMessage.h"

WarningMessage::WarningMessage(LiquidCrystal_I2C &lcd): _lcd(lcd)
{
}

WarningMessage::~WarningMessage()
{
}

void WarningMessage::Init(uint8_t selectChar)
{
    _selectChar = selectChar;
}

void WarningMessage::UpdateScreen()
{
    if (!_isDirty)
        return;

    _lcd.setCursor(2, 0);
    _lcd.print("Reset turns?");
    _lcd.setCursor(3, 1);
    _lcd.print("Yes");
    _lcd.setCursor(11, 1);
    _lcd.print("No");

    _lcd.setCursor(2, 1);
    _lcd.print(" ");

    _lcd.setCursor(10, 1);
    _lcd.print(" ");

    uint8_t p = _selectYes ? 2 : 10;
    _lcd.setCursor(p, 1);
    _lcd.write(byte(_selectChar));
    _isDirty = false;
}

void WarningMessage::SetDirty()
{
    _isDirty = true;
}

void WarningMessage::Click()
{
    if(_selectYes && _yesFunc){
        _yesFunc();
    }else if(_noFunc){
        _noFunc();
    }
    Hide();
}

void WarningMessage::Left()
{
     if(!_isActive) return;
    _selectYes = !_selectYes;
    SetDirty();
}

void WarningMessage::Right()
{
    if(!_isActive) return;
    _selectYes = !_selectYes;
    SetDirty();
}

void WarningMessage::Show()
{
    if(_isActive) return;
    _lcd.clear();
    _selectYes = false;
    _isActive = true;
    SetDirty();
}

void WarningMessage::Hide()
{
    _isActive = false;
    _lcd.clear();
}

bool WarningMessage::IsActive()
{
    return _isActive;
}

void WarningMessage::OnClick(void (*yesFunc)(void), void (*noFunc)(void))
{
    _yesFunc = yesFunc;
    _noFunc = noFunc;
}
