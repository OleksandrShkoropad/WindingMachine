#pragma once
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class SettingItemBase
{
private:
    char _title[16] = "";
    bool _isSelected;
    void (*_clickFunc)()=nullptr;
protected:
    bool canEditValue;
    uint8_t selectChar;

public:
    SettingItemBase();
    ~SettingItemBase();
    void Init(uint8_t selectChar);
    void SetTitle(char title[]);
    void SetSelected(bool value);
    void Click();
    virtual void Left();
    virtual void Right();
    virtual void UpdateVisual(LiquidCrystal_I2C &lcd, uint8_t line);
    bool IsSelected();
    void OnClick(void (*userFunc)(void));
    bool IsEditValue();
};
