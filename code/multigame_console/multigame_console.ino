#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

// ---------------- CONFIG ----------------
// Button mapping
#define BTN_GAME 2      // Cycle through games
#define BTN_SELECT 3    // Start selected game
#define BTN_MODE 4      // Toggle single/dual mode
#define BTN_P1 5        // Player 1 action
#define BTN_P2 6        // Player 2 action

#define BUZZER 7        // Optional buzzer pin

#define NUM_GAMES 3

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------------- VARIABLES ----------------
byte currentGame = 0;
bool dualMode = false;
bool gameRunning = false;

unsigned long lastDebounceTime[5] = {0};
bool lastButtonState[5] = {HIGH, HIGH, HIGH, HIGH, HIGH};
bool buttonState[5] = {HIGH, HIGH, HIGH, HIGH, HIGH};
unsigned long debounceDelay = 50;

// Game-specific variables
unsigned long gameStartTime;
unsigned long reactionStart;
int p1Score, p2Score;
int memorySeq[10], seqLength, seqPos;

// EEPROM addresses
#define EEPROM_ADDR_P1 0
#define EEPROM_ADDR_P2 2

// ---------------- BUTTON READ ----------------
bool readButton(int btn, int index) {
  bool reading = digitalRead(btn);
  if (reading != lastButtonState[index]) {
    lastDebounceTime[index] = millis();
  }
  if ((millis() - lastDebounceTime[index]) > debounceDelay) {
    if (reading != buttonState[index]) {
      buttonState[index] = reading;
      if (buttonState[index] == LOW) return true; // Press detected
    }
  }
  lastButtonState[index] = reading;
  return false;
}

// ---------------- BUZZER ----------------
void beep(int freq, int dur) {
  tone(BUZZER, freq, dur);
  delay(dur);
  noTone(BUZZER);
}

// ---------------- LCD DISPLAY ----------------
void showStatus() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Game: ");
  if (currentGame == 0) lcd.print("Reaction");
  else if (currentGame == 1) lcd.print("ButtonMash");
  else if (currentGame == 2) lcd.print("MemoryTap");

  lcd.setCursor(0, 1);
  lcd.print(dualMode ? "Dual" : "Single");
  lcd.print(" Mode");
}

// ---------------- GAME 1: Reaction Time ----------------
void gameReaction() {
  unsigned long p1Time = 0, p2Time = 0;
  unsigned long aiDelay = 0;
  bool p1Done = false, p2Done = false;

  lcd.clear();
  lcd.print("Wait for signal");
  delay(random(2000, 5000)); // Random wait before signal

  lcd.clear();
  lcd.print("GO!");
  beep(1000, 200);
  reactionStart = millis();

  // If single mode, set AI delay
  if (!dualMode) {
    aiDelay = random(300, 900); // AI reacts 300-900 ms after GO!
  }

  while (true) {
    // Player 1 reaction
    if (!p1Done && readButton(BTN_P1, 3)) {
      p1Time = millis() - reactionStart;
      p1Done = true;
    }

    // Player 2 reaction
    if (dualMode) {
      if (!p2Done && readButton(BTN_P2, 4)) {
        p2Time = millis() - reactionStart;
        p2Done = true;
      }
    } else {
      // AI reaction in single mode
      if (!p2Done && millis() - reactionStart >= aiDelay) {
        p2Time = aiDelay;
        p2Done = true;
      }
    }

    // End condition
    if (p1Done && p2Done) break;
  }

  // Display results
  lcd.clear();
  lcd.print("P1: ");
  lcd.print(p1Time);
  lcd.print("ms");

  lcd.setCursor(0, 1);
  lcd.print("P2: ");
  lcd.print(p2Time);
  lcd.print("ms");
  delay(2000);

  // Show winner
  lcd.clear();
  if (p1Time < p2Time) lcd.print("P1 Wins!");
  else if (p2Time < p1Time) lcd.print("P2 Wins!");
  else lcd.print("Tie!");
  delay(2000);

  // Save best time for P1
  int best = (EEPROM.read(EEPROM_ADDR_P1) << 8) | EEPROM.read(EEPROM_ADDR_P1+1);
  if (p1Time < best || best == 0) {
    EEPROM.write(EEPROM_ADDR_P1, p1Time >> 8);
    EEPROM.write(EEPROM_ADDR_P1+1, p1Time & 0xFF);
  }
}


// ---------------- GAME 2: Button Mash ----------------
void gameButtonMash() {
  p1Score = 0;
  p2Score = 0;
  lcd.clear();
  lcd.print("Mash!");
  delay(1000);
  gameStartTime = millis();

  while (millis() - gameStartTime < 10000) {
    if (readButton(BTN_P1, 3)) p1Score++;
    if (dualMode && readButton(BTN_P2, 4)) p2Score++;
  }

  lcd.clear();
  lcd.print("P1:");
  lcd.print(p1Score);
  if (dualMode) {
    lcd.print(" P2:");
    lcd.print(p2Score);
  }
  delay(3000);
}

// ---------------- GAME 3: Memory Tap ----------------
void gameMemoryTap() {
  seqLength = 1;
  seqPos = 0;
  for (int i = 0; i < 10; i++) memorySeq[i] = random(0, dualMode ? 2 : 1); 

  while (true) {
    lcd.clear();
    lcd.print("Watch...");
    delay(1000);

    for (int i = 0; i < seqLength; i++) {
      lcd.clear();
      if (memorySeq[i] == 0) { 
        lcd.print("P1");
        beep(800, 300);
      } 
      else {
        lcd.print("P2");
        beep(1000, 300);
      }
      delay(500);
    }

    lcd.clear();
    lcd.print("Repeat!");
    seqPos = 0;

    while (seqPos < seqLength) {
      if (readButton(BTN_P1, 3) && memorySeq[seqPos] == 0) seqPos++;
      else if (dualMode && readButton(BTN_P2, 4) && memorySeq[seqPos] == 1) seqPos++;
      else if (readButton(BTN_P1, 3) || (dualMode && readButton(BTN_P2, 4))) {
        lcd.clear();
        lcd.print("Wrong!");
        delay(2000);
        return;
      }
    }

    seqLength++;
    if (seqLength > 10) return;
  }
}

// ---------------- SETUP ----------------
void setup() {
  pinMode(BTN_GAME, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_MODE, INPUT_PULLUP);
  pinMode(BTN_P1, INPUT_PULLUP);
  pinMode(BTN_P2, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);

  lcd.init();
  lcd.backlight();
  randomSeed(analogRead(0));

  showStatus();
}

// ---------------- LOOP ----------------
void loop() {
  // Game selection mode
  if (!gameRunning) {
    if (readButton(BTN_GAME, 0)) { 
      currentGame = (currentGame + 1) % NUM_GAMES;
      showStatus();
    }
    if (readButton(BTN_MODE, 2)) { 
      dualMode = !dualMode;
      showStatus();
    }
    if (readButton(BTN_SELECT, 1)) { 
      gameRunning = true;
      if (currentGame == 0) gameReaction();
      else if (currentGame == 1) gameButtonMash();
      else if (currentGame == 2) gameMemoryTap();
      gameRunning = false;
      showStatus();
    }
  }
}
