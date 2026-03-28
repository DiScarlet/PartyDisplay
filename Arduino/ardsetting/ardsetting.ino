#include <Arduio.h>
#include <ArduioJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <WiFi.h>
#include <SpotifyEsp32.h>
#include <SPI.h>

//Vars
//consts
const char* CLIENT_ID = "CLIENT_ID";
const char* CLIENT_SECRET = "CLIENT_SECRET";
const char* SSID = "later"
const char* PASSWORD = "not now"
//vars
String lastArtist;
String lastTrackname;
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
  Serial.print("Connecting to WiFi...") //Print to console
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".") //A loading dot
  }

  Serial.printf("\nConnected!\n"); //WiFi connected


  //Preparation/additional setup
  tft.setCursor(0,0); //Cursor -> top left
  tft.write(WiFi.localIP().toString().c_str()) //Print local IP

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

  if (lastArtist != currentArtist && currentArtist != "Something went wrong" && !currentArtist.isEmpty()){
    tft.fillScreen(ST77XX_BLACK);
    lastArtist = currentArtist;
    Serial.println("Artist:" + lastArtist);
    tft.setCursor(10,10);
    tft.write(lastArtisst.c_str());
  }

  if(lastArtist != currentArtist && currentArtist != "Something went wrong" && currentTrackname != "null"){
    lastTrackname = currentTrackname;
    Serial.println("Track:" + lastTrackname);
    tft.setCursor(10,40);
    tft.write(lastTrackname.c_str());
  }
  delay(2000);
}
