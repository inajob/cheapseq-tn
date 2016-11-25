#define WR 13
#define DATA 11
#define CS 10

void data(bool b){
  digitalWrite(WR, LOW);
  digitalWrite(DATA, b);
  digitalWrite(WR, HIGH);
}
void com(bool b0, bool b1, bool b2){
  data(b0);
  data(b1);
  data(b2);
}
void op8(bool b0, bool b1, bool b2, bool b3, bool b4, bool b5, bool b6, bool b7){
  data(b0);
  data(b1);
  data(b2);
  data(b3);
  data(b4);
  data(b5);
  data(b6);
  data(b7);
}


void op(bool b0, bool b1, bool b2, bool b3, bool b4, bool b5, bool b6, bool b7, bool b8){
  data(b0);
  data(b1);
  data(b2);
  data(b3);
  data(b4);
  data(b5);
  data(b6);
  data(b7);
  data(b8);
}

void op4(bool b0, bool b1, bool b2, bool b3){
  data(b0);
  data(b1);
  data(b2);
  data(b3);
}
void op6(bool b0, bool b1, bool b2, bool b3, bool b4, bool b5){
  data(b0);
  data(b1);
  data(b2);
  data(b3);
  data(b4);
  data(b5);
}

void lcd_init(){
  pinMode(WR, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(CS, OUTPUT);
  
  
  digitalWrite(CS, HIGH);
  
  // == 9bit code sequence ==
  // SYS EN  100 0000-0001-X
  // BIAS    100 0010-1001-X 1/3bias 4 commons option
  // LCD ON  100 0000-0011-X
  
  // pattern

  // BIAS    100 0010-0000-X 1/2bias 2 commons option
  // BIAS    100 0010-0100-X 1/2bias 3 commons option
  // BIAS    100 0010-1000-X 1/2bias 4 commons option
  
  // BIAS    100 0010-0001-X 1/3bias 2 commons option
  // BIAS    100 0010-0101-X 1/3bias 3 commons option
  // BIAS    100 0010-1001-X 1/3bias 4 commons option

  
  digitalWrite(CS, HIGH);
  digitalWrite(CS, LOW);
  com(1,0,0); op(0,0,0,0, 0,0,0,1, 0);
  digitalWrite(CS, HIGH);
  digitalWrite(CS, LOW);
  com(1,0,0); op(0,0,1,0, 1,0,0,1, 0);
  digitalWrite(CS, HIGH);
  digitalWrite(CS, LOW);
  com(1,0,0); op(0,0,0,0, 0,0,1,1, 0);
  digitalWrite(CS, HIGH);
  digitalWrite(CS, LOW);
  
  com(1,0,1); op6(0,0,0,0,0,0);

  op4(1,1,1,1);  
  op4(1,1,1,1);  
  op4(1,1,1,1);  
  op4(1,1,1,1);  
  
//  for(int i = 0; i < 100; i ++){
//    op4(0,0,0,0);
//  }

  digitalWrite(CS, HIGH);
  digitalWrite(CS, LOW);
}

byte genVram(char c){
  switch(c){
    case ' ': return 0B00000000;
    case '*': return 0B11111111;
    case '-': return 0B00000010;
    case '0': return 0B11110101;
    case '1': return 0B00000101;
    case '2': return 0B11010011;
    case '3': return 0B10010111;
    case '4': return 0B00100111;
    case '5': return 0B10110110;
    case '6': return 0B11110110;
    case '7': return 0B00110101;
    case '8': return 0B11110111;
    case '9': return 0B10110111;
    case 'o': return 0B11000110;
    case 'O': return 0B00110011; // upper o
    case '_': return 0B10000000;
    case '~': return 0B00010000;
    case '=': return 0B10000010; // under =
    case '^': return 0B00010010; // upper =
    case '#': return 0B10010000; // wide =
    case '%': return 0B10010010; // three line
    case 'A': return 0B01110111;
    case 'b': return 0B11100110;
    case 'C': return 0B11110000;
    case 'd': return 0B11000111;
    case 'P': return 0B01110011;
    case 'E': return 0B11110010;
    case 'H': return 0B01100111;
    case 'F': return 0B01110010;
    case 't': return 0B00000110;
    case 'T': return 0B01110000;
    case 'U': return 0B11100101;
    case 'r': return 0B01000010;
    case 'n': return 0B01000110;
    case 'Y': return 0B01100011;
  }
  return 0;
}
void genString(char* s, char* vram){
  while(*(s)){
    *(vram++) = genVram(*s++);
  }
}



void lcd_fill(char* str){
  digitalWrite(CS, HIGH);
  digitalWrite(CS, LOW);
  com(1,0,1);op6(0,0,0,0,0,0);

  for(byte i = 0; i < 12; i ++){
    op8(
     bitRead(*str, 7),
     bitRead(*str, 6),
     bitRead(*str, 5),
     bitRead(*str, 4),
     bitRead(*str, 3),
     bitRead(*str, 2),
     bitRead(*str, 1),
     bitRead(*str, 0)
    );
    str ++;
  }
}

