#include <EEPROM.h>

#define bomba =  13; // Pino que ativa a bomba
#define motor =  12; // Pino que ativa o motor

#define led_nivel_1 =  11; // Pino que o led1 (Nivel 1)
#define led_nivel_2 =  10; //Pino que o led2 (Nivel 2)
#define led_nivel_3 =  9; //Pino que o led3 (Nivel 3)

#define led_modo_1 =  8; // Pino que o led1 (Nivel Leve)
#define led_modo_2 =  7; //Pino que o led2 (Nivel Normal)
#define led_modo_3 =  6; //Pino que o led3 (Nivel Pesado)

#define button_seleciona_nivel = 5; // Pino para entrada do nivel desejado
#define button_seleciona_modo = 4; // Pino para entrada do modo desejado
int addr = 0; // Posicao Inicial da Memoria
const int LM35 = A0; // Pino que verifica o nivel de agua
float temperatura; // Variável que armazenará a temperatura medida
int   saida;

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  
  Serial.begin(9600); // inicializa a comunicação serial
  
  // Inicialia os pinos de saida
  pinMode(bomba, OUTPUT);
  pinMode(motor, OUTPUT);
    
  pinMode(led_nivel_1, OUTPUT);
  pinMode(led_nivel_2, OUTPUT);
  pinMode(led_nivel_3, OUTPUT);
  
  pinMode(led_modo_1, OUTPUT);
  pinMode(led_modo_2, OUTPUT);
  pinMode(led_modo_3, OUTPUT);

//Inicializa os pinos de entrada
  pinMode(button_seleciona_nivel, INPUT);
  pinMode(button_seleciona_modo, INPUT);
}

void lerEprom(){

  
  int address = 0;
  byte value;

  while (address >= EEPROM.length()){
  
  value = EEPROM.read(address);

  Serial.print(address);
  Serial.print("\t");
  Serial.print(value, DEC);
 

  // avance ao próximo endereço da EEPROM
  address = address + 1;


  // há apenas 512 bytes na EEPROM, de 0 a 511, de forma que se
  // estivermos no endereço 512, meia-volta ao 0
  if (address == EEPROM.length())
    {
      address = 0;
    }
  }
}

boolean ligar_bomba(char nivel){
        
        switch(nivel){
          
          case '0': // Tirar a agua
                digitalWrite(bomba, HIGH);
                while(analogRead(LM35) > 0){  // Maior que 0
                      temperatura = analogRead(LM35);
                    Serial.print("Valor: ");
                    Serial.println(temperatura);
                }
                digitalWrite(bomba, LOW);
                return true;
                break;         
          
          case '1': // Colocar agua até o nivel1
          
                digitalWrite(bomba, HIGH);
                while(analogRead(LM35) < 360){  // Menor que 35% 
                      temperatura = analogRead(LM35);
                    Serial.print("Valor: ");
                    Serial.println(temperatura);
                }
                digitalWrite(bomba, LOW);
                return true;
                break;
                
          case '2': // Colocar agua até o nivel2
                digitalWrite(bomba, HIGH);
                while(analogRead(LM35) < 614){ // Menor que 60%                  
                    temperatura = analogRead(LM35);
                    Serial.print("Valor: ");
                    Serial.println(temperatura);
                }
                digitalWrite(bomba, LOW);
                return true;
                break;
          case '3': // Colocar agua até o nivel3          
                digitalWrite(bomba, HIGH);
                while(analogRead(LM35) <= 1023){                  
                    temperatura = analogRead(LM35);
                    Serial.print("Valor: ");
                    Serial.println(temperatura);
                }
                digitalWrite(bomba, LOW);
                return true;
                break;
        }
      
}
void ligar_motor(int tempo){
    digitalWrite(motor, HIGH);
    // conta 5 minutos
    digitalWrite(motor, LOW);
  }
   
boolean modoRapido(){
 
  // Ligar a bomba para colocar agua (Nivel);
  ligar_bomba('1'); // Passando nivel 1 de agua
  ligar_motor(5); // Passando 5 minutos
  delay(1000); // Molho
  ligar_motor(5); // Passando 5 minutos 
  ligar_bomba('0');// Ligar a bomba para tirar agua; (Tirando agua suja)
  ligar_bomba('1');// Ligar a bomba para colcoar agua(Nivel); (Colocando agua limpa)
  ligar_motor(5); // Ligar o motor; (Enxaguando)
  ligar_bomba('0');// Ligar a bomba para tirar agua; (Tirando agua enxague)
  ligar_motor(5); // Ligar o motor; (Centrifugando)
  return true;
  
} 
boolean modoNormal(){
    
    ligar_bomba('1'); // Passando nivel 1 de agua
    ligar_motor(10); // Ligar motor por 10 minutos
    delay(5000); // Molho por 5 min
    ligar_motor(10); // Ligar motor por 10 minutos
    delay(5000); // Molho por 5 min
    ligar_bomba('0');// Ligar a bomba para tirar agua; (Tirando agua suja)
    ligar_bomba('1');// Ligar a bomba para colcoar agua(Nivel); (Colocando agua limpa)
    ligar_motor(5); // Ligar motor por 5 minutos
    ligar_bomba('0');// Ligar a bomba para tirar agua; (Tirando agua suja)
    ligar_motor(10); // Ligar o motor; (Centrifugando)
    return true;
 }

 boolean modoPesado(){
    
    ligar_bomba('1'); // Passando nivel 1 de agua
    ligar_motor(15); // Ligar motor por 15 minutos
    delay(10000); // Molho por 10 min
    ligar_motor(15); // Ligar motor por 15 minutos
    delay(10000); // Molho por 10 min
    ligar_bomba('0');// Ligar a bomba para tirar agua; (Tirando agua suja)
    ligar_bomba('1');// Ligar a bomba para colcoar agua(Nivel); (Colocando agua limpa)
    ligar_motor(5); // Ligar motor por 5 minutos
    ligar_bomba('0');// Ligar a bomba para tirar agua; (Tirando agua suja)
    ligar_bomba('1');// Ligar a bomba para colcoar agua(Nivel); (Colocando agua limpa)
    ligar_motor(5); // Ligar motor por 5 minutos
    ligar_bomba('0');// Ligar a bomba para tirar agua; (Tirando agua suja)
    ligar_motor(15); // Ligar o motor; (Centrifugando)
    return true;
 
 }


void loop() {
  // read the state of the pushbutton value:
  //buttonState = digitalRead(buttonPin);

  modoRapido();
 delay(1000);
    /*  
      // turn LED on:
      digitalWrite(ledPin, HIGH);
      temperatura = (float(analogRead(LM35))*5/(1023))/0.01;
      Serial.print("Temperatura: ");
      Serial.println(temperatura);
      EEPROM.write(addr, temperatura);
      
      // see if there's incoming serial data:
      if (Serial.available() > 0){
        
        // read the oldest byte in the serial buffer:  
        saida = Serial.read();  
          // if it's a capital H (ASCII 72), chama a funcao lerEprom:
        if (saida == 'H')
          {
              lerEprom();
              delay(500);
          }
      
      }
      
      addr = addr + 1;
      delay(2000);
      // turn LED off:
      digitalWrite(ledPin, LOW);
  delay(1000);
 */
}
