#include <EEPROM.h>

#define bomba  13 // Pino que ativa a bomba
#define motor  12 // Pino que ativa o motor

#define led_nivel_1  11 // Pino que o led1 (Nivel 1) (vermelho )
#define led_nivel_2  10 //Pino que o led2 (Nivel 2) (vermelho)
#define led_nivel_3  9 //Pino que o led3 (Nivel 3) (vermelho)

#define led_modo_1  8  // Pino que o led1 (Nivel Leve) (branco)
#define led_modo_2  7  //Pino que o led2 (Nivel Normal) (branco)
#define led_modo_3  6  //Pino que o led3 (Nivel Pesado) (branco)

#define button_start_system 5 // Pino que verifica se o sistema pode iniciar
#define button_seleciona_nivel 2 // Pino para entrada do nivel desejado
#define button_seleciona_modo 3 // Pino para entrada do modo desejado

int incomingByte;      // a variable to read incoming serial data into
int buttonState = 0 ; // botao comeca desligado
int addr = 0; // Posicao Inicial da Memoria
const int LM35 = A0; // Pino que verifica o nivel de agua
float temperatura; // Variável que armazenará a temperatura medida
int   saida;

int nivel = 0;
int modo = 0;

typedef struct Estados{
     int modo_operacao;
     int nivel_agua;
     int dia;
     int mes;
     int ano;
}Estado;

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

void ligar_bomba(int nivel_agua){

      
      switch (nivel_agua) {
          
          case 0: // Tirar a agua
                analogWrite(bomba, 254);
                while(analogRead(LM35) > 1){  // Maior que 0
                      temperatura = analogRead(LM35);
                    Serial.print("Tirando agua: ");
                    Serial.println(temperatura);
                }
                analogWrite(bomba, 0);
                break;         
          
          case 1: // Colocar agua até o nivel1
                analogWrite(bomba, 254);
                while(analogRead(LM35) < 360){  // Menor que 35% 
                      temperatura = analogRead(LM35);
                    Serial.print("Nivel1: ");
                    Serial.println(temperatura);
                }
                analogWrite(bomba, 0);
                break;
                
          case 2: // Colocar agua até o nivel2
                analogWrite(bomba, 254);
                while(analogRead(LM35) < 614){ // Menor que 60%                  
                    temperatura = analogRead(LM35);
                    Serial.print("Nivel2: ");
                    Serial.println(temperatura);
                }
                analogWrite(bomba, 0);
                break;
          case 3: // Colocar agua até o nivel3          
                analogWrite(bomba, 254);
                while(analogRead(LM35) < 1023){                  
                    temperatura = analogRead(LM35);
                    Serial.print("Nivel3: ");
                    Serial.println(temperatura);
                }
                analogWrite(bomba, 0);
                break;
        }

        delay(1000);
      
}

void ligar_motor(int tempo){

    tempo = tempo * 1000;
        
    digitalWrite(motor, HIGH);
    Serial.println("Motor Ligado");
    delay(tempo);
    Serial.println("Motor Desligado");
    digitalWrite(motor, LOW);
  }
   
void modoRapido(){
 
  // Ligar a bomba para colocar agua (Nivel);
  ligar_bomba(nivel); // Passando nivel 1 de agua
  ligar_motor(5); // Passando 5 minutos
  Serial.println("Molho por 5 segundos");
  delay(5000); // Molho
  ligar_motor(5); // Passando 5 minutos 
  ligar_bomba(0);// Ligar a bomba para tirar agua; (Tirando agua suja)
  ligar_bomba(nivel);// Ligar a bomba para colcoar agua(Nivel); (Colocando agua limpa)
  ligar_motor(5); // Ligar o motor; (Enxaguando)
  ligar_bomba(0);// Ligar a bomba para tirar agua; (Tirando agua enxague)
  Serial.println("Centrifugando");
  ligar_motor(5); // Ligar o motor; (Centrifugando)
  Serial.println("Roupas Lavadas no modo Rapido");
  delay(5000);
   
} 


boolean modoNormal(){
    
    ligar_bomba(nivel); // Passando nivel Selecionado de agua
    ligar_motor(10); // Ligar motor por 10 minutos
    delay(5000); // Molho por 5 min
    Serial.println("Molho por 5 segundos");
    ligar_motor(10); // Ligar motor por 10 minutos
    delay(5000); // Molho por 5 min
    ligar_bomba(0);// Ligar a bomba para tirar agua; (Tirando agua suja)
    ligar_bomba(nivel);// Ligar a bomba para colcoar agua(Nivel); (Colocando agua limpa)
    ligar_motor(5); // Ligar motor por 5 minutos
    ligar_bomba(nivel);// Ligar a bomba para tirar agua; (Tirando agua suja)
    Serial.println("Centrifugando por 10 segundos");
    ligar_motor(10); // Ligar o motor; (Centrifugando)
    Serial.println("Roupas Lavadas no modo Normal");
 
 }


 boolean modoPesado(){
    
    ligar_bomba(nivel); // Passando nivel 1 de agua
    ligar_motor(15); // Ligar motor por 15 minutos
    Serial.println("Molho por 10 segundos");
    delay(10000); // Molho
    ligar_motor(15); // Ligar motor por 15 minutos
    Serial.println("Molho por 10 segundos");
    delay(10000); // Molho
    ligar_bomba(0);// Ligar a bomba para tirar agua; (Tirando agua suja)
    ligar_bomba(nivel);// Ligar a bomba para colcoar agua(Nivel); (Colocando agua limpa)
    ligar_motor(5); // Ligar motor por 5 minutos
    ligar_bomba(0);// Ligar a bomba para tirar agua; (Tirando agua suja)
    ligar_bomba(nivel);// Ligar a bomba para colcoar agua(Nivel); (Colocando agua limpa)
    ligar_motor(5); // Ligar motor por 5 minutos
    ligar_bomba(0);// Ligar a bomba para tirar agua; (Tirando agua suja)
    Serial.println("Centrifugando por 15 segundos");
    ligar_motor(15); // Ligar o motor; (Centrifugando)
    Serial.println("Roupas Lavadas no modo Pesado");
 
 }
