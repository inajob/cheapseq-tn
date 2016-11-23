#include <tones.h>
#include <encoder.h>
#include <lcd.h>
#include <keys.h>


byte rythm[4][8] = {
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0}
};
byte melody[4][8] = {
{255,255,255,255,255,255,255,255},
{255,255,255,255,255,255,255,255},
{255,255,255,255,255,255,255,255},
{255,255,255,255,255,255,255,255}
};

byte prythm[8] = {255,255,255,255,255,255,255,255};
byte pmelody[8] = {255,255,255,255,255,255,255,255};

#define M_RYTHM 0
#define M_TONE 1

#define M_PRYTHM 4
#define M_PTONE 5
byte mode = M_RYTHM;
byte cursor = 0;
byte track = 0;
byte pattern = 0;
byte rpattern = 0;

byte rcount = 0;
byte pcount = 0; // pattern count



void setup(){
  tones_init();
  encoder_init();
  lcd_init();
  keys_init();

  Serial.begin(9600);
}

void loop(){
  // todo: name change
  rythmApp();
  keys_scan();

  //Serial.println(ecount);
}


void key(byte n){
  if(trigger[20] > 2){
    switch(n){
      case 0:
        mode = M_RYTHM;
	pattern = 0;
	rpattern = 0;
	return;
      case 2:
        mode = M_TONE;
	pattern = 0;
	rpattern = 0;
	return;
      case 7:
        mode = M_PRYTHM;
	rcount = 0;
	pcount = 0;
	return;
      case 9:
	rcount = 0;
	pcount = 0;
        mode = M_PTONE;
	return;
    }
  }
  switch(mode){
  case M_RYTHM:
    switch(n){
      case 0: rythm[rpattern][0] =  (rythm[rpattern][0] + 1) & 3; break;
      case 2: rythm[rpattern][1] =  (rythm[rpattern][1] + 1) & 3; break;
      case 4: rythm[rpattern][2] =  (rythm[rpattern][2] + 1) & 3; break;
      case 5: rythm[rpattern][3] =  (rythm[rpattern][3] + 1) & 3; break;
      case 7: rythm[rpattern][4] =  (rythm[rpattern][4] + 1) & 3; break;
      case 9: rythm[rpattern][5] =  (rythm[rpattern][5] + 1) & 3; break;
      case 11: rythm[rpattern][6] = (rythm[rpattern][6] + 1) & 3; break;
      case 12: rythm[rpattern][7] = (rythm[rpattern][7] + 1) & 3; break;

      case 23: rpattern = (rpattern + 1)&3; break;
      case 24: rpattern = (rpattern + 4 - 1)&3; break;
    }
    break;
  case M_TONE:
    if(n <= 12){
      d[1] = getRawTone(n);
      vol[1] = 12;
      melody[pattern][cursor] = n;
    }else{
      switch(n){
        case 22: melody[pattern][cursor] = 255; break;
        case 25: cursor = (cursor + 1)&7; break;
        case 26: cursor = (cursor + 8 - 1)&7; break;
	// todo: pattern max
        case 23: pattern = (pattern + 1)&3; break;
        case 24: pattern = (pattern + 4 - 1)&3; break;
      }
    }
    break;
  case M_PRYTHM:
    if(n <= 12){
      prythm[cursor] = n;
    }else{
      switch(n){
        case 22: prythm[cursor] = 255; break;
        case 25: cursor = (cursor + 1)&7; break;
        case 26: cursor = (cursor + 8 - 1)&7; break;
      }
    }
    break;
  case M_PTONE:
    if(n <= 12){
      pmelody[cursor] = n;
    }else{
      switch(n){
        case 22: pmelody[cursor] = 255; break;
        case 25: cursor = (cursor + 1)&7; break;
        case 26: cursor = (cursor + 8 - 1)&7; break;
      }
    }
    break;
  }
}


void rythmApp(){
  static byte count = 0;
  static int dcount = 0;
  char vram[12];
  byte tmp;
  switch(mode){
  case M_RYTHM:
    vram[0] = genVram(' ');
    vram[1] = genVram('0');
    vram[2] = genVram('0' + rcount);
    vram[3] = genVram('0' + rpattern);
    for(byte i = 4; i < 12; i ++){
      switch(rythm[rpattern][i - 4]){
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
    vram[1] = genVram('1');
    vram[2] = genVram('0' + rcount);
    vram[3] = genVram('0' + pattern);

    for(byte i = 4; i < 12; i ++){
      switch(melody[pattern][i - 4]){
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
  case M_PRYTHM:
    vram[0] = genVram(' ');
    vram[1] = genVram('4');
    vram[2] = genVram('0' + rcount);
    vram[3] = genVram(' ');

    for(byte i = 4; i < 12; i ++){
      if(prythm[i - 4] != 255){
        vram[i] = genVram('0' + prythm[i - 4]);
      }else{
        vram[i] = genVram('-');
      }
    }

    if((dcount & 0B100000) == 0B100000){
      vram[4 + cursor] = 0;
    }
    break;
  case M_PTONE:
    vram[0] = genVram(' ');
    vram[1] = genVram('5');
    vram[2] = genVram('0' + rcount);
    vram[3] = genVram(' ');

    for(byte i = 4; i < 12; i ++){
      if(pmelody[i - 4] != 255){
        vram[i] = genVram('0' + pmelody[i - 4]);
      }else{
        vram[i] = genVram('-');
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
    if(rpattern != 255){
      // play rythm
      if(rythm[rpattern][rcount]){
        if((rythm[rpattern][rcount]&0B01) == 0B01)nf = 0xf;
        if((rythm[rpattern][rcount]&0B10) == 0B10)nf2 = 0xf;
      }
    }
    if(pattern != 255){
      // play melody
      for(byte i = 0; i < 4; i ++){
        if(melody[pattern][rcount] != 255){
          d[i] = getRawTone(melody[pattern][rcount]);
          vol[i] = 12;
        }
      }
    }
    rcount = (rcount + 1)%8;
    switch(mode){
      case M_PRYTHM:
      case M_PTONE:
      if(rcount == 0){
        rpattern = prythm[pcount];
        pattern = pmelody[pcount];
        pcount = (pcount + 1) & 7;
      }
      break;
    }
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
