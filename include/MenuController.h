#define SETTING_MENU_SIZE 4

#include<Arduino.h>
#include<LiquidCrystal_I2C.h>
#include "SettingItemBase.h"

class MenuController{
private:
    LiquidCrystal_I2C &_lcd; 

    SettingItemBase** _mainMenu;
    uint8_t _size;

    uint8_t _currentSelectIndex;
    uint8_t _currentViewIndex;
    bool _isDirty;
    bool _isActive;
    void ClearLine(uint8_t line);
    void SelectCurrent(uint8_t index);
public:
    MenuController(LiquidCrystal_I2C &lcd);
    ~MenuController();
    void Init(SettingItemBase* itemsArray[], uint8_t size,uint8_t selectChar);

    void UpdateScreen();
    void SetDirty();

    void Click();
    void Left();
    void Right();

    void Show();
    void Hide();
    bool IsActive();
};


