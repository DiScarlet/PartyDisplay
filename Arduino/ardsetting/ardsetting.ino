#include <Arduino.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <WiFi.h>
#include <SpotifyEsp32.h>
#include <SPI.h>

//STRUCTS
struct Theme
{
  const char* keyword;
  uint16_t color;
};

//Struct objects
Theme themes[] = {
  {"drake", ST77XX_MAGENTA},
  {"travis", ST77XX_MAGENTA},
  {"metal", ST77XX_RED},
  {"rock", ST77XX_RED},
  {"pop", ST77XX_CYAN},
  {"chill", ST77XX_BLUE},
  {"lofi", ST77XX_BLUE}
};


//VARS
//consts
const int LOOP_DELAY_MS = 1000;
const char* CLIENT_ID = "CLIENT_ID";
const char* CLIENT_SECRET = "CLIENT_SECRET";
const char* SSID = "later";
const char* PASSWORD = "not now";
  //themes
const uint8_t themeCount = sizeof(themes)/sizeof(themes[0]);
//vars
String lastArtist;
String lastTrackname;
  //progress bar
int progressMs = 0;
int durationMs = 1; // mi val of 1 to avoid division by 0 error
int lastProgressWidth = -1;
  //background color
uint16_t currentThemeColor = ST77XX_GREEN;
//def
#define TFT_CS 1
#define TFT_RST 2
#define TFT_DC 3
#define TFT_SCLK 4
#define TFT_MOSI 5
  //buttons
#define BTN_PLAY_PAUSE 11
#define BTN_NEXT 10
#define BTN_PREV 9
//objs
Spotify sp(CLIENT_ID, CLIENT_SECRET);
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
//state vars
  //bools
bool lastPlayState = HIGH;
bool lastNextState = HIGH;
bool lastPrevState = HIGH;
  //long, int, float
unsigned long lastDebounceTime = 0;
const int debounceDelay  = 200;


//FUNCS
void setup() {
  Serial.begin(115200);

    //Setup the buttons'=
  pinMode(BTN_PLAY_PAUSE, INPUT_PULLUP);
  pinMode(BTN_NEXT, INPUT_PULLUP);
  pinMode(BTN_PREV, INPUT_PULLUP);

    //Initiate the screen
  tft.initR(INITR_BLACKTAB); //type of screen
  Serial.println("TFT Initialized!");
  tft.fillRect(0, 0, 128, 60, ST77XX_BLACK);


  //WiFi setup
  WiFi.begin(SSID, PASSWORD); //Connect to WiFi
  Serial.print("Connecting to WiFi..."); //Print to console
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print("."); //A loading dot
  }

  Serial.printf("\nConnected!\n"); //WiFi connected


  //Preparation/additional setup
  tft.setCursor(0,0); //Cursor -> top left
  tft.write(WiFi.localIP().toString().c_str()); //Print local IP

  //Spotify connection setup
  sp.begin();
  while(!sp.is_auth())
  {
    sp.handle_client();
  }
  Serial.println("Connected to Spotify!");
}

void loop() {
  String currentArtist = sp.current_artist_names();
  String currentTrackname = sp.current_track_name();
    //progress bar vars
  progressMs = sp.current_progress_ms();
  durationMs = sp.current_track_duration_ms();

  if (lastArtist != currentArtist && currentArtist != "Something went wrong" && !currentArtist.isEmpty()){
    tft.fillScreen(ST77XX_BLACK);
    lastArtist = currentArtist;

    updateTheme(lastArtist);
    Serial.println("Artist:" + lastArtist);

    tft.setTextColor(currentThemeColor);
    tft.setCursor(10,10);
    tft.print(lastArtist);
  }

  if(lastTrackname != currentTrackname && currentArtist != "Something went wrong" && currentTrackname != "null"){
    lastTrackname = currentTrackname;
    Serial.println("Track:" + lastTrackname);
    tft.setCursor(10,40);
    tft.print(lastTrackname);
  }


  //draw progress bar
  if(durationMs > 0)
  {
    drawProgressBar(progressMs, durationMs);
  }

  delay(LOOP_DELAY_MS);
}


//helper functions
void drawProgressBar(int progress, int duration)
{
  int barX = 10;
  int barY = 70;
  int barWidth = 100;
  int barHeight = 8;

  float percent = (float)progress / duration;
  int currentWidth = percent * barWidth;

  if(currentWidth != lastProgressWidth)
  {
    tft.fillRect(barX, barY, barWidth, barHeight, ST77XX_BLACK);
    tft.drawRect(barX, barY, barWidth, barHeight, ST77XX_WHITE);
    tft.fillRect(barX, barY, currentWidth, barHeight, currentThemeColor);

    lastProgressWidth = currentWidth;
  }
}

void updateTheme(String artist)
{
  artist.toLowerCase();
  currentThemeColor = ST77XX_GREEN;

  for(uint8_t i = 0; i < themeCount; i++)
  {
    if(artist.indexOf(themes[i].keyword) >= 0)
    {
      currentThemeColor = themes[i].color;
      break;
    }
  }
}

void handleButtons()
{
  if(millis() - lastDebounceTime < debounceDelay)
    return;

  bool playState = digitalRead(BTN_PLAY_PAUSE);
  bool nextState = digitalRead(BTN_NEXT);
  bool prevState = digitalRead(BTN_PREV);

  //Play/Pause pressed
  if(playState == LOW && lastPlayState == HIGH)
  {
    Serial.println("Play/Pause pressed");
    sp.toggle_play_pause();
    lastDebounceTime = millis();
  }


  //Next
  if (nextState == LOW lastNextState == HIGH)
  {
    Serial.println("Next pressed");
    sp.next_track();
    lastDebounceTime = millis();
  }

   //Prev
  if (prevState == LOW lastPrevState == HIGH)
  {
    Serial.println("Prev pressed");
    sp.previous_track();
    lastDebounceTime = millis();
  }


  lastPlayState = playState;
  lastNextState = nextState;
  lastPrevState = prevState;
}