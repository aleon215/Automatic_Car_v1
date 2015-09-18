#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"
#include <IRremote.h>

//int accelX;
//int accelY;
int giro = 0;
int modo = 0; // 0=Auto, 1=Manual

// Laser
int LASER = 4;

// Configurar variables para sensor IR
int LED_PIN = 5;
int IR_PIN = 11;
IRrecv irrecv(IR_PIN);
decode_results IR_resultados;

// Crear el objeto del driver de motores
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Instanciar los motores
Adafruit_DCMotor *myMotor1LeftBack = AFMS.getMotor(1);
Adafruit_DCMotor *myMotor2LeftFront = AFMS.getMotor(2);
Adafruit_DCMotor *myMotor3RightBack = AFMS.getMotor(3);
Adafruit_DCMotor *myMotor4RightFront = AFMS.getMotor(4);

void setup() {
  // Laser
  pinMode(LASER, OUTPUT);
  // Serial.begin(9600);

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  // Set the speed to start, from 0 (off) to 255 (max speed)
  myMotor1LeftBack->setSpeed(0);
  myMotor1LeftBack->run(FORWARD);
  // turn on motor
  myMotor1LeftBack->run(RELEASE);
  
   // Set the speed to start, from 0 (off) to 255 (max speed)
  myMotor2LeftFront->setSpeed(0);
  myMotor2LeftFront->run(FORWARD);
  // turn on motor
  myMotor2LeftFront->run(RELEASE);
  
     // Set the speed to start, from 0 (off) to 255 (max speed)
  myMotor3RightBack->setSpeed(0);
  myMotor3RightBack->run(FORWARD);
  // turn on motor
  myMotor3RightBack->run(RELEASE);
  
     // Set the speed to start, from 0 (off) to 255 (max speed)
  myMotor4RightFront->setSpeed(0);
  myMotor4RightFront->run(FORWARD);
  // turn on motor
  myMotor4RightFront->run(RELEASE);
  
  // Sensores de distancia
  pinMode(9, OUTPUT); // Activación del pin 9 como salida: para el pulso ultrasónico
  pinMode(8, INPUT); // Activación del pin 8 como entrada: tiempo del rebote del ultrasonido
  pinMode(7, OUTPUT); // Activación del pin 7 como salida: para el pulso ultrasónico
  pinMode(6, INPUT); // Activación del pin 6 como entrada: tiempo del rebote del ultrasonido
  
  // Led de señal de IR
  pinMode(LED_PIN, OUTPUT);
  
  // Arrancamos el sensor IR
  irrecv.enableIRIn();
  
  //pinMode(0, INPUT); // Acelerometro axis X
  //pinMode(1, INPUT); // Acelerometro Axis Y
}

void loop() {

  //Activar laser
  digitalWrite(LASER, HIGH);
  
  // Leer infrarojos
  
  if (irrecv.decode(&IR_resultados)) {
    digitalWrite(LED_PIN, HIGH);
    //Serial.println(results.value, DEC);
    irrecv.resume(); // Recibir el siguiente valor
    delay(200);
    
    switch(IR_resultados.value) {
      
      // Boton "POWER mando" SAMSUNG --> modo automatico (default) = 0
      case 3392264773:
        modo=0;
      break;
      
      // Boton "TV POWER" mando SAMSUNG --> modo manual = 1
      case 3772793023:
        modo=1;
        detener();
      break;
      
      // Boton "Vol+" --> Aumentar velocidad
      case 3476004267:
        adelantar();
      break;
      
      // Boton "Vol-" --> Retroceder
      case 2998643817:
        retroceder();
      break;
      
      // Boton "Level <" --> Girar izquierda
      case 4218783810:
        giro_izquierda();
      break;
      
      // Boton "Level >" --> Girar derecha
      case 1863712582:
        giro_derecha();
      break;
      
      // Boton "MUTE" --> Detener
      case 305976139:
        detener();
      break;
    }
  }
  

  
  digitalWrite(LED_PIN, LOW);
  
  // Reading accelaration
  //accelX = analogRead(0);
  //accelY = analogRead(1);
  //Serial.print("X:  ");
  //Serial.println(accelX);
  //Serial.print("Y:  ");
  //Serial.println(accelY);
  //delay(1000);
  
  if (modo==0) {modo_automatico();}
  
}

