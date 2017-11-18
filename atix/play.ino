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


