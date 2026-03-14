#include <Arduino.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h> 
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Adafruit_ST7735.h>
#include <WiFi.h>
#include <SpotifyEsp32.h>
#include <SPI.h>


#define TFT_CS 1
#define TFT_RST 2
#define TFT_DC 3
#define TFT_SCLK 4
#define TFT_MOSI 5


sp.current_artist_names(); 
sp.current_track_name();   
sp.start_resume_playback();
sp.skip();                  
sp.previous();             
sp.is_playing();   


char* SSID = "left blank for privacy reasons";
char* PASSWORD = "";
const char* CLIENT_ID = "";
const char* CLIENT_SECRET = "";


String lastArtist;
String lastTrackname;
String play/pause;
String skipsong;
String gotoprevious;
String playing;
String currentArtist;
String currentTrackname;

const int kbswitch1 = 1; 
const int kbswitch2 = 2; 
const int kbswitch3 = 3; 
int buzzer = 4;


unsigned long lastpressed = 0;

Spotify sp(CLIENT_ID, CLIENT_SECRET);
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);


void setup() {

    Serial.begin(115200);
    tft.setFont(&FreeMonoBoldOblique12pt7b);

    pinMode(kbswtich1, INPUT_PULLUP);
    pinMode(kbswtich2, INPUT_PULLUP);
    pinMode(kbswtich3, INPUT_PULLUP);
    pinMode(buzzer, OUTPUT);

    tft.initR(INITR_BLACKTAB);
    tft.setRotation(1); 
    Serial.println("TFT Initialized!");
    tft.fillScreen(ST77XX_BLACK); 

    WiFi.begin(SSID, PASSWORD);
    Serial.print("Connecting to WiFi...");
    while(WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }
    Serial.printf("\nConnected!\n");

    tft.setCursor(0,0); 

    sp.begin();
    while(!sp.is_auth()){
        sp.handle_client();
    }
    Serial.println("Authenticated");
}

void loop()
{
   
    lastArtist = sp.current_artist_names(); 
    lastTrackname = sp.current_track_name();  
    play/pause = sp.start_resume_playback(); 
    skipsong = sp.skip();                 
    gotoprevious = sp.previous();            
    playing = sp.is_playing(); 
    currentArtist = sp.current_artist_names();
    currentTrackname = sp.current_track_name();
    
    if(millis() - lastpressed >= 300){

        if(digitalRead(kbswitch1) == LOW){
            sp.previous();
            lastpressed = millis();
            tone(buzzer, 1000);
            delay(150);
            noTone(buzzer);}

        else if(digitalRead(kbswitch2) == LOW){
            sp.start_resume_playback();
            lastpressed = millis();
            tone(buzzer, 1500);
            delay(100);
            noTone(buzzer);
            delay(100);
            tone(buzzer, 1500);
            delay(100);
            noTone(buzzer);}

        else if(digitalRead(kbswitch3) == LOW){
            sp.skip();
            lastpressed = millis();
            tone(buzzer, 800);
            delay(100);
            tone(buzzer, 1200);
            delay(100);
            tone(buzzer, 1600);
            delay(100);
            noTone(buzzer);}
    
    }
}
