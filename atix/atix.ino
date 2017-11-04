const int NUMOFNUMBERS = 4;
int numbers[NUMOFNUMBERS];
uint8_t vol[5] = { 0x7e, 0x03, 0x31, 0x19, 0xef }; //ef 10; 0f 15 ; 1E 30
static uint8_t play0[6] = {0};
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output
const int button = 2; //digital pin connected to button in Joystick

int bRead;
int flag=0;
int exercise_word;
int option;
void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);
  digitalWrite(2, HIGH);
  randomSeed(analogRead(A3));
  
  for (uint8_t i = 0; i < 5; i++) //
  {
    Serial.write(vol[i]);
  }
  delay(300);
  play(3,2);
  delay(2000);
}

void loop() {  
  randomizeList();  
  exercise_word=numbers[random(0,4)];  
  play(exercise_word,0);
  delay(1000);
  while(true){
    bRead=digitalRead(button);
    if(analogRead(X_pin)/10>98){
      option=numbers[0];
      play(numbers[0],1);
    }
    else if(analogRead(X_pin)/10<3){
      option=numbers[1];
      play(numbers[1],1);
    }
    else if(analogRead(Y_pin)/10>98){
      option=numbers[2];
      play(numbers[2],1);
    }
    else if(analogRead(Y_pin)/10<3){
      option=numbers[3];
      play(numbers[3],1);      
    }
    if(bRead==LOW){
      if(option==exercise_word){
        play(1,2);
        break;
      }
      else{
        play(2,2);
      }
      delay(300);
    }  
  }
  
}
void play(int option,int lang) {
  uint8_t audio_words[4] = { 0x01, 0x02, 0x03, 0x04};
  uint8_t language[3]={0x01,0x02,0x03}; //01: English,02: German, 03: Effects
  uint8_t audio_good[6]={0x7e,0x04,0x42,0x03,0x01,0xef};
  uint8_t audio_wrong[6]={0x7e,0x04,0x42,0x03,0x02,0xef};
  uint8_t startup[6]={0x7e,0x04,0x42,0x03,0x03,0xef};
  //play0[1]={0x7e, 0x04, 0x42, 0x01, 0x01, 0xef };
  play0[0] = 0x7e;
  play0[1] = 0x04;
  play0[2] = 0x42;
  play0[3] = language[lang];
  play0[4] = audio_words[option-1];
  play0[5] = 0xef;
  for (uint8_t i = 0; i < 6; i++) {
    Serial.write(play0[i]);
  }
  switch(lang){
    case 0:
    case 1:
      delay(400);
    case 2:
      delay(800);
  }
}
void randomizeList()
{
  unsigned char chosen[NUMOFNUMBERS];
  unsigned char index, i2;

  for (index = 0; index < NUMOFNUMBERS; index++)
    chosen[index] = 0;

  for (index = 0; index < NUMOFNUMBERS; index++)
  {
    int r = random(NUMOFNUMBERS-index);
    for (i2 = 0; i2 <= r; i2++)
    {
      r += chosen[i2];
    }
    chosen[r] = 1;
    numbers[index] = r+1;
    Serial.print(numbers[index]);
    Serial.print(" ");
  }
  Serial.println("");
} 

