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
        case 1: //Exercise
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
        case 2: //Control Volume
          delay(200);
          control_volume();
          return;
  }  
}
