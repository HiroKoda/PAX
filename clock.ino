int i;
int SpeakerPin = 4;
int flag = 0;

#define BEATTIME 310 
#define PIN 12
 
#define LA 880
#define FA 740
#define MI 659
#define RE 587
#define DLA 440

void setup() {
 Serial.begin(9600);
 pinMode(2,INPUT);
 pinMode(3,OUTPUT);
 pinMode(SpeakerPin, OUTPUT);

}

void mario_1up(){  //マリオ
for (i=0; i<97; i++){
digitalWrite(SpeakerPin,HIGH);
delayMicroseconds(379);
digitalWrite(SpeakerPin,LOW);
delayMicroseconds(379);
}
for (i=0; i<235; i++){
digitalWrite(SpeakerPin,HIGH);
delayMicroseconds(319);
digitalWrite(SpeakerPin,LOW);
delayMicroseconds(319);
}
for (i=0; i<396; i++){
digitalWrite(SpeakerPin,HIGH);
delayMicroseconds(189);
digitalWrite(SpeakerPin,LOW);
delayMicroseconds(189);
}
for (i=0; i<315; i++){
digitalWrite(SpeakerPin,HIGH);
delayMicroseconds(238);
digitalWrite(SpeakerPin,LOW);
delayMicroseconds(238);
}
for (i=0; i<353; i++){
digitalWrite(SpeakerPin,HIGH);
delayMicroseconds(212);
digitalWrite(SpeakerPin,LOW);
delayMicroseconds(212);
}
for (i=0; i<471; i++){
digitalWrite(SpeakerPin,HIGH);
delayMicroseconds(159);
digitalWrite(SpeakerPin,LOW);
delayMicroseconds(159);
}

}

void famima(){  //ファミマ
  tone(PIN,FA,BEATTIME) ; // ファ#
delay(BEATTIME) ;
tone(PIN,RE,BEATTIME) ; // レ
delay(BEATTIME) ;
tone(PIN,DLA,BEATTIME) ; // dラ
delay(BEATTIME) ;
tone(PIN,RE,BEATTIME) ; // レ
delay(BEATTIME) ;
tone(PIN,MI,BEATTIME) ; // ミ
delay(BEATTIME) ;
tone(PIN,LA,BEATTIME) ; // ラ
delay(BEATTIME*2) ;
tone(PIN,DLA,BEATTIME) ; // dラ
delay(BEATTIME) ;
tone(PIN,MI,BEATTIME) ; // ミ
delay(BEATTIME) ;
tone(PIN,FA,BEATTIME) ; // ファ#
delay(BEATTIME) ;
tone(PIN,MI,BEATTIME) ; // ミ
delay(BEATTIME) ;
tone(PIN,DLA,BEATTIME) ; // dラ
delay(BEATTIME) ;
tone(PIN,RE,BEATTIME) ; // レ
}

void loop(){
  int val = digitalRead(2);
  if(val == 1){
   digitalWrite(3,1);
   if(flag == 0){
     mario_1up();
//   famima(); 
     flag = 1;  
   }  
  } else{
  digitalWrite(3,0);
  flag = 0;
  }

}
