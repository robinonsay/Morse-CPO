#ifndef CPO_HPP
#define CPO_HPP

constexpr unsigned int BAUD = 9600;  // bps
constexpr unsigned int SERIAL_TIMEOUT = 10;  // ms
constexpr uint16_t TONE_FREQ = 500; // Hz
constexpr uint8_t SPKR_PIN = 9;
constexpr uint8_t LED_PIN = 8;
constexpr uint8_t KEYER_PIN = 7;
constexpr unsigned long DEBOUNCE = 10;  // ms
constexpr char START_MAP = ' ';

constexpr uint32_t WPM = 20;
constexpr uint32_t DOT_MS = 1200 / WPM;
constexpr uint32_t DASH_MS = 3 * DOT_MS; // Dash is three times as long as a dot
constexpr uint32_t SYM_MS = DOT_MS; // Gap between symbols within a character
constexpr uint32_t CHR_MS = 3 * DOT_MS; // Gap between symbols within a character
constexpr uint32_t SPACE_MS = 7 * DOT_MS; // Space between words


#define PLAY_TONE(duration) \
tone(SPKR_PIN, TONE_FREQ); digitalWrite(LED_PIN, HIGH); delay(duration); \
noTone(SPKR_PIN); digitalWrite(LED_PIN, LOW)

#define CHAR_TO_IDX(c) c - START_MAP

String morseMap[] = {
  // Starting from space (ASCII 32)
  "",       // Space
  "",       // ! (Reserved - but note that "!" has a Morse code representation, this placeholder is due to direct mapping issues)
  ".-.-.-", // "
  "",       // # (Reserved)
  "",       // $ (Reserved - but note that "$" has a Morse code representation, this placeholder is due to direct mapping issues)
  "",       // % (Reserved)
  ".-...",  // &
  ".----.", // '
  "-.--.",  // (
  "-.--.-", // )
  "",       // * (Reserved)
  ".-.-.",  // +
  "--..--", // ,
  "-....-", // -
  ".-.-.-", // .
  "-..-.",  // /
  "-----",  // 0
  ".----",  // 1
  "..---",  // 2
  "...--",  // 3
  "....-",  // 4
  ".....",  // 5
  "-....",  // 6
  "--...",  // 7
  "---..",  // 8
  "----.",  // 9
  "---...", // :
  "-.-.-.", // ;
  "",       // < (Reserved)
  "-...-",  // =
  "",       // > (Reserved)
  "..--..", // ?
  ".--.-.", // @
  ".-",     // A
  "-...",   // B
  "-.-.",   // C
  "-..",    // D
  ".",      // E
  "..-.",   // F
  "--.",    // G
  "....",   // H
  "..",     // I
  ".---",   // J
  "-.-",    // K
  ".-..",   // L
  "--",     // M
  "-.",     // N
  "---",    // O
  ".--.",   // P
  "--.-",   // Q
  ".-.",    // R
  "...",    // S
  "-",      // T
  "..-",    // U
  "...-",   // V
  ".--",    // W
  "-..-",   // X
  "-.--",   // Y
  "--..",   // Z
  "",       // [ (Reserved)
  "",       // \ (Reserved)
  "",       // ] (Reserved)
  "",       // ^ (Reserved)
  "..--.-", // _
};

#endif