#include "cpo.hpp"

void setup()
{
    Serial.begin(BAUD);
    pinMode(KEYER_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
}

bool bState = false;
unsigned long uiTime = 0;
void loop()
{
    if(Serial.available())
    {
        String text = Serial.readString();
        for(size_t i = 0; i < text.length(); i++)
        {
            if(CHAR_TO_IDX(text[i]) == ' ')
            {
                delay(SPACE_MS);
                continue;
            }
            String code = morseMap[CHAR_TO_IDX(text[i])];
            for(size_t j = 0; j < code.length(); j++)
            {
                if(code[j] == '.')
                {
                    PLAY_TONE(DOT_MS);
                    delay(SYM_MS);
                }
                else if(code[j] == '-')
                {
                    PLAY_TONE(DASH_MS);
                    delay(SYM_MS);
                }
                else
                {
                    // Invalid Symbol
                }
            }
            delay(CHR_MS);
        }
    }
    digitalWrite(LED_PIN, !digitalRead(KEYER_PIN));
    if(!digitalRead(KEYER_PIN))
    {
        if(!bState && (millis() - uiTime) > DEBOUNCE)
        {
            uiTime = millis();
            Serial.print('1');
            bState = true;
            tone(SPKR_PIN, TONE_FREQ);
        }

    }
    else
    {
        if(bState && (millis() - uiTime) > DEBOUNCE)
        {
            uiTime = millis();
            Serial.print('0');
            bState = false;
            noTone(SPKR_PIN);
        }
    }
}
