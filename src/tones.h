
#include <avr/pgmspace.h>

#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988

volatile unsigned int tones[]= {
NOTE_C3,NOTE_CS3,
NOTE_D3,NOTE_DS3,
NOTE_E3,
NOTE_F3,NOTE_FS3,
NOTE_G3,NOTE_GS3,
NOTE_A3,NOTE_AS3,
NOTE_B3,
NOTE_C4,NOTE_CS4,
NOTE_D4,NOTE_DS4,
NOTE_E4,
NOTE_F4,NOTE_FS4,
NOTE_G4,NOTE_GS4,
NOTE_A4,NOTE_AS4,
NOTE_B4,
NOTE_C5,NOTE_CS5,
NOTE_D5,NOTE_DS5,
NOTE_E5,
NOTE_F5,NOTE_FS5,
NOTE_G5,NOTE_GS5,
NOTE_A5,NOTE_AS5,
NOTE_B5
};

PROGMEM const char sinTable[512] = {
0,1,3,4,6,7,9,10,12,13,15,17,18,20,21,23,24,26,27,29,30,32,33,35,36,38,39,41,42,44,45,47,48,50,51,52,54,55,57,58,59,61,62,63,65,66,67,69,70,71,73,74,75,76,78,79,80,81,82,84,85,86,87,88,89,90,91,93,94,95,96,97,98,99,100,101,102,102,103,104,105,106,107,108,108,109,110,111,112,112,113,114,114,115,116,116,117,117,118,119,119,120,120,121,121,121,122,122,123,123,123,124,124,124,125,125,125,125,126,126,126,126,126,126,126,126,126,126,127,126,126,126,126,126,126,126,126,126,126,125,125,125,125,124,124,124,123,123,123,122,122,121,121,121,120,120,119,119,118,117,117,116,116,115,114,114,113,112,112,111,110,109,108,108,107,106,105,104,103,102,102,101,100,99,98,97,96,95,94,93,91,90,89,88,87,86,85,84,82,81,80,79,78,76,75,74,73,71,70,69,67,66,65,63,62,61,59,58,57,55,54,52,51,50,48,47,45,44,42,41,39,38,36,35,33,32,30,29,27,26,24,23,21,20,18,17,15,13,12,10,9,7,6,4,3,1,0,-1,-3,-4,-6,-7,-9,-10,-12,-13,-15,-17,-18,-20,-21,-23,-24,-26,-27,-29,-30,-32,-33,-35,-36,-38,-39,-41,-42,-44,-45,-47,-48,-50,-51,-52,-54,-55,-57,-58,-59,-61,-62,-63,-65,-66,-67,-69,-70,-71,-73,-74,-75,-76,-78,-79,-80,-81,-82,-84,-85,-86,-87,-88,-89,-90,-91,-93,-94,-95,-96,-97,-98,-99,-100,-101,-102,-102,-103,-104,-105,-106,-107,-108,-108,-109,-110,-111,-112,-112,-113,-114,-114,-115,-116,-116,-117,-117,-118,-119,-119,-120,-120,-121,-121,-121,-122,-122,-123,-123,-123,-124,-124,-124,-125,-125,-125,-125,-126,-126,-126,-126,-126,-126,-126,-126,-126,-126,-127,-126,-126,-126,-126,-126,-126,-126,-126,-126,-126,-125,-125,-125,-125,-124,-124,-124,-123,-123,-123,-122,-122,-121,-121,-121,-120,-120,-119,-119,-118,-117,-117,-116,-116,-115,-114,-114,-113,-112,-112,-111,-110,-109,-108,-108,-107,-106,-105,-104,-103,-102,-102,-101,-100,-99,-98,-97,-96,-95,-94,-93,-91,-90,-89,-88,-87,-86,-85,-84,-82,-81,-80,-79,-78,-76,-75,-74,-73,-71,-70,-69,-67,-66,-65,-63,-62,-61,-59,-58,-57,-55,-54,-52,-51,-50,-48,-47,-45,-44,-42,-41,-39,-38,-36,-35,-33,-32,-30,-29,-27,-26,-24,-23,-21,-20,-18,-17,-15,-13,-12,-10,-9,-7,-6,-4,-3,-1
};




volatile char magic[] = {
0 , 2, 4, 5, 7, 9,11,
12,14,16,17,19,21,23,
24,26,28,29,31,33,35
};
const unsigned int timerLoadValue = 220;//50;//220;
volatile unsigned char level = 0;
volatile unsigned int d[5] = {552<<3,173<<3,0,0,0}; // default value( for debug)
volatile unsigned int dn[5] = {0,0,0,0,0};       // work variable

volatile unsigned char vol[5] = {0,0,0,0,0}; // volume par channel
char octave = 1;
int shiftTune = 0;
char shiftTone = 12;
char shiftMagic = 0;
int noise = 50;
volatile byte nf = 0;
int noise2 = 50;
volatile byte nf2 = 0;