void ligar_Led_Nivel(){

  if(nivel == 0){
    Serial.println("Nivel 0");
    digitalWrite(led_nivel_1, LOW);
    digitalWrite(led_nivel_2, LOW);
    digitalWrite(led_nivel_3, LOW);
  } 
  if(nivel == 1){
    Serial.println("Nivel 1");
    digitalWrite(led_nivel_1, HIGH);
    digitalWrite(led_nivel_2, LOW);
    digitalWrite(led_nivel_3, LOW);
  }
  if(nivel == 2){
    Serial.println("Nivel 2");
    digitalWrite(led_nivel_1, HIGH);
    digitalWrite(led_nivel_2, HIGH);
    digitalWrite(led_nivel_3, LOW);
  }
  if(nivel == 3){
    Serial.println("Nivel 3");
    digitalWrite(led_nivel_1, HIGH);
    digitalWrite(led_nivel_2, HIGH);
    digitalWrite(led_nivel_3, HIGH);
  }
  
}

void ligar_Led_Modo(){

  if (modo == 0){
    Serial.println("Modo Desligado");
    digitalWrite(led_modo_1, LOW);
    digitalWrite(led_modo_2, LOW);
    digitalWrite(led_modo_3, LOW);
    }
  if(modo == 1){
    Serial.println("Modo Rapido");
    digitalWrite(led_modo_1, HIGH);
    digitalWrite(led_modo_2, LOW);
    digitalWrite(led_modo_3, LOW);
  }
  
  if(modo == 2){
    Serial.println("Modo Normal");
    digitalWrite(led_modo_1, HIGH);
    digitalWrite(led_modo_2, HIGH);
    digitalWrite(led_modo_3, LOW);
  } 
  
  if(modo == 3){
    Serial.println("Modo Pesado");
    digitalWrite(led_modo_1, HIGH);
    digitalWrite(led_modo_2, HIGH);
    digitalWrite(led_modo_3, HIGH);
  }
  
}

void selecionarNivel(){
  if (buttonState != HIGH){
  nivel++;
  ligar_Led_Nivel();
  
  if(nivel > 3){
      nivel = 0; 
    }
  }else{
      Serial.println("Não é possivel selecionar nivel apos sistema iniciado");
    }
}

void selecionarModo(){
  if (buttonState != HIGH){
  modo++;
  ligar_Led_Modo();
  if (modo > 3){
    modo = 0;  
  }
  }else{
      Serial.println("Não é possivel selecionar modo apos sistema iniciado");
    }
  
}

void salvarEstado(){
        
    EEPROM.write(addr, modo);
    EEPROM.write(addr++, nivel);
    EEPROM.write(addr++, 28);
    EEPROM.write(addr++, 03);
    EEPROM.write(addr++, 17);
   if (addr == 512) {
    addr = 0;
   }
}
void historico(){
  
  int estado;
  int address = 0 ;
  
  Serial.println("\t\t------------------ HISTORICO---------------------------");
  
  while(address < addr){
        estado = EEPROM.read(address);
        Serial.println(" --------------------");
        if(estado == 1){ Serial.println("|Modo Rapido       |");}
        if(estado == 2){ Serial.println("|Modo Normal       |");}
        if(estado == 3){ Serial.println("|Modo Pesado       |");}
        Serial.print("|Nivel de agual: "); Serial.print(EEPROM.read(address++));Serial.println(" |");
        Serial.print("|Data: "); Serial.print(EEPROM.read(address++));
        Serial.print("/"); Serial.print(EEPROM.read(address++));
        Serial.print("/"); Serial.print(EEPROM.read(address++)); Serial.println("     |");
        Serial.println(" --------------------");
    }
}
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
  pinMode(button_start_system, INPUT);
  pinMode(button_seleciona_nivel, INPUT);
  pinMode(button_seleciona_modo, INPUT);

  attachInterrupt(digitalPinToInterrupt(2), selecionarNivel, RISING);  //Selecionar Nivel
  attachInterrupt(digitalPinToInterrupt(3), selecionarModo, RISING); // Selecionar Modo
}

void loop() {

  
 buttonState = digitalRead(button_start_system);

 if (Serial.available() > 0) {
    
    incomingByte = Serial.read();
    if (incomingByte == 'H') {
        historico();
    }
  
  }
 
 if (buttonState == HIGH){
    Serial.println("System ON");
    switch(modo){
      
      case 1: Serial.println("Modo Rapido Selecionado");
              //modoRapido();
              salvarEstado();
              break;
      case 2: Serial.println("Modo Normal Selecionado");
              //modoNormal();
              salvarEstado();
              break;
      case 3: Serial.println("Modo Pesado Selecionado");
              //modoPesado(); // Salvar Estado
              salvarEstado();
              break;
      default:  Serial.println("Nenhum Modo Selecionado");
                break;
   }

   buttonState = LOW;
 
 }
  delay(50);
 
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
