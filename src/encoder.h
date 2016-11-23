int new_SW, old_SW;
volatile int ecount;
volatile int intcount; //割り込み回数

//const int QEM[] = {0, -1, 1, 2, 1, 0, 2, -1, -1, 2, 0, 1, 2, 1, -1, 0};
const int QEM[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
void QE_IRQ(void)
{
  old_SW = new_SW;
  new_SW = digitalRead (2) * 2 + digitalRead (3);
  ecount += QEM[old_SW * 4 + new_SW];
  intcount++; //割り込み回数カウント、通常は不要
}

void encoder_init(){
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(0, QE_IRQ, CHANGE);
  attachInterrupt(1, QE_IRQ, CHANGE);
  new_SW = digitalRead (2) * 2 + digitalRead (3);
}
