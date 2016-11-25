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

volatile char magic[] = {
0 , 2, 4, 5, 7, 9,11,
12,14,16,17,19,21,23,
24,26,28,29,31,33,35
};
const unsigned int timerLoadValue = 220;//50;//220;
volatile unsigned char level = 0;
volatile unsigned int d[5] = {552<<3,173<<3,204<<3,346<<3,552<<3}; // default value( for debug)
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
  dn[0] = dn[0] + d[0] /*+ vf * ((*(wave[15] + (lfo >> 10))) << 2)*/;
  dn[1] = dn[1] + d[1];
  dn[2] = dn[2] + d[2];
  dn[3] = dn[3] + d[3];
  dn[4] = dn[4] + d[4];
  
  noise = (noise>>1) + ((bitRead(noise, 13) xor bitRead(noise, 3) xor 1) << 15);
  noise2= 2100005341  * noise2 + 1651869;

  level = ((dn[0]&(1<<14))?vol[0]:0) +
          ((dn[1]&(1<<14))?vol[1]:0) +
          ((dn[2]&(1<<14))?vol[2]:0) +
          ((dn[3]&(1<<14))?vol[3]:0) +
          ((dn[4]&(1<<14))?vol[4]:0)  + (noise & nf) + (noise2 & nf2);

  OCR1A = level;
}

void tones_init(){
  pinMode(9,OUTPUT);

  
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
  
  
  vol[0] = vol[1] = vol[2] = vol[3] = vol[4] = 0;
  d[0] = d[1] = d[2] = d[3] = d[4] = 0;
  
  // boot sound
  d[1] = getRawTone(0);
  vol[1] = 12;
  d[2] = getRawTone(2);
  vol[2] = 0;
  d[3] = getRawTone(4);
  vol[3] = 0;
  d[4] = getRawTone(0) >> 1;
  vol[4] = 0;  

}

