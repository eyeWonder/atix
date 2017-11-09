/* Atix is a conceptual device using an audio interface  learning  
 * to learn and practice words in a foreign language.
 * http://github.com/hfvaldesg/atix
 * https://hackaday.io/project/27982-atix-language-learning-audio-based-device
 * Developed by Hernán Valdés
 */
const int NUMOFNUMBERS = 20;
int numbers[NUMOFNUMBERS];
const int X_pin = 1; // analog pin connected to X output
const int Y_pin = 0; // analog pin to Y output
const int button = 2; //digital pin connected to button in Joystick
int bRead;
void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);
  digitalWrite(2, HIGH); //Set for the switch in the Joystick
  randomSeed(analogRead(A3));
  set_volume(20);
  play(3,2);
  delay(2000);
}

void loop() {  
  menu();
}
void set_volume(int opt){
  static uint8_t volume_value[5]={0};
  uint8_t volume_total[30]={0x01, 0x02, 0x03, 0x04, 0x05,
                            0x06,0x07,0x08,0x09,0x0a,0x0b,
                            0x0c,0x0d,0x0e,0x0f,0x10,0x11,
                            0x12,0x13,0x14,0x15,0x16,0x17,
                            0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e};
  volume_value[0]=0x7e;
  volume_value[1]=0x03;
  volume_value[2]=0x31;
  volume_value[3]=volume_total[opt-1];
  volume_value[4]=0xef;
  for (uint8_t i = 0; i < 5; i++) //Set the initial volume
  {
    Serial.write(volume_value[i]);
  }
  delay(100);
}
void control_volume(){
  int value=15; //Value of the volume
  boolean flag_volume=false;
  while(true){
    bRead=digitalRead(button);
    if(analogRead(Y_pin)/10>98 && value<30){  //Up in the Y axis    
      value++;
      flag_volume=true;   
    }
    else if(analogRead(Y_pin)/10<3 && value>1){ //Down in the Y axis
      value--;   
      flag_volume=true;
    }
    if(flag_volume){
      flag_volume=false;
      set_volume(value);
      play(8,2); //Play sample pip        
      delay(100);   
    }    
    if(bRead==LOW){ //Confirm the volume selection
      play(1,2); //Play confirmation
      delay(300);
      return; 
    }
  }  
}
void play(int index,int audio_option) {  
  static uint8_t play0[6] = {0};
  uint8_t audio_words[20] = { 0x01, 0x02, 0x03, 0x04, 0x05,0x06,0x07,0x08,0x09,
                              0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14};
  uint8_t audio_general[12]={0x01, 0x02, 0x03, 0x04, 0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c}; 
  uint8_t audio_options[3]={0x01,0x02,0x03}; //01: English,02: German, 03: General audio
  play0[0] = 0x7e;
  play0[1] = 0x04;
  play0[2] = 0x42;
  play0[3] = audio_options[audio_option];
  switch(audio_option){
    case 0:
    case 1:
      play0[4] = audio_words[index-1];
      break;
    case 2:
      play0[4] = audio_general[index-1];
  }  
  play0[5] = 0xef;
  for (uint8_t i = 0; i < 6; i++) {
    Serial.write(play0[i]);
  }
  switch(audio_option){
    case 0:
    case 1:
      delay(200);
    case 2:
      switch(audio_general[index-1]){ //Custom volumes
        case 0x01: //correct
        case 0x02: //wrong
          delay(700);
          break;
        case 0x08: //pip
          delay(100);
          break;
        case 0x0b:
        case 0x0c:
          delay(500);
          break;
        default:
          delay(600);   
          break;
      }
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
  }
} 
void exercise(){ //Practice 5 random words of the list
  int exercise_word;
  int option=-1;
  randomizeList();  
  exercise_word=numbers[random(0,4)];  
  play(exercise_word,0);
  delay(200);
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
        play(1,2); //good answer sound
        break;
      }
      else if(option==-1){ //Repeat the word, if you couldn't understand it
        play(exercise_word,0);
      }
      else{
        play(2,2); //wrong answer sound
      }
      delay(300);
    }  
  }  
}
boolean repeat_session(){
  play(10,2);
  boolean answer;
  while(true){
    bRead=digitalRead(button);
    if(analogRead(X_pin)/10>98){  //Up in the Y axis    
      play(12,2); //no
      answer=false; 
    }
    else if(analogRead(X_pin)/10<3){ //Down in the Y axis
      play(11,2);
      answer=true;
    }   
    if(bRead==LOW){ //Confirm if the user want to keep playing
      if(answer==true || answer ==false){
        play(1,2); //Play confirmation
        delay(300);
        return answer;         
      }
      else{
        play(2,2); //Play wrong
      }
    }
  }      
}
void menu(){
  boolean flag=true;
  boolean flag_menu=false;
  boolean continue_exercise=true;
  int menu_option=1;
  int menu_options[2]={4,5}; //Learn (4) || Exercise (5)
  if(flag){
    play(6,2); //Play the main menu audio
    delay(1000);
    flag=false; 
    play(4,2);//Play the first option (Learn)  
  }
  while(true){
    bRead=digitalRead(button);
    if(analogRead(X_pin)/10>98 && menu_option<3){  //To the right in the menu    
      menu_option++;
      flag=true;
      flag_menu=true;
      delay(200);
    }
    else if(analogRead(X_pin)/10<3 && menu_option>1){ //To the left in the menu
      if(menu_option!=0){
        menu_option--;
        flag=true;  
        flag_menu=true;     
        delay(200);        
      }
    }
    if(flag_menu){
      switch(menu_option){ //Play the name of the menu option
        case 1:
          play(4,2); //Play the first option          
          break;
        case 2:
          play(5,2); //Play the second option
          break;
        case 3:
          play(9,2); //Play the third option
          break;
      } 
      flag_menu=false;     
    }
    if(bRead==LOW){ //Confirm the menu option
      switch(menu_option){
        case 1: //Learn (not ready. don't know what to do)
          return;    
        case 2: //Exercise
          play(1,2);
          while(continue_exercise){
            for(int i=0;i<5;i++){
            exercise(); //Begin the exercise
            } //Maybe ask at the end of the session, if the user wants another round of practice
            continue_exercise=repeat_session();
            if(continue_exercise==false){
              return;      
            }              
          }          
        case 3: //Control Volume
          play(1,2);
          delay(200);
          control_volume();
          return;
      }
    }
  }  
}
