//******************* HEADER **************
/*
  Name : fm_radio
  Title : Fm-radio and speaker
  Description : STM32 STM32F103 "blue pill"  based FM radio using TEA5767 FM module, and Nokia 5110  LCD Display Module.
  Radio stations are selected via two push buttons
  and Audio Volume via a Potentiometer on the Audio AMplifer which audio out output of TEa5767 is connected
  A third push button enters a  settings mode.(TODO not in V1.0)
  Version: 1.0
  IDE: Arduino 1.8.10
  Author: Gavin Lyons
  URL: https://github.com/gavinlyonsrepo/STM32_projects
*/
// ********* LIBRARIES **********

#include <Wire.h>
#include <Button.h> // https://github.com/madleech/Button
#include <NOKIA5110_TEXT.h> // Authors libray at https://github.com/gavinlyonsrepo/NOKIA5110_TEXT
#include "./TEA5767custom.h" // Custom FM module library included. 

// ************ DEFINES & GLOBALS **************

unsigned long previousMillis = 0;     // will store last time LCD was updated
const long interval = 1000;           // interval at which to refresh display (milliseconds)

// LCD 
// LCD Nokia 5110 pinout left to right
// RST / CE / DC / DIN / CLK / VCC/ LIGHT / GND
#define RST PA0
#define CE PA1
#define DC PA2
#define DIN PA3
#define CLK PA4

#define inverse  false // set to true to invert display pixel color
#define contrast 0xB2 // default is 0xBF set in LCDinit, Try 0xB1 <-> 0xBF if your display is too dark
#define bias 0x13 // LCD bias mode 1:48: Try 0x12 or 0x13 or 0x14
#define SPLASH_DELAY 2500

 // NOTE NB Comment in fonts 1,2 and 7 defines at top of NOKIA5110_TEXT.h for this project 
#define DefaultFont 1
#define WideFont  2  
#define LargeFont  7  

NOKIA5110_TEXT mylcd(RST, CE, DC, DIN, CLK); // Create an LCD object

// Volume analog read from the amplifer control. 
#define VOLUME_PIN PA7

// Radio 
TEA5767 Radio; //Class object  for Fm radio module SCLK1 -PB6  SDA - PB7 
unsigned char buf[5];
uint8_t  search_mode = 0;
uint8_t  search_direction = 0; 
double current_freq;
uint8_t  signal_level;
#define FREQ_START 96.40
#define INIT_DELAY 500

// Push buttons 
#define btn_One_pin PB12
#define btn_Two_pin PB14
#define btn_Three_pin PB15 
#define ButtonDelay 700

Button btn_One(btn_One_pin);
Button btn_Two(btn_Two_pin);
Button btn_Three(btn_Three_pin);

// ********* Function Prototypes *************
void LCD_Init(void);
void SerailInit(void);
void RadioInit(void);
void GPIOinit(void);

void Volume_display(void);
void RadioInfo_display(void);
void ScanButtons(void);
void Drawtext(void);

// ***************** SETUP ****************
void setup() 
{
  SerailInit();
  LCD_Init();
  RadioInit();
  GPIOinit();
}

// ***************** MAIN LOOP ****************
void loop() 
{  
  RadioInfo_display();
  ScanButtons();
}
// ************ END of MAIN ***********


// ********** FUNCTIONS ************

// Init Serial comms
void SerailInit(void)
{
  Serial.begin(9600);
  delay(100);
  Serial.println("-- STM32 FM RADIO Comms up --");

}

void LCD_Init(void)
{
  mylcd.LCDInit(inverse, contrast, bias); // init  the lCD
  
  //Splash Screen
  mylcd.LCDFont(LargeFont); // Set the font
  mylcd.LCDFillBlock(0x5F, 0);
  mylcd.LCDgotoXY(0, 1); 
  mylcd.LCDString("FMRADIO");
  mylcd.LCDgotoXY(0, 3); 
  mylcd.LCDString(" STM32 ");
  mylcd.LCDFillBlock(0xF5 , 5);
  delay(SPLASH_DELAY); 
  mylcd.LCDClear(0x00);
  
  Drawtext(); 
}


