#include <L298N.h>   //libreria motor
#include <LiquidCrystal.h>

//MOTOR A PINES DE SALIDA Y ENTRADA
#define IN1_A 0   //SALIDA  IN1 pin
#define IN2_A 0   //SALIDA  IN2 pin
#define ENA_A 0  //SALIDA  ENA-PWM (Sirve Para Velocidad del Motor por PWM)

//Pines del LCD ni idea cuales son
#define RS 0
#define E  0
#define D4 0
#define D5 0
#define D6 0
#define D7 0

#define SIGNAL 0                  //pin señal de inicio
#define POT_VELOCITY 0            //fijate pin PMW
#define POT_TIME 0                //fiajte pin PWM
//tiempo el cual tarda en apagarse el motor
#define TIME_MIN 0                //tiempo minimo en el motor en MS
#define TIME_MAX 3000             //tiempo maximo en el motor en MS
unsigned long POT_TIME_VAR = 0;   //variablea modificar
unsigned int POT_VELOCITY_VAR=0;  //variable para la velocidad del motor

void LCDFunt(void);   //funcion que llama para el codigo del LCD

void motorFunt(void); //funcuin que llama para el codigo del motor
void motorFuntEnd(void);  //funcion que llama luego de terminar el delay o tiempo

L298N motorA(ENA_A, IN1_A, IN2_A);
LiquidCrystal LCD(RS, E, D4, D5, D6, D7);

void setup() {
   pinMode(IN1_A,OUTPUT);                                                       // CONFIGURACION DE PINES DE SALIDA/ENTRADA
   pinMode(IN2_A,OUTPUT);                                                       // CONFIGURACION DE PINES DE SALIDA/ENTRADA
   pinMode(ENA_A,OUTPUT);                                                       // CONFIGURACION DE PINES DE SALIDA/ENTRADA
   // What is this ? What's it for?
   pinMode(SIGNAL,INPUT);                                                       // CONFIGURACION DE PINES DE SALIDA/ENTRADA //Supongo que pata de entrada de señal

   POT_VELOCITY_VAR =  analogRead(POT_VELOCITY);                                //lee el valor del pote en cada ciclo
                                                                                //la uso para actualizar el valor del LCD en "tiempo REAL"
   POT_TIME_VAR =  analogRead(POT_TIME);
   motorA.setSpeed(POT_VELOCITY_VAR>> 2);
   motorA.stop();

   LCD.begin(16, 2);                                                            // Inicia un LCD 16x02 (columnas,fila)
   LCD.noBlink();
   LCD.noCursor();
   LCD.clear();
}

void loop() {
  LCDFunt();
  motorFunt();
}

void LCDFunt(void){
  static bool LCD_REFRESH=1;
  if(POT_VELOCITY_VAR != analogRead(POT_VELOCITY)){
    POT_VELOCITY_VAR = analogRead(POT_VELOCITY);
    LCD_REFRESH = 1;
  }
  if(POT_TIME_VAR != analogRead(POT_TIME)){
    POT_TIME_VAR = analogRead(POT_TIME);
    LCD_REFRESH = 1;
  }
  if(LCD_REFRESH){
    LCD.clear();
    LCD.setCursor(0,0);
    LCD.print("Velocidad  %");
    LCD.setCursor(12,0);
    unsigned char aux_V = map(POT_VELOCITY_VAR, 0, 1023, 0, 100);
    LCD.print(aux_V);

    LCD.setCursor(0,1);
    LCD.print("Tiempo (mS)");
    LCD.setCursor(12,1);
    unsigned int aux_T = map(POT_TIME_VAR, 0, 1023, TIME_MIN, TIME_MAX);
    LCD.print(aux_T);
  }
}

void motorFunt(void){
  static bool SIGNAL_AUX = 0;

  if(digitalRead(SIGNAL)){                                                    //detecto la señal de entrada
    motorA.forward();                                                         //motor hacia adelante

    SIGNAL_AUX = 1;
  }else if(digitalRead(SIGNAL)==0 && SIGNAL_AUX==1){                          //detecto flanco decendente
    motorA.forwardFor(map(POT_TIME_VAR, 0, 1023, TIME_MIN, TIME_MAX),motorFuntEnd);

    SIGNAL_AUX = 0;
  }
}

void motorFuntEnd(void){
  motorA.reset();                                                               //necesario ya que el motor se establece en "don't move"
  motorA.setSpeed(POT_VELOCITY_VAR>>2);                                         //establece el valor del potenciometro lo shifteo 2 veces asi pasa de 10 a 8 bits
                                                                                //en y elimino los 2 bits menos significativos
  motorA.stop();                                                                //paro el motor
}
