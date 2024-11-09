#include "SoundController.h"

SoundController::SoundController(uint8_t pin) : _pin(pin)
{
}

SoundController::~SoundController()
{
}

void SoundController::Init()
{
    pinMode(_pin, OUTPUT);
}

void SoundController::Update()
{

    if (!_playing || _currentMelody == nullptr)
    {
        return; // Немає мелодії для відтворення
    }

    tone(_pin, _currentMelody[_noteIndex]);

    unsigned long currentMillis = millis();

    if (currentMillis - _noteStartTime >= 1000 / _currentDurations[_noteIndex])
    {
        // Зупиняємо поточну ноту
        noTone(_pin);

        // Переходимо до наступної ноти
        _noteIndex++;

        // Якщо дійшли до кінця мелодії, зупиняємо її
        if (_noteIndex >= _currentMelodyLength)
        {
            _playing = false;
            return;
        }

        // Відтворюємо наступну ноту
        tone(_pin, _currentMelody[_noteIndex]);

        // Запам'ятовуємо час початку нової ноти
        _noteStartTime = currentMillis;
    }
}

void SoundController::Play(MelodyTypeE melody)
{
    // Вибираємо відповідну мелодію та її тривалості
    switch (melody)
    {
    case M_START:
        _currentMelody = _m_start;
        _currentDurations = _d_start;
        _currentMelodyLength = sizeof(_m_start) / sizeof(_m_start[0]);
        break;

    case M_TURN:
        _currentMelody = _m_turn;
        _currentDurations = _d_turn;
        _currentMelodyLength = sizeof(_m_turn) / sizeof(_m_turn[0]);
        break;

    case M_PRESS:
        _currentMelody = _m_press;
        _currentDurations = _d_press;
        _currentMelodyLength = sizeof(_m_press) / sizeof(_m_press[0]);
        break;

    case M_STEP:
        _currentMelody = _m_step;
        _currentDurations = _d_step;
        _currentMelodyLength = sizeof(_m_step) / sizeof(_m_step[0]);
        break;

        // Додаємо інші мелодії тут
    }

    // Скидаємо індекс ноти і статус для нової мелодії
    _noteIndex = 0;
    _playing = true;
    _noteStartTime = millis();
}
