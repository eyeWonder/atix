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
