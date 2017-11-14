/* Atix is a conceptual device using an audio interface  learning  
 * to learn and practice words in a foreign language.
 * http://github.com/hfvaldesg/atix
 * https://hackaday.io/project/27982-atix-language-learning-audio-based-device
 * Developed by Hernán Valdés
 */
const int NUMOFNUMBERS = 10;
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
  play("effect",1,3);
  delay(2000);
}

void loop() {  
  start();
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
      play("effect",2,4); //Play sample pip        
      delay(100);   
    }    
    if(bRead==LOW){ //Confirm the volume selection
      play("effect",1,1); //Play confirmation
      delay(300);
      return; 
    }
  }  
}
//void play(int index,int audio_option) {  
void play(String opt1,int opt2, int index) {
  static uint8_t play0[6] = {0};
  uint8_t audio_words[20] = { 0x01, 0x02, 0x03, 0x04, 0x05,0x06,0x07,0x08,0x09,
                              0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14};
  uint8_t audio_general[12]={0x01, 0x02, 0x03, 0x04, 0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c}; 
  uint8_t audio_effects[4]={0x01, 0x02, 0x03, 0x04};
  uint8_t audio_menu[10]={0x01, 0x02, 0x03, 0x04, 0x05,0x06,0x07,0x08,0x09,0x0a};
  uint8_t audio_options[3]={0x01,0x02,0x03}; //01: English,02: German, 03: General audio
  play0[0] = 0x7e;
  play0[1] = 0x04;
  play0[2] = 0x42;
  if(opt1=="language" &&opt2==1){
    play0[3]=0x01; 
    play0[4]=audio_words[index-1];
  }
  else if(opt1=="language" &&opt2==2){
    play0[3]=0x02;
    play0[4]=audio_words[index-1];
  }
  else if(opt1=="effect"){
    play0[3]=0x03;
    play0[4]=audio_effects[index-1];
  }
  else if(opt1=="menu_item"){
    play0[3]=0x04;
    play0[4]=audio_menu[index-1];
  }  
  play0[5] = 0xef;
  for (uint8_t i = 0; i < 6; i++) {
    Serial.write(play0[i]);
  }
  if(opt1=="language"){
    delay(300);
  }
  else if(opt1=="effect"){
    switch(opt2){
      case 1: //Long duration
        delay(600);
        break;
      case 2: //Short duration
        delay(100);
        break;
    }
  }
  else if(opt1=="menu_item"){
    delay(600);  
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
void exercise(int group){ //Practice 5 random words of the list
  int exercise_word;
  int option=-1;
  int group_factor=10*(group-1);
  randomizeList();  
  exercise_word=numbers[random(0,4)];  
  play("language",1,exercise_word+group_factor);
  delay(200);
  while(true){
    bRead=digitalRead(button);
    if(analogRead(X_pin)/10>98){
      option=numbers[0];
      play("language",2,numbers[0]+group_factor);
    }
    else if(analogRead(X_pin)/10<3){
      option=numbers[1];
      play("language",2,numbers[1]+group_factor);
    }
    else if(analogRead(Y_pin)/10>98){
      option=numbers[2];
      play("language",2,numbers[2]+group_factor);
    }
    else if(analogRead(Y_pin)/10<3){
      option=numbers[3];
      play("language",2,numbers[3]+group_factor);      
    }
    if(bRead==LOW){
      if(option==exercise_word){
        play("effect",1,1); //good answer sound
        break;
      }
      else if(option==-1){ //Repeat the word, if you couldn't understand it
        play("language",1,exercise_word);
      }
      else{
        play("effect",1,2); //wrong answer sound
      }
      delay(300);
    }  
  }  
}
boolean return_main_menu(){
  play("menu_item",1,4);
  boolean answer;
  while(true){
    bRead=digitalRead(button);
    if(analogRead(X_pin)/10>98){  //Up in the Y axis         
      play("menu_item",1,7); //no
      answer=false;
    }
    else if(analogRead(X_pin)/10<3){ //Down in the Y axis
      play("menu_item",1,8); //yes
      answer=true;  
    }   
    if(bRead==LOW){ //Confirm if the user want to keep playing
      if(answer==true || answer ==false){
        play("effect",1,1); //Play confirmation
        delay(300);
        return answer;         
      }
      else{
        play("effect",1,2); //Play wrong
      }
    }
  }      
}
int menu_generator(String menu_name){
  int option=0;
  int options;
  boolean flag_menu=false;
  int main_menu[3]={1,2,5};
  int exercise_menu[2]={9,10};
  if(menu_name=="main_menu"){
    options=3;
  }
  else if(menu_name="exercise"){
    options=2;
  }
  while(true){
    bRead=digitalRead(button);
    if(analogRead(X_pin)/10>98 && option<options){  //To the right in the menu    
      option++;
      flag_menu=true;
      delay(200);
    }
    else if(analogRead(X_pin)/10<3 && option>1){ //To the left in the menu
      if(option!=0){
        option--;
        flag_menu=true;     
        delay(200);        
      }
    }
    if(flag_menu){
      if(menu_name=="main_menu"){
        play("menu_item",1,main_menu[option-1]);
      }
      else if(menu_name=="exercise"){
        play("menu_item",1,exercise_menu[option-1]);
      }           
    }
    flag_menu=false;
    if(bRead==LOW){ //Confirm the menu option
      play("effect",1,1);
      return option;
    }
  }      
}
void start(){
  boolean flag=true;
  boolean continue_exercise=true;
  int menu_option;
  int exercise_option;
  if(flag){
    play("menu_item",1,3); //Play the main menu audio
    delay(1000);
    flag=false;  
  }
  menu_option=menu_generator("main_menu");
  switch(menu_option){
        case 1: //Learn (not ready. don't know what to do)
          return;    
        case 2: //Exercise
          exercise_option=menu_generator("exercise");
          while(continue_exercise){
            for(int i=0;i<5;i++){
            exercise(exercise_option); //Begin the exercise
            } //Maybe ask at the end of the session, if the user wants another round of practice
            continue_exercise=return_main_menu();
            if(continue_exercise==false){
              return;      
            }              
          }          
        case 3: //Control Volume
          delay(200);
          control_volume();
          return;
  }  
}