void RadioInfo_display(void)
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) 
      {
      previousMillis = currentMillis;
      static char freqstr[13];
      uint8_t bars = 0;
      if (Radio.read_status(buf) == 1) 
      {
        current_freq =  floor (Radio.frequency_available (buf) / 100000 + .5) / 10;
        signal_level = Radio.signal_level(buf);
          
        // Freq
        mylcd.LCDFont(DefaultFont);
        mylcd.LCDgotoXY(0, 1);
        sprintf(freqstr, " %03.2f MHz", current_freq);
        mylcd.LCDString(freqstr);
        
        // Signal Strength
        mylcd.LCDClearBlock(3);
        mylcd.LCDgotoXY(0, 3);
        sprintf(freqstr, " %02u ", signal_level);
        mylcd.LCDString(freqstr);
        for (bars = 0; bars <15 ; bars++)
        {
            if (signal_level > bars)
              mylcd.LCDWrite(LCD_DATA, 0X7E);
        }
        
      }
        Volume_display();
  }
}

void Volume_display(void)
{
  uint8_t bars = 0;
  uint16_t VolumeVal = 0;  
  static char Volstring[10];
  VolumeVal  = analogRead(VOLUME_PIN);
  VolumeVal = map(VolumeVal, 0, 4095, 0, 99);
  mylcd.LCDFont(DefaultFont);
  sprintf(Volstring, " %02u ", VolumeVal);
  mylcd.LCDClearBlock(5);
  mylcd.LCDgotoXY(0, 5);
  mylcd.LCDString(Volstring);
  for (bars = 0; bars <20 ; bars++)
  {
      if ((VolumeVal/10)*2 > bars)
        mylcd.LCDWrite(LCD_DATA, 0X7E);
  }
}


void RadioInit(void)
{
  Wire.begin();
  delay(INIT_DELAY); //I2C init  delay
  Radio.init();
  Radio.set_frequency(FREQ_START); // On power on go to station 96.4
}

// Function called from main to scan the  push buttons
void ScanButtons()
{
  //When button pressed, search for new station
  if (search_mode == 1)
  {
    if (Radio.process_search (buf, search_direction) == 1)
    {
      search_mode = 0;
    }
  }

  //If forward button is pressed, go up to next station
  if (btn_One.pressed())
  {
    search_mode = 1;
    search_direction = TEA5767_SEARCH_DIR_UP;
    Radio.search_stations(buf, false);
    
    delay(ButtonDelay); //Pause let TEA5767 tune on Station before next press
  }
  
  //If backward button is pressed, go down to next station
  if (btn_Two.pressed())
  {
    search_mode = 1;
    search_direction = TEA5767_SEARCH_DIR_DOWN;
    Radio.search_stations(buf, true);
    delay(ButtonDelay); //Pause let TEA5767 tune on Station before next press
  }

  if (btn_Three.pressed())
  {
    DisplaySettings();
  }
  
}

void GPIOinit(void)
{
  digitalWrite(btn_One_pin, HIGH);
  digitalWrite(btn_Two_pin, HIGH);
  digitalWrite(btn_Three_pin, HIGH);
  btn_One.begin();
  btn_Two.begin();
  btn_Three.begin();
}

void Drawtext(void)
{
  mylcd.LCDFont(WideFont);
  mylcd.LCDgotoXY(0, 0);
  mylcd.LCDString("FREQUENCY");
  mylcd.LCDgotoXY(0, 2);
  mylcd.LCDString("SIGNAL");
  mylcd.LCDgotoXY(0, 4);
  mylcd.LCDString("VOLUME");
}

void DisplaySettings(void)
{
// TODO not implemented in version 1.0
}
  
