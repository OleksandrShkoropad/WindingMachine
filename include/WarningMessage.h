#include <Arduino.h>
#include<LiquidCrystal_I2C.h>
class WarningMessage {
private:
    LiquidCrystal_I2C &_lcd; 
    bool _selectYes;
    bool _isDirty;
    bool _isActive;
    void (*_yesFunc)()=nullptr;
    void (*_noFunc)()=nullptr;
    uint8_t _selectChar;

public:
    WarningMessage(LiquidCrystal_I2C &lcd);
    ~WarningMessage();
    void Init(uint8_t selectChar);

    void UpdateScreen();
    void SetDirty();

    void Click();
    void Left();
    void Right();

    void Show();
    void Hide();
    bool IsActive();
    void OnClick(void (*yesFunc)(void),void (*noFunc)(void));
};