long medir_distancia (int sensor) {
  long distancia;
  long tiempo;
  if (sensor==1) {
    // Getting distance sensor 1
    digitalWrite(9,LOW); /* Por cuestión de estabilización del sensor*/ 
    delayMicroseconds(5);
    digitalWrite(9, HIGH); /* envío del pulso ultrasónico*/
    delayMicroseconds(10);
    tiempo=pulseIn(8, HIGH);
    distancia= int(0.017*tiempo); /*fórmula para calcular la distancia obteniendo un valor entero*/
    return distancia;
  }
  if (sensor==2) {
     // Getting distance sensor 2
    digitalWrite(7,LOW); /* Por cuestión de estabilización del sensor*/
    delayMicroseconds(5);
    digitalWrite(7, HIGH); /* envío del pulso ultrasónico*/
    delayMicroseconds(10);
    tiempo=pulseIn(6, HIGH);
    distancia= int(0.017*tiempo); /*fórmula para calcular la distancia obteniendo un valor entero*/
    return distancia;
  }
}

void retroceder() {
    
    uint8_t i;
    
    myMotor1LeftBack->run(BACKWARD);
    myMotor2LeftFront->run(BACKWARD);
    myMotor3RightBack->run(BACKWARD);
    myMotor4RightFront->run(BACKWARD);
    
    i = 80;
    
    myMotor1LeftBack->setSpeed(i);  
    myMotor2LeftFront->setSpeed(i); 
    myMotor3RightBack->setSpeed(i);
    myMotor4RightFront->setSpeed(i);
    
    delay(400); 
}

void giro_derecha() {
  
  //Serial.print("Giro derecha: ");
  //Serial.println(giro);
  
  uint8_t i;
      
  myMotor1LeftBack->run(FORWARD);
  myMotor2LeftFront->run(FORWARD);
  myMotor3RightBack->run(BACKWARD);
  myMotor4RightFront->run(BACKWARD);
        
  i = 80;
  myMotor1LeftBack->setSpeed(i);  
  myMotor2LeftFront->setSpeed(i); 
  i = 140;
  myMotor3RightBack->setSpeed(i);
  myMotor4RightFront->setSpeed(i);
        
  delay(200);
}

void giro_izquierda() {

  //Serial.print("Giro izquierda: ");
  //Serial.println(giro);
  
  uint8_t i;
      
  myMotor1LeftBack->run(BACKWARD);
  myMotor2LeftFront->run(BACKWARD);
  myMotor3RightBack->run(FORWARD);
  myMotor4RightFront->run(FORWARD);
    
  i = 80;
  myMotor1LeftBack->setSpeed(i);  
  myMotor2LeftFront->setSpeed(i); 
  
  i = 140;
  myMotor3RightBack->setSpeed(i);
  myMotor4RightFront->setSpeed(i);
    
  delay(200);
}

void adelantar() {
  
  uint8_t i;
  
  myMotor1LeftBack->run(FORWARD);
  myMotor2LeftFront->run(FORWARD);
  myMotor3RightBack->run(FORWARD);
  myMotor4RightFront->run(FORWARD);

  i = 150;
  
  myMotor1LeftBack->setSpeed(i);  
  myMotor2LeftFront->setSpeed(i);
  myMotor3RightBack->setSpeed(i);
  myMotor4RightFront->setSpeed(i);
  
  delay(5);
}

void detener() {
  
  uint8_t i;
  
  myMotor1LeftBack->run(FORWARD);
  myMotor2LeftFront->run(FORWARD);
  myMotor3RightBack->run(FORWARD);
  myMotor4RightFront->run(FORWARD);

  i = 0;
  
  myMotor1LeftBack->setSpeed(i);  
  myMotor2LeftFront->setSpeed(i);
  myMotor3RightBack->setSpeed(i);
  myMotor4RightFront->setSpeed(i);
  
  delay(5);
}

void modo_automatico() {
  
  if (medir_distancia(1) < 20 | medir_distancia(2) < 20) {
    
    if (giro == 10) {giro = 0;}
      retroceder();

    // Determinamos el lado del giro
    if (giro < 5) {
      
      // Giramos 5 veces a la derecha
      giro_derecha();
      giro = giro + 1;

    }
    // Giramos 5 veces a la izquierda
    else {
      giro_izquierda();
      giro = giro + 1;
    }
  } 
  
  // Vamos hacia adelante!
  else {
    adelantar();
  }
}
