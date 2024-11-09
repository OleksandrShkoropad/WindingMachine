#include "SettingItemBase.h"

class SettingItemInt : public SettingItemBase
{
private:
    uint16_t &_value;
    uint16_t _min;
    uint16_t _max;

public:
    SettingItemInt(uint16_t &value,uint16_t min,uint16_t max);
    ~SettingItemInt();
    void Left() override;
    void Right() override;
    void UpdateVisual(LiquidCrystal_I2C &lcd, uint8_t line) override;
    uint16_t GetValue();
};