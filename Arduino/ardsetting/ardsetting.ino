#include <Arduino.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <WiFi.h>
#include <SpotifyEsp32.h>
#include <SPI.h>

//Vars
//consts
const char* CLIENT_ID = "CLIENT_ID";
const char* CLIENT_SECRET = "CLIENT_SECRET";
const char* SSID = "later";
const char* PASSWORD = "not now";
//vars
String lastArtist;
String lastTrackname;
  //progress bar
int progressMs = 0;
int durationMs = 1; // mi val of 1 to avoid division by 0 error
int lastProgressWidth = -1;
//def
#define TFT_CS 1
#define TFT_RST 2
#define TFT_DC 3
#define TFT_SCLK 4
#define TFT_MOSI 5
//objs
Spotify sp(CLIENT_ID, CLIENT_SECRET);
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST)


void setup() {
  Serial.begin(115200);

    //Initiate the screen
  tft.initR(INITR_BLACKTAB); //type of screen
  Serial.println("TFT Initialized!");
  tft.fillScreen(ST77XX_BLACK);


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
    Serial.println("Artist:" + lastArtist);
    tft.setCursor(10,10);
    tft.write(lastArtist.c_str());
  }

  if(lastTrackname != currentTrackname && currentArtist != "Something went wrong" && currentTrackname != "null"){
    lastTrackname = currentTrackname;
    Serial.println("Track:" + lastTrackname);
    tft.setCursor(10,40);
    tft.write(lastTrackname.c_str());
  }


  //draw progress bar
  if(durationMs > 0)
  {
    DrawProgressBar(progressMs, durationMs);
  }

  delay(2000);
}


//helper functions
void DrawProgressBar(int progress, int duration)
{
  int barX = 10;
  int barY = 70;
  int barWidth = 100;
  int barHeight = 8;

  float percet = (float)progress / duration;
  int currentWidth = percent * barX;

  if(currentWidth != lastProgressWidth)
  {
    tft.fillRect(barX, barY, barWidth, barHeight, ST77XX_BLACK);
    tft.DrawRect(barX, barY, barWidth, barHeight, ST77XX_WHITE);
    tft.fillRect(barX, barY, barWidth, barHeight, ST77XX_GREEN);

    lastProgressWidth = currentWidth;
  }
}