inline int getRawTone(byte no){
  return  (tones[no + shiftTone] + shiftTune) << (octave);
}

ISR(TIMER1_OVF_vect) {    // Timer/Counter1 Overflow
  TCNT2 = timerLoadValue; // Reset the timer

  //volatile byte tmp2 = ((dn[2]&(1<<15))?vol[2]:0);
  //volatile byte tmp3 = ((dn[3]&(1<<15))?vol[3]:0);

  //dn[4] = dn[4] + d[4];

/*
  // pattern 4->3->2->1 
  volatile byte tmp4 = ((dn[4]&(1<<14))?vol[4]:0);
  dn[4] = dn[4] + d[4]+ tmp4; // feedback


  dn[3] = dn[3] + d[3] + (tmp4 << 4);
  volatile byte tmp3 = (((dn[3]&(1<<14)) == (1<<14))?vol[3]:0);

  dn[2] = dn[2] + d[2] + (tmp3 << 4);
  volatile byte tmp2 = ((dn[2]&(1<<14))?vol[2]:0);

  dn[1] = dn[1] + d[1] + (tmp2 << 4);
  volatile byte tmp1 = ((dn[1]&(1<<14))?vol[1]:0);
  
  //tmp1 -> out
*/

/*
  // pattern 4 -> 3,2 -> 1
  dn[4] = dn[4] + d[4];
  volatile byte tmp4 = ((dn[4]&(1<<14))?vol[4]:0);


  dn[3] = dn[3] + d[3] + (tmp4 << 5);
  volatile byte tmp3 = (((dn[3]&(1<<14)) == (1<<14))?vol[3]:0);

  dn[2] = dn[2] + d[2] + (tmp4 << 5);
  volatile byte tmp2 = ((dn[2]&(1<<14))?vol[2]:0);

  dn[1] = dn[1] + d[1] + (tmp3 << 4) + (tmp2 << 4);
  volatile byte tmp1 = ((dn[1]&(1<<14))?vol[1]:0);
  
  // tmp1 -> out
*/

/*
  // pattern 4 -> 1
  dn[4] = dn[4] + d[4];
  volatile byte tmp4 = ((dn[4]&(1<<15))?vol[4]:0);

  dn[1] = dn[1] + d[1] + (tmp4 << 4);
  volatile byte tmp1 = ((dn[1]&(1<<14))?vol[1]:0);
  */



  // pattern 1 + LFO

  dn[2] = dn[2] + d[2];
  volatile byte tmp2 = ((dn[2]&(1<<14))?vol[2]:0);


  dn[1] = dn[1] + d[1] + tmp2;
  volatile byte tmp1 = ((dn[1]&(1<<14))?vol[1]:0);
   // tmp1 -> out


  // detune
  dn[0] = dn[0] + d[0];
  volatile byte tmp0 = ((dn[0]&(1<<14))?vol[0]:0);

  //base 
  dn[3] = dn[3] + d[3];
  volatile byte tmp3 = ((dn[3]&(1<<14))?vol[3]:0);



  
  noise = (noise>>1) + ((bitRead(noise, 13) xor bitRead(noise, 3) xor 1) << 15);
  noise2= 2100005341  * noise2 + 1651869;

/*
  level = ((dn[0]&(1<<14))?vol[0]:0) +
          ((dn[1]&(1<<14))?vol[1]:0) +
          ((dn[2]&(1<<14))?vol[2]:0) +
          ((dn[3]&(1<<14))?vol[3]:0) +
          ((dn[4]&(1<<14))?vol[4]:0)  + (noise & nf) + (noise2 & nf2);
*/

  //volatile byte tmp = ((dn[0]&(1<<14))?vol[0]:0);
  //volatile byte tmp = ((pgm_read_byte_near(&sinTable[dn[0] >> 7]) * vol[0])>>8);
  level = tmp0 + tmp1 + tmp3 + (noise & nf) + (noise2 & nf2);


  OCR1A = level;
}

void tones_init(){
  pinMode(9,OUTPUT);
  pinMode(6,INPUT);

  
  // 3-pwm ,pwm
  TCCR1A = _BV(COM1A1)| _BV(WGM10);
  // clk/1 prescale
  TCCR1B = _BV(CS20); //1      31250
  //TCCR2B = _BV(CS21); //8     3906.25
  //1<<CS22 | 0<<CS21 | 1<<CS20; // clk/128 prescale
  //TCCR2B = TCCR2B & 0b11100000 | 0b00001; // pwm: 31250Hz
  TIMSK1 = 1<<TOIE1; // Timer/Counter2 Overflow Interrupt Enable
  OCR1A = 80; // for debug
  TCNT1 = timerLoadValue;
  
  //TCCR0A = 0xC1;
  //TCCR1B = _BV(CS20);
  
  vol[0] = vol[1] = vol[2] = vol[3] = vol[4] = 0;
  d[0] = d[1] = d[2] = d[3] = d[4] = 0;
  
  // boot sound
  //d[1] = getRawTone(0);
  //vol[1] = 12;

}

