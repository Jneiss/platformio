//screen has 128X160

#include <TFT.h> 
#include <SPI.h>
#include "pics.h"


#define CS   10
#define DC   9
#define RST  8

#define VAL_COUNT 6
#define VAL_SIZE 1
#define VAL_TEXTOFF_X 10
#define VAL_TEXTOFF_Y 10
#define VAL_VALOFF_X 40

#define VAL_TEMP 1
#define VAL_MOIST1 2
#define VAL_MOIST2 3
#define VAL_DAYS 4
#define VAL_FAN 5

#define PUMP1_PIN 0
#define PUMP2_PIN 1

int fanfront_spd = 3; //0-99
int fanback_spd = 2;  //0-9

struct Value{
  char text[20];
  char val[5];
  int color_text[3] = {255,255,100};
  int color_val[3] = {255,255,0};
};


Value values[VAL_COUNT];

// create an instance of the library
TFT screen = TFT(CS, DC, RST);

// char array to print to the screen
char sensorPrintout[4];


void drawImage(int posx, int posy,const uint8_t map[],int scale = 1){
  int x = 0,y=0, len =map[0]*map[1]*3;

  for(int i=2;i<len;i+=3){
    screen.fillRect(posx+x,posy+y,scale,scale,screen.newColor(map[i],map[i+1],map[i+2]));

    x+=scale;
    if(x == map[0]*scale){
      x = 0;
      y+=scale;
    }
  }
}

void moisturize(int plant){

}

void readSensors(){
  return;
  strcpy(values[VAL_TEMP].val,"b");
}

void drawText(){
  screen.setTextSize(VAL_SIZE);

  for(int i=0;i<VAL_COUNT;i++){
    screen.stroke(values[i].color_text[0],values[i].color_text[1],values[i].color_text[2]);
    screen.text(values[i].text,VAL_TEXTOFF_X,i*VAL_SIZE*10+VAL_TEXTOFF_Y);
  }
}

void drawValues(){
  readSensors();

  screen.setTextSize(VAL_SIZE);

  for(int i=0;i<VAL_COUNT;i++){
    screen.stroke(values[i].color_val[0],values[i].color_val[1],values[i].color_val[2]);
    screen.text(values[i].val,VAL_TEXTOFF_X+VAL_VALOFF_X,i*VAL_SIZE*10+VAL_TEXTOFF_Y);
  }

  drawImage(100,30,plant_veg,3);
}

int t2_ctc_cnt = 0; 
int t2_ctc_cntf = 0;

ISR(TIMER2_COMPA_vect){
  t2_ctc_cnt++;
  t2_ctc_cntf++;

  if(t2_ctc_cntf > 100){
    t2_ctc_cntf = 0;
    PORTD |= 1 << PORTD7;
  }

  if(t2_ctc_cnt > 9){
    t2_ctc_cnt = 0;
    PORTB |= 1 << PORTB0;
  }
    

  if(t2_ctc_cnt == fanback_spd)
    PORTB &= ~(1<< PORTB0);


  if(t2_ctc_cntf == fanfront_spd)
    PORTD &= ~(1 << PORTD7);
}

void setup() {

  strcpy(values[VAL_TEMP].text,"temp:");
  strcpy(values[VAL_MOIST1].text,"moist1:");
  strcpy(values[VAL_MOIST2].text,"moist2:");
  strcpy(values[VAL_DAYS].text,"days veg:");
  strcpy(values[VAL_FAN].text,"fan speed:");

  // Put this line at the beginning of every sketch that uses the GLCD:
  screen.begin();

  // clear the screen with a black background
  screen.background(0, 0, 0);

  TCCR2A = 1<<WGM21;
  OCR2A = 2;//ocr for 200khz
  TCCR2B = 1 << CS21; //timer 2  freq 
  TIMSK2 = 1 << OCIE2A; //overflow interrupt enable

  screen.stroke(255, 255, 255);
  screen.setTextSize(2);
  screen.text("GB 2k5", 0, 0);

  drawText();
 
  DDRB |= 1 << PORTB0;
  DDRD |= 1 << PORTD7;

}

void loop() {
  
  drawValues();
  /*
  // Read the value of the sensor on A0
  String sensorVal = String(analogRead(A0));

  // convert the reading to a char array
  sensorVal.toCharArray(sensorPrintout, 4);

  // set the font color
  screen.stroke(255, 255, 255);
  // print the sensor value
  screen.text(sensorPrintout, 0, 20);
  // wait for a moment
  delay(250);
  // erase the text you just wrote
  screen.stroke(0, 0, 0);
  screen.text(sensorPrintout, 0, 20);*/
}
