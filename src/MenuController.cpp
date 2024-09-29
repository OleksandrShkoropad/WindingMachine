#define SCREEN_LINE_COUNT 2

#include "MenuController.h"

MenuController::MenuController(LiquidCrystal_I2C &lcd) : _lcd(lcd)
{
    _currentViewIndex = 0;
    _currentSelectIndex = 0;
    _isActive = false;
    _size = 0;
}

MenuController::~MenuController()
{
}

void MenuController::Init(SettingItemBase *itemsArray[], uint8_t size, uint8_t selectChar)
{
    _mainMenu = new SettingItemBase *[size];

    _size = size;

    for (int i = 0; i < size; ++i)
    {
        _mainMenu[i] = itemsArray[i];
        _mainMenu[i]->Init(selectChar);
    }
}

void MenuController::UpdateScreen()
{
    if (!_isDirty){
        return;
    }

    _currentViewIndex = floor(_currentSelectIndex / (float)SCREEN_LINE_COUNT) * SCREEN_LINE_COUNT;

    uint8_t view = _currentViewIndex;

    for (uint8_t i = 0; i < SCREEN_LINE_COUNT; i++)
    {
        if (view < _size)
        {
            _mainMenu[view]->UpdateVisual(_lcd, i);
        }
        else
        {
            ClearLine(i);
        }
        view++;
    }

    Serial.println("menuController.UpdateScreen()");
    _isDirty = false;
}

void MenuController::ClearLine(uint8_t line)
{
    _lcd.setCursor(0, line);
    _lcd.print("                ");
}

void MenuController::SetDirty()
{
    _isDirty = true;
}

void MenuController::Click()
{
    if(!_isActive) return;

    _mainMenu[_currentSelectIndex]->Click();
    SetDirty();
}

void MenuController::Left()
{
    if(!_isActive) return;

    if (_mainMenu[_currentSelectIndex]->IsEditValue())
    {
        _mainMenu[_currentSelectIndex]->Left();
    }
    else
    {
        if (_currentSelectIndex < _size - 1)
        {
            _currentSelectIndex++;
        }
        SelectCurrent(_currentSelectIndex);
    }
    SetDirty();
}

void MenuController::Right()
{
    if(!_isActive) return;

    if (_mainMenu[_currentSelectIndex]->IsEditValue())
    {
        _mainMenu[_currentSelectIndex]->Right();
    }
    else
    {
        if (_currentSelectIndex > 0)
        {
            _currentSelectIndex--;
        }
        SelectCurrent(_currentSelectIndex);
    }
    SetDirty();
}

void MenuController::SelectCurrent(uint8_t index)
{
    for (uint8_t i = 0; i < _size; i++)
    {
        _mainMenu[i]->SetSelected(i == index);
    }
}

void MenuController::Show()
{
    if (_isActive)
        return;

    _lcd.clear();
    _isActive = true;
    _currentSelectIndex = 0;
    _currentViewIndex = 0;
    SelectCurrent(0);
    SetDirty();
    Serial.println("menuController.Show()");
}

void MenuController::Hide()
{
    _lcd.clear();
    _isActive = false;
    SetDirty();
    
    Serial.println("menuController.Hide()");
}

bool MenuController::IsActive()
{
    return _isActive;
}
