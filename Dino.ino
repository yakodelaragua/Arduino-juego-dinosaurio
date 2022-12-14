#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);
const int switchPin = 6;
int switchState = 0;
int prevSwitchState = 0;
int reply;
int salta = 0;
int posPiedra = 0;
int gameOver = 0;
int pulsado = 0;
int correcto = 0;
int puntos = 0;

byte cuerpo[8] = {
B01110,
B01110,
B00100,
B11111,
B00100,
B01010,
B10001,
B00000,
};

byte piedra[8] = {
B00000,
B00000,
B01111,
B11111,
B11111,
B11111,
B01111,
B00000,
};

byte nada[8] = {
B00000,
B00000,
B00000,
B00000,
B00000,
B00000,
B00000,
B00000,
};

void setup() {
  Serial.begin(9600);
  lcd.createChar(0,cuerpo);
  lcd.createChar(1,piedra);
  lcd.createChar(2,nada);

  
  lcd.begin(16, 2);
  pinMode(switchPin, INPUT);
  lcd.setCursor(1, 1);
  lcd.write(byte(0));   
   
  lcd.setCursor(13, 0);
  lcd.print(puntos);


}

void loop() {
  if(gameOver != 1){
    run(); 
    
  } else {
    lcd.setCursor(0,0);
    lcd.print("GAME OVER :(");
    puntos = 0;
    
    
    switchState = digitalRead(switchPin);

    if(switchState != prevSwitchState){
      if(switchState == LOW){ //Reiniciar partida
        lcd.clear();
        gameOver = 0;
        //pintar persona
        lcd.setCursor(1, 1);
        lcd.write(byte(0));  
      }   
    }
    prevSwitchState = switchState;
  }
  
 
}

void salto(){
    switchState = digitalRead(switchPin);

    if(switchState != prevSwitchState){
      if(switchState == LOW){
        
        if(posPiedra < 1 && correcto == 0){
          gameOver = 1;
          Serial.print("Game Over");
          
        } else if(posPiedra >= 1) { //animacion buen salto
        
         if(posPiedra == 1) {
            correcto = 1;          
          }
          
          //eliminar persona abajo
          lcd.setCursor(1,1);
          lcd.write(byte(2));     

          //pintar persona arriba       
          salta = 1;
          lcd.setCursor(1,0);
          lcd.write(byte(0)); 
          Serial.print(posPiedra);

          
          delay(150);
          lcd.setCursor(posPiedra, 1);
          lcd.write(byte(1)); 
          lcd.write(byte(2)); 
          delay(150);
                               
          //eliminar persona arriba
          lcd.setCursor(1,0);
          lcd.write(byte(2));
               
          lcd.setCursor(posPiedra--, 1);
          lcd.write(byte(1)); 
          lcd.write(byte(2)); 
          
          //bajar 
          lcd.setCursor(1,1);
          lcd.write(byte(0));
          
          salta = 0;
          pulsado = 1;

        }
      }
      prevSwitchState = switchState;
    }
    
}

void run(){
  delay(100);
  lcd.setCursor(0, 1);
  lcd.write(byte(2)); 
  
  for(int i = 15; i >= 0; i--){
    if(i == 0){
      gameOver = 1;
      
      lcd.setCursor(0,1);
      lcd.print("pts: ");
      lcd.setCursor(5,1);
      lcd.print(puntos);
      lcd.setCursor(13, 0);
      lcd.print("   ");
      delay(500);
     
      break;
    
    }
    
    posPiedra = i;
    
    for(int j = 0; j < 300; j++){ //Tiempo de espera antes de mover piedra
      salto();
      delay(1);
     
      if(pulsado == 1){
        j = 300;        
      }
    }
    
    if(pulsado == 1){
      pulsado = 0;
      if(correcto == 1){
        puntos++;
        correcto = 0;
        lcd.setCursor(13, 0);
        lcd.print(puntos);
        break;
      }

    } else {
      lcd.setCursor(i, 1);
      lcd.write(byte(1)); 
      lcd.write(byte(2));       
    }
    
    
    
          
  
  
   
  }

}
