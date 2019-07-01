
void setup(){
 pinMode(2,INPUT);
 pinMode(4,INPUT);
 pinMode(5,OUTPUT);
 pinMode(3, OUTPUT);
}

void loop(){
  int trig = digitalRead(4);
  int lead = digitalRead(2);
  if(trig == 0 && lead == 0){
     digitalWrite(5,1);
     digitalWrite(3,1);
    } else{
    digitalWrite(5,0);
    digitalWrite(3,0);
  }
}
