int menu_generator(String menu_name){
  int option=0;
  int options;
  boolean flag_menu=false;
  int main_menu[2]={2,5}; //without learn
  int exercise_menu[2]={9,10};
  if(menu_name=="main_menu"){
    options=2;
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
