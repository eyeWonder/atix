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
