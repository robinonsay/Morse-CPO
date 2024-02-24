constexpr unsigned int BAUD = 57600;
constexpr unsigned int SERIAL_TIMEOUT = 10;  // ms
constexpr uint8_t LED_PIN = 8;
constexpr uint8_t KEYER_PIN = 7;
constexpr unsigned long DEBOUNCE = 10;  // ms

void setup()
{
    Serial.begin(BAUD);
    Serial.setTimeout(SERIAL_TIMEOUT);
    pinMode(KEYER_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
}

bool bState = false;
unsigned long uiTime = 0;
void loop()
{
    digitalWrite(LED_PIN, !digitalRead(KEYER_PIN));
    if(!digitalRead(KEYER_PIN))
    {
        if(!bState && (millis() - uiTime) > DEBOUNCE)
        {
            uiTime = millis();
            Serial.print('1');
            bState = true;
        }

    }
    else
    {
        if(bState && (millis() - uiTime) > DEBOUNCE)
        {
            uiTime = millis();
            Serial.print('0');
            bState = false;
        }
    }
}
