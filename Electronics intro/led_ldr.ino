
int ldr       = 15;
int led_red   =  5;
int led_green =  6;
int led_blue  =  9;


void setup() {
  Serial.begin(9600);
  pinMode(ldr,       INPUT);
  pinMode(led_red,   OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(led_blue,  OUTPUT);
  
}
int i_red = 0, i_green = 0, i_blue = 0; 
void loop() {
  int input;
  
  
  input = analogRead(ldr);
  Serial.println(input);
  
  if(input < 400)
  {
    i_blue = 0;
    i_green = 0;
    digitalWrite(led_blue, LOW);
    digitalWrite(led_green, LOW);
    if(i_red == 0)
      for(i_red = 0; i_red < 256; i_red++)
        {
          analogWrite(led_red, i_red);
          delay(2);
        }
  }
  else if(input < 700)
  {
    i_blue = 0;
    i_red = 0;
    digitalWrite(led_red, LOW);
    digitalWrite(led_blue, LOW);
    if(i_green == 0)
      for(i_green = 0; i_green < 256; i_green++)
      {
         analogWrite(led_green, i_green);
         delay(2);
      }
  }  
  else
  {
    i_red = 0;
    i_green = 0;
    digitalWrite(led_red, LOW);
    digitalWrite(led_green, LOW);
    if(i_blue == 0)
      for(i_blue = 0; i_blue < 256; i_blue++)
      {
         analogWrite(led_blue, i_blue);
         delay(2);
      }
   }
    
 
}
