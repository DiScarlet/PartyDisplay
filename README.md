# Party Display

---

## What is this project?

**Party Display** is a hardware-based music project built using an ESP32 and a 1.8" TFT screen.

## What does it do?

It connects to the Spotify API and allows you to control the music you're currently listening to.   

## Why does it exist?

This project gives you a **dedicated physical device** to interact with your music.

I created it as a way to kick off my journey into hardware development.  
Spotify now actually *feels physical* — using real buttons is far more engaging than tapping digital ones on a screen.

---

## 🔧 Components

### ESP32 Microcontroller  
Handles backend logic such as Wi-Fi communication and Spotify API integration.  

![ESP32](Images/image-2.png)

---

### 1.8" TFT Display  
Displays song information, themes, and a progress bar.  

![TFT](Images/image-3.png)

---

### Dupont Cables (Female-to-Female)  
Used to connect all components together (ESP32 and TFT both use male pins).  
![DuPont Connections](Images/image-9.png)

---

### Buttons (x3)  
Used for playback control:
- Next track  
- Previous track  
- Play / Pause  

![Tactile Button Switch](Images/image-1.png)

---

## Wiring

![Wiring](Images/image-4.png)

---

## CAD Design

![CAD1](Images/image-5.png)  
![CAD2](Images/image-6.png)  
![CAD3](Images/image-7.png)  
![CAD4](Images/image-8.png)

---

## Future Improvements

- I am planning to add volume control  
- Display album artwork (optional)  
- Improve case design