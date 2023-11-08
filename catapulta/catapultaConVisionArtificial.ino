#include <IRremote.h>
#include <Servo.h>
#include <SoftwareSerial.h>



#define SENSOR_IR 11

#define IR_BUTTON_0 0
#define IR_BUTTON_2 17
#define IR_BUTTON_GIRAR_DER 6
#define IR_BUTTON_GIRAR_IZQ 4



#define lockOpen    100 
#define lockClosed  10 

#define armOpen     110 
#define armArmed    180

#define trayClosed  165
#define trayOpened  155

//Creacion de Servo
Servo lockServo ; 
Servo armServo ; 
Servo trayServo ; 
Servo baseServo ; 

int angulo = 60;
char dato; 

//Sensor IR
IRrecv irrecv(SENSOR_IR); //creamos la variable u objeto para el receptor IR
decode_results Valor;


void sweep ( Servo servo , int from , int to , int speed  );
void rest();
void prepareToShoot();
void shoot();
void MoverServo();

void setup() {

  Serial.begin(9600);
  Serial.setTimeout(10);

  //setup sensor IR
  // inicializamos la recepción de datos
  IrReceiver.begin(SENSOR_IR);

  /// conectamos los servos
  lockServo.attach(3); 
  armServo.attach(5); 
  trayServo.attach(6);
  baseServo.attach(9) ;  

  //base 
  baseServo.write(angulo);

  trayClose();
  // tiempo de carga de proyectil
  delay(2500); 
  trayRelease(); 

  rest() ; 
  prepareToShoot() ;  
  shoot();
  pinMode(13,OUTPUT);

}

void loop() {
  
  if (IrReceiver.decode())
  {
    //Serial.println(IrReceiver.decodedIRData.command);
    switch (IrReceiver.decodedIRData.command)
    {
      case IR_BUTTON_0:
      	Serial.println("Sistema Apagado");
      	break;
      case IR_BUTTON_2:
      	Serial.println("Sistema ejecutandose");
        trayClose();   
        delay(2500); 
        trayRelease(); 
      	rest() ; 
        prepareToShoot() ;  
        shoot(); 
      	break;
    }
    IrReceiver.resume();
  }
   MoverServo();
}


void rest() {
  
  sweep(lockServo,lockServo.read(),lockOpen,15) ; 
  sweep(armServo,armServo.read(),armOpen,15) ; 
 
}

void prepareToShoot() {

  sweep(lockServo,lockServo.read(),lockClosed,15) ; 
  sweep(armServo,armServo.read(),armArmed,15) ; 
  
}

void shoot() {

  sweep(lockServo,lockServo.read(),lockOpen,15) ; 
  
}


void sweep ( Servo servo , int from , int to , int speed  ) {

  int pos = 0 ;

  if (from < to) {

    for (pos = from; pos <= to; pos += 1) { 
      servo.write(pos);              
      delay(speed);                       
    }
    
  } else {

    for (pos = from; pos >= to; pos -= 1) { 
      servo.write(pos);              
      delay(speed);                       
    }

    
  }


}

void trayClose(){
  sweep(trayServo,trayServo.read(),trayClosed,22); 
}

void trayOpen() {
  sweep(trayServo,trayServo.read(),trayOpened,22) ; 
}

void trayRelease() {

    trayClose() ; 
    trayOpen() ; 
    trayClose() ; 
    delay(2000);

}


void MoverServo() {

  digitalWrite(13,LOW);
  while(Serial.available()){
    dato = Serial.read();
    delay(10);  
    switch(dato){
      case 'd':
      //Gira servo hacia la derecha
              digitalWrite(13,HIGH);
             if(angulo>180)
            {
              angulo = 180;
            }
            else
            {
              angulo+=1;
              for (int pos = baseServo.read(); pos <= angulo; pos += 1) { 
                baseServo.write(pos);              
                delay(10);                       
              }

            }
     
      
      break;
      
      case 'i':
      //Gira servo hacia la izquierda
          digitalWrite(13,HIGH);  
      if(angulo<0)
            {
              angulo = 0;
            }
            else
            {
              angulo-=1;
              for (int pos = baseServo.read(); pos >= angulo; pos -= 1) { 
                baseServo.write(pos);              
                delay(10);                       
              }
            }
           

      break;
      
      case 'p':
        //Parar el servo
        angulo = angulo;
        baseServo.write(angulo); 

      break;
      

      }
   }
 }