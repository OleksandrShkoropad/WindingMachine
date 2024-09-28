#include "SettingItemBase.h"

class SettingItemBool : public SettingItemBase
{
private:
    bool _value;

public:
    SettingItemBool(bool value);
    ~SettingItemBool();
    void Left() override;
    void Right() override;
    void UpdateVisual(LiquidCrystal_I2C &lcd, uint8_t line) override;
    bool GetValue();
};