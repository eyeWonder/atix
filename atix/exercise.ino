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
        play("language",1,exercise_word+group_factor);
      }
      else{
        play("effect",1,2); //wrong answer sound
      }
      delay(300);
    }  
  }  
}
