#include <EEPROM.h>


// o endereço atual na EEPROM (isto é, é a posição do
// byte que gravaremos dentro da EEPROM)
int addr = 0;

typedef struct {
    boolean motor_on_off; // 0 - Motor Desligado/1 - Motor Ligado
    int tempo; // Tempo que o motor ficara ligado
  }modo_rapido;
  
const int buttonPin = 2;     // the number of the pushbutton pin
const int bomba =  13;      // Pino que ativa a bomba
const int motor =  12;      // Pino que ativa o motor

const int LM35 = A0; // Pino que verifica o nivel de agua
float temperatura; // Variável que armazenará a temperatura medida
int   saida;

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  
  Serial.begin(9600); // inicializa a comunicação serial
  
  // initialize the LED pin as an output:
  pinMode(bomba, OUTPUT);
  pinMode(motor, OUTPUT);
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
   
void modoRapido(){
 
  // Ligar a bomba para colocar agua (Nivel);
  ligar_bomba('1'); // Passando nivel 1 de agua
  ligar_motor(5); // Passando 5 minutos
  ligar_motor(5); // Passando 5 minutos 
  ligar_bomba('0');// Ligar a bomba para tirar agua; (Tirando agua suja)
  ligar_bomba('1');// Ligar a bomba para colcoar agua(Nivel); (Colocando agua limpa)
  ligar_motor(5); // Ligar o motor; (Enxaguando)
  ligar_bomba('0');// Ligar a bomba para tirar agua; (Tirando agua enxague)
  ligar_motor(5); // Ligar o motor; (Centrifugando)
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
