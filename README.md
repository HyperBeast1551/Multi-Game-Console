# 🎮 Multi-Game Console (Arduino-Based)

This is a **multi-game console** powered by an Arduino Uno, featuring multiple classic games in a single sketch! It uses **4 push buttons** for game control, player input, and mode selection. Designed for **single or dual player** gameplay.

> 🚀 Built as part of **Hack Club’s Summer of Making**

---

## 🧠 Features

- 🎲 **Three built-in games** (customizable)
- 👨‍👩‍👧‍👦 **Single-player and Dual-player** modes
- 🔄 **Button-based game selector**
- 📟 LCD-based status display (I2C 16x2)
- ⚙️ Human-friendly game loop structure
- 💡 Expandable to add more games or sensors

---

## 🛠️ Hardware Used

| Component                | Quantity | Purpose                          |
|-------------------------|----------|----------------------------------|
| Arduino Uno R3          | 1        | Main controller                  |
| 4x Push Buttons         | 4        | Input for game control           |
| 10kΩ Resistors          | 4        | Pull-downs for buttons           |
| I2C 16x2 LCD Display     | 1        | Game and mode status display     |
| Breadboard + Jumper Wires | —      | Prototyping                      |
| (Optional) Buzzer       | 1        | Game sound feedback              |

---

## 🕹️ Controls

| Button | Function                            |
|--------|-------------------------------------|
| BTN1   | Change current game                 |
| BTN2   | Toggle between Single/Dual player   |
| BTN3   | Player 1 action                     |
| BTN4   | Player 2 action (if in dual mode)   |

---

## 🪢 Circuit Wiring

| Components |              Pins                 |
|----------|-------------------------------------|
| BTN1     | 2                                   |
| BTN2     | 3                                   |
| BTN3     | 4                                   |
| BTN4     | 5                                   |
| lcd(SDA) | A4                                  |
| lcd(SCL) | A5                                  | 

---

## 🚦 Games Currently Included

1. **Reaction Time Game**  
   Measure how fast you can react to an LED turning on.

2. **Button Mash Challenge**  
   Compete who can press faster in 10 seconds!

3. **Memory Tap Game**  
   Remember and repeat the button sequence (like Simon Says).

---

## 🧪 How to Use

1. **Upload `multi_game_console.ino` to your Arduino Uno** using the Arduino IDE or VS Code with PlatformIO.
2. **Connect buttons and LCD** as per the pin mappings in code.
3. **Power up** — LCD shows the current game and mode.
4. **Play!** Switch games and modes using BTN1 and BTN2.

---

## ⚡ Future Improvements

- Add buzzer for sound effects
- Store high scores in EEPROM
- Add joystick or rotary encoder for richer controls
- Game selection via menu interface
- OLED or Nokia 5110 LCD graphics upgrade

---

## 🤝 Contributions

Feel free to fork this repo, add more games, or improve the UI. Open a PR if you'd like to contribute!

---

## 📜 License

This project is open-source under the [MIT License](LICENSE).

---

> Designed with ❤️ by [@HyperBeast1551](https://github.com/HyperBeast1551)  
> Made during Hack Club’s [Summer of Making](https://hackclub.com)
