#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
class SoundController
{
private:
    uint8_t _pin = 8;
    unsigned long _previousMillis = 0;
    bool _playing = false;
    uint8_t _noteIndex = 0;
    unsigned long _noteStartTime = 0;
    uint16_t *_currentMelody = nullptr;    // Поточна мелодія
    uint8_t *_currentDurations = nullptr; // Поточні тривалості
    int _currentMelodyLength = 0;          // Довжина поточної мелодії



    uint16_t _m_start[6] = {800, 900, 1000, 1200, 1000, 900};
    uint8_t _d_start[6] = {8, 8, 8, 4, 8, 2};

    uint16_t _m_turn[1] = {700};
    uint8_t _d_turn[1] = {8};

    uint16_t _m_press[1] = {1300};
    uint8_t _d_press[1] = {8};

    uint16_t _m_step[2] = {1000,900};
    uint8_t _d_step[2] = {8,8};
    

public:

    SoundController(uint8_t pin);
    ~SoundController();
    enum MelodyTypeE
    {
        M_START,
        M_TURN,
        M_PRESS,
        M_STEP
    };
    void Init();
    void Update();
    void Play(MelodyTypeE melody);
};