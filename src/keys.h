extern void key(byte);

void keys_init(){
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  pinMode(17, INPUT_PULLUP);
  pinMode(18, INPUT_PULLUP);
}

static byte trigger[27];

void keys_trigger_press(byte n){
  if(trigger[n] < 255){
    trigger[n] ++;
  }
  if(trigger[n] == 1){
    key(n);
  }
}
void keys_trigger_up(byte n){
  trigger[n] = 0;
}

void keys_scan(){
  static byte mode = 0;
  static volatile byte keyMap[20] = {
    22, 3, 1,21,20,
     7, 5, 4, 2, 0,
     6, 8, 9,10,11,
    23,24,25,26,12
  };

  // == key scan ==
  switch(mode){
    case 0:
      pinMode(4, OUTPUT);digitalWrite(4, LOW);
      pinMode(5, INPUT);
      pinMode(6, INPUT);
      pinMode(7, INPUT);
    break;
    case 1:
      pinMode(4, INPUT);
      pinMode(5, OUTPUT);digitalWrite(5, LOW);
      pinMode(6, INPUT);
      pinMode(7, INPUT);
    break;
    case 2:
      pinMode(4, INPUT);
      pinMode(5, INPUT);
      pinMode(6, OUTPUT);digitalWrite(6, LOW);
      pinMode(7, INPUT);
    break;
    case 3:
      pinMode(4, INPUT);
      pinMode(5, INPUT);
      pinMode(6, INPUT);
      pinMode(7, OUTPUT);digitalWrite(7, LOW);
    break;
  }
  if(digitalRead(14) == LOW){keys_trigger_press(keyMap[mode * 5 + 0]);}else{keys_trigger_up(keyMap[mode * 5 + 0]);}
  if(digitalRead(15) == LOW){keys_trigger_press(keyMap[mode * 5 + 1]);}else{keys_trigger_up(keyMap[mode * 5 + 1]);}
  if(digitalRead(16) == LOW){keys_trigger_press(keyMap[mode * 5 + 2]);}else{keys_trigger_up(keyMap[mode * 5 + 2]);}
  if(digitalRead(17) == LOW){keys_trigger_press(keyMap[mode * 5 + 3]);}else{keys_trigger_up(keyMap[mode * 5 + 3]);}
  if(digitalRead(18) == LOW){keys_trigger_press(keyMap[mode * 5 + 4]);}else{keys_trigger_up(keyMap[mode * 5 + 4]);}
  mode = (mode + 1) % 4;
}
