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


#define E_SPEED 0
#define E_SHIFT 1
#define E_OCTAVE 2
#define E_TUNE 3
#define E_DETUNE 4
#define E_DECAY 5
#define E_RDECAY 6

byte mode = M_RYTHM;
byte emode = E_SPEED;
byte cursor = 0;
byte track = 0;
byte pattern = 0;
byte rpattern = 0;

byte rcount = 0;
byte pcount = 0; // pattern count
int speed = 0;
int deTune = 0;
int decay = 16;
int rdecay = 16;

int msgTimer = 0;
char msg[12];



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

void showEmode(){
  msgTimer = 256;
  char buf[12];
  switch(emode){
    case E_SPEED:
      sprintf(buf, "   5PEEd %03d", speed);
      genString(buf ,msg);
      break;
    case E_SHIFT:
      sprintf(buf, "   5H1FT %03d", shiftTone);
      genString(buf ,msg);
      break;
    case E_OCTAVE:
      sprintf(buf, "   oCTA %04d", octave);
      genString(buf ,msg);
      break;
     case E_TUNE:
      sprintf(buf, "   TUnE %04d", shiftTune);
      genString(buf ,msg);
      break;
     case E_DETUNE:
      sprintf(buf, "   dETUnE %02d", deTune);
      genString(buf ,msg);
      break;
      case E_DECAY:
      sprintf(buf, "   dECAY %03d", decay);
      genString(buf ,msg);
      break;
      case E_RDECAY:
      sprintf(buf, "   rdECAY %02d", rdecay);
      genString(buf ,msg);
      break;

  }
}

void rotate(char n){
  static char acc = 0;
  acc += n;

  char diff;

  if(abs(acc) >= 4){
    diff = (acc >> 2);

    switch(emode){
      case E_SPEED:
        speed = speed + diff;
        break;
      case E_SHIFT:
        shiftTone = shiftTone + diff;
        break;
      case E_OCTAVE:
        octave = octave + diff;
        break;
       case E_TUNE:
        shiftTune = shiftTune + diff;
        break;
       case E_DETUNE:
        deTune = deTune + diff;
        break;
        case E_DECAY:
        decay = decay + diff;
        break;
        case E_RDECAY:
        rdecay = rdecay + diff;
        break;
    }
    showEmode();
    acc = 0;
  }
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
  if(trigger[21] > 2){
    switch(n){
      case 0: emode = E_SPEED;break;
      case 1: emode = E_SHIFT;break;
      case 2: emode = E_OCTAVE;break;
      case 3: emode = E_TUNE;break;
      case 4: emode = E_DETUNE;break;
      case 5: emode = E_DECAY;break;
      case 6: emode = E_RDECAY;break;
    }
    showEmode();
    return;
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
  static unsigned int dcount = 0;
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

  if(msgTimer > 0){
    msgTimer --;
    lcd_fill(msg);
  }else{
    lcd_fill(vram);
  }

  // set detune
  vol[1] = vol[0];
  d[1] = d[0] + deTune;

  if(count > speed + 0xf){
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
      if(melody[pattern][rcount] != 255){
        d[0] = getRawTone(melody[pattern][rcount]);
        vol[0] = 12;
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

  if((dcount & (0xffff >> rdecay)) == (0xffff >> rdecay)){
    if(nf != 0){
      nf = (nf >> 1);
    }
    if(nf2 != 0){
      nf2 = (nf2 >> 1);
    }
  }

  if((dcount & (0xffff >> decay)) == (0xffff >> decay)){
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
