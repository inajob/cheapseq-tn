#include <tones.h>
#include <encoder.h>
#include <lcd.h>
#include <keys.h>


byte rythm[8] = {0,0,0,0,0,0,0,0};
byte melody[4][8] = {
{255,255,255,255,255,255,255,255},
{255,255,255,255,255,255,255,255},
{255,255,255,255,255,255,255,255},
{255,255,255,255,255,255,255,255}
};

#define M_RYTHM 0
#define M_TONE 1
byte mode = M_RYTHM;
byte cursor = 0;
byte track = 0;

void setup(){
  tones_init();
  encoder_init();
  lcd_init();
  keys_init();

  Serial.begin(9600);
}

void loop(){
  switch(mode){
  case M_RYTHM:
    rythmApp();
    break;
  case M_TONE:
    rythmApp();
    break;
  }
  keys_scan();

  //Serial.println(ecount);
}


void key(byte n){
  switch(mode){
  case M_RYTHM:
    switch(n){
      case 0: rythm[0] =  (rythm[0] + 1) & 3; break;
      case 2: rythm[1] =  (rythm[1] + 1) & 3; break;
      case 4: rythm[2] =  (rythm[2] + 1) & 3; break;
      case 5: rythm[3] =  (rythm[3] + 1) & 3; break;
      case 7: rythm[4] =  (rythm[4] + 1) & 3; break;
      case 9: rythm[5] =  (rythm[5] + 1) & 3; break;
      case 11: rythm[6] = (rythm[6] + 1) & 3; break;
      case 12: rythm[7] = (rythm[7] + 1) & 3; break;
      case 21: mode = M_TONE; break;
    }
    break;
  case M_TONE:
    if(n <= 12){
      d[1] = getRawTone(n);
      vol[1] = 12;
      melody[track][cursor] = n;
    }else{
      switch(n){
        case 22: melody[track][cursor] = 255; break;
        case 20: mode = M_RYTHM; break;
        case 25: cursor = (cursor + 1)&7; break;
        case 26: cursor = (cursor + 8 - 1)&7; break;
        case 23: track = (track + 1)&3; break;
        case 24: track = (track + 4 - 1)&3; break;
      }
    }
    break;
  }
}


void rythmApp(){
  static byte count = 0;
  static int dcount = 0;
  static byte rcount = 0;

  char vram[12];
  byte tmp;
  switch(mode){
  case M_RYTHM:
    vram[0] = genVram(' ');
    vram[1] = genVram('-');
    vram[2] = genVram('0' + rcount);
    vram[3] = genVram(' ');
    for(byte i = 4; i < 12; i ++){
      switch(rythm[i - 4]){
        case 0: vram[i] = genVram(' '); break;
        case 1: vram[i] = genVram('_'); break;
        case 2: vram[i] = genVram('-'); break;
        case 3: vram[i] = genVram('='); break;
      }
    }
    tmp = ((rcount+7) & 7) + 4;
    vram[tmp] = vram[tmp] | 0B00010000;
    break;

  case M_TONE:
    vram[0] = genVram(' ');
    vram[1] = genVram('0' + track);
    vram[2] = genVram('0' + rcount);
    vram[3] = genVram(' ');

    for(byte i = 4; i < 12; i ++){
      switch(melody[track][i - 4]){
        case 0: vram[i] = genVram('0'); break;
        case 1: vram[i] = genVram('1'); break;
        case 2: vram[i] = genVram('2'); break;
        case 3: vram[i] = genVram('3'); break;
        case 4: vram[i] = genVram('4'); break;
        case 5: vram[i] = genVram('5'); break;
        case 6: vram[i] = genVram('6'); break;
        case 7: vram[i] = genVram('7'); break;
        case 8: vram[i] = genVram('8'); break;
        case 9: vram[i] = genVram('9'); break;
        case 10: vram[i] = genVram('A'); break;
        case 11: vram[i] = genVram('b'); break;
        case 12: vram[i] = genVram('C'); break;
        case 255: vram[i] = genVram('-'); break;
      }
    }
    if((dcount & 0B100000) == 0B100000){
      vram[4 + cursor] = 0;
    }
    break;
  }

  lcd_fill(vram);


  if(count > (ecount>>2) + 0xf){
    count = 0;
    if(rythm[rcount]){
      if((rythm[rcount]&0B01) == 0B01)nf = 0xf;
      if((rythm[rcount]&0B10) == 0B10)nf2 = 0xf;
    }
    for(byte i = 0; i < 4; i ++){
      if(melody[i][rcount] != 255){
        d[i] = getRawTone(melody[i][rcount]);
        vol[i] = 12;
      }
    }
    rcount = (rcount + 1)%8;
  }

  if((dcount & 0xf) == 0xf){
    if(nf != 0){
      nf = (nf >> 1);
    }
    if(nf2 != 0){
      nf2 = (nf2 >> 1);
    }
  }

  if((dcount & 0xf) == 0xf){
    if(vol[0] > 0){vol[0] --;}
    if(vol[1] > 0){vol[1] --;}
    if(vol[2] > 0){vol[2] --;}
    if(vol[3] > 0){vol[3] --;}
  }

  count ++;
  dcount ++;
}

void toneUpApp(){
  char vram[12];
  sprintf(vram, " --%03d------", ecount >> 2);
  d[1] = getRawTone((ecount >> 2));
  lcd_fill(vram);
}
