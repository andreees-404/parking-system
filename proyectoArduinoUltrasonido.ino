#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Servo.h>

/* LED Variables  */
const int ledRojo = 6;
const int ledVerde = 5;
const int ledAzul = 4;
const int piezo = 12;

const int MIN_DISTANCE = 25;

// Pantalla LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Servomotor
Servo servomotor;

/* Variables del button
@var button
	Pin del pulsador
    
@var buttonState
	Estado del button (LOW - HIGH),
    representados en un Integer
*/
const int buttonIn = 7;
const int buttonOut = 4;

int buttonInState = 0;
int buttonOutState = 0;

/*
@Test
Esta variable hace referencia a
la disponibilidad del estacionamiento
*/
int available = 2;


/*
  Ultrasonic Sensor HC-SR04
@var echo
  Pin que recibe las señales
  
@var trig
  Pin que envía las señales
*/
const int trig = 8;
const int echo = 9;

/* Duración y distancia del objeto frente al sensor */
long duration;
int distance;

void setup()
{
  //LCD
  lcd.begin(); 
  lcd.backlight();
  Serial.begin(9600);
  
  // Diodos LED
  pinMode(ledRojo,OUTPUT);
  pinMode(ledVerde,OUTPUT);
  pinMode(ledAzul,OUTPUT);
  
  // Servomotor
  servomotor.attach(13);
 
  // Pines del Ultrasonic Sensor
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
 
  // Pulsadores
  pinMode(buttonIn, INPUT);
  pinMode(buttonOut, INPUT);
  

}

void loop()
{ 
   
  lcd.setCursor(0,0);
  lcd.print("Disponible: ");

  lcd.setCursor(12,0);
  lcd.print(available);	
  
  buttonInState = digitalRead(buttonIn);
  buttonOutState = digitalRead(buttonOut);
  
 
  
  clearTrigPin();
  digitalWrite(trig, HIGH);
  digitalWrite(trig, LOW);

  // Leer los datos que recibe el pin echo
  duration = pulseIn(echo, HIGH);

  // Calcular la distancia
  distance = duration * 0.034 /2;
  
  if(buttonInState == HIGH){
  	unlockBarrier(distance);
  }else if (buttonOutState == HIGH){
    vehicleOut(distance);
  }else if (available == 0){
     if(distance > MIN_DISTANCE){
    	servomotor.write(0);
    }
    noAvailable(distance);
   
    
  }else{
    if(distance > MIN_DISTANCE){
    	servomotor.write(0);
    }
  }
  
  noTone(piezo);
	
      
  // Encender el LED rojo
  analogWrite(ledRojo,255);
  analogWrite(ledVerde,0);
  analogWrite(ledAzul,0);
  	
}


/* 
  Limpiar el pin emisor
 */
void clearTrigPin(){
  digitalWrite(trig, LOW);
  delay(100);
}


/*
  Imprimir la distancia
  
  @param _distance
  	Distancia entre el objeto y el sensor
*/
void unlockBarrier(int _distance){
  lcd.clear();
  if(available <= 25 && available > 0){
    Serial.print("Distance: ");
    Serial.println(_distance);
    
    if(_distance < MIN_DISTANCE){
      
      available--;
      // Girar el motor en 90°
      servomotor.write(90);
	
      // Encender el LED verde
      analogWrite(ledRojo,0);
      analogWrite(ledVerde, 255);
      analogWrite(ledAzul,0);

       // Secuencia piezo
      for(int i = 0; i < 3; i++){
        noTone(piezo);    
        delay(500);
        tone(piezo, 500);
      }
    // Imprimir en el LCD
    lcd.setCursor(0, 0);
    lcd.print("Bienvenide");
    }
  } 
noTone(piezo);
  delay(1000);
  lcd.clear();
}


/*
Informar que no quedan más
espacios en el estacionamiento

@var _distance
	Distancia entre el objeto y el 
    sensor ultrasónico
*/
void noAvailable(int _distance){
  lcd.clear();
  Serial.print("Distancia: ");
  Serial.println(_distance);
  if(_distance < MIN_DISTANCE){
    
    analogWrite(ledRojo, 255);
    analogWrite(ledVerde, 0);
    analogWrite(ledAzul, 0);


    // Imprimir en el LCD
    lcd.setCursor(0, 0);
    lcd.print("No disponible");
    for (int i = 0; i < 13; i++) {
      delay(200);
      lcd.scrollDisplayLeft();
    }
    
  }

  delay(1000);
  lcd.clear();
}





/*
Marcar la salida de un vehículo
@var _distance
	Distancia entre el objeto y el sensor
*/
void vehicleOut(int _distance){
  if (available >= 0){
  	lcd.clear();
  	Serial.println("Vehicle out"); 
  if(_distance < MIN_DISTANCE){	
    if(available < 25){
    	available++; }
    String message = "Distancia: ";
    String dist = String(_distance);
    message.concat(dist);
    Serial.println(message);
      // Girar el motor en 90°
      servomotor.write(90);
      analogWrite(ledRojo,0);
      analogWrite(ledVerde, 255);
      analogWrite(ledAzul,0);
    // Secuencia piezo
      for(int i = 0; i < 3; i++){
        noTone(piezo);    
        delay(500);
        tone(piezo, 500); }
    // Imprimir en el LCD
    lcd.setCursor(0,1);
    lcd.print("Hasta luego!");
    noTone(piezo);
    for (int i = 0; i < 12; i++) {
    delay(200);
    lcd.scrollDisplayLeft();
    }
  }  
  }
  noTone(piezo);
  delay(1000);
  lcd.clear();
}

