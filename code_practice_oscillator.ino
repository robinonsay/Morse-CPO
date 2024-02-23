constexpr uint8_t SPKR_PIN = 9;
constexpr uint8_t KEYER_PIN = 7;
constexpr unsigned int SPKR_FREQ = 750;
constexpr uint16_t WPM = 13;
constexpr uint16_t DOT_DUR = 1200/WPM;
constexpr uint16_t DASH_DUR = DOT_DUR * 3;
constexpr uint16_t SYM_SPC = DOT_DUR;
constexpr uint16_t CHR_SPC = DOT_DUR * 3;
constexpr uint16_t WRD_SPC = DOT_DUR * 7;

uint32_t uiBtnPrsdTime = 0;
uint32_t uiBtnRelTime = 0;
uint32_t uiDuration = 0;
bool isEndChar = false;
String currentMorse = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(KEYER_PIN, INPUT_PULLUP);
}

char morseToChar(String morseCode) {
  if (morseCode == ".-") return 'A';
  if (morseCode == "-...") return 'B';
  if (morseCode == "-.-.") return 'C';
  if (morseCode == "-..") return 'D';
  if (morseCode == ".") return 'E';
  if (morseCode == "..-.") return 'F';
  if (morseCode == "--.") return 'G';
  if (morseCode == "....") return 'H';
  if (morseCode == "..") return 'I';
  if (morseCode == ".---") return 'J';
  if (morseCode == "-.-") return 'K';
  if (morseCode == ".-..") return 'L';
  if (morseCode == "--") return 'M';
  if (morseCode == "-.") return 'N';
  if (morseCode == "---") return 'O';
  if (morseCode == ".--.") return 'P';
  if (morseCode == "--.-") return 'Q';
  if (morseCode == ".-.") return 'R';
  if (morseCode == "...") return 'S';
  if (morseCode == "-") return 'T';
  if (morseCode == "..-") return 'U';
  if (morseCode == "...-") return 'V';
  if (morseCode == ".--") return 'W';
  if (morseCode == "-..-") return 'X';
  if (morseCode == "-.--") return 'Y';
  if (morseCode == "--..") return 'Z';
  if(morseCode == ".----") return '1';
  if(morseCode == "..---") return '2';
  if(morseCode == "...--") return '3';
  if(morseCode == "....-") return '4';
  if(morseCode == ".....") return '5';
  if(morseCode == "-....") return '6';
  if(morseCode == "--...") return '7';
  if(morseCode == "---..") return '8';
  if(morseCode == "----.") return '0';
  if(morseCode == "-----") return '0';
  // Extend with digits and punctuation as needed
  return '?'; // Unknown character
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!digitalRead(KEYER_PIN))
  {
    tone(SPKR_PIN, SPKR_FREQ);
    if(uiBtnPrsdTime == 0)
    {
      uiBtnPrsdTime = millis();
      uiBtnRelTime = 0;
    }
  }
  else
  {
    noTone(SPKR_PIN);
    if(uiBtnPrsdTime != 0)
    {
      isEndChar = false;
      uiDuration = millis() - uiBtnPrsdTime;
      uiBtnRelTime = millis();
      uiBtnPrsdTime = 0;
      if(DOT_DUR <= uiDuration && uiDuration < DASH_DUR)
      {
        currentMorse += ".";
      }
      else if(DASH_DUR <= uiDuration)
      {
        currentMorse += "-";
      }
    }
    else if (uiBtnRelTime != 0 && (millis() - uiBtnRelTime >= WRD_SPC)) {
      // Check if the time since the button was released indicates a word space
      Serial.println(); // New line to separate words
      currentMorse = "";
      uiBtnRelTime = 0; // Reset to avoid multiple prints
    }
    else if(!isEndChar && uiBtnRelTime != 0 && (millis() - uiBtnRelTime >= CHR_SPC) && (millis() - uiBtnRelTime < WRD_SPC))
    {
      char letter = morseToChar(currentMorse);
      Serial.print(letter);
      currentMorse = "";
      isEndChar = true;
    }
  }
}
