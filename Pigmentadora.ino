#include  <SparkFun_TB6612.h>

//Configuracion de Motor
//pines de conexion del motor al Driver
#define A_IN1 2
#define A_IN2 4
#define A_PWM 5 //esta pata tiene que tener PWM
#define STBY 9
//lado de giro del motor
const char offsetA = 1;
//los motores aceptan Valores de -255 a 255 Velocidad Left,Right
//la direcion inicial esta dada por la Variable offsetA o offsetB
int VelocityMotorA=0;
//Variable que maneja el tiempo que mantiene el motor prendido despues
//del flanco descendente y Funciona en MiliSegundos
double timeA=0;
//inicio del objeto/clase motor
Motor MotorA = Motor(A_IN1, A_IN2, A_PWM, offsetA, STBY);

//Si se utiliza el lado B del Driver descomentar
//#define B_IN1 7
//#define B_IN2 8
//#define B_PWM 6
//const char offsetB = 1;
//int VelocityMotorA=0;
//Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

//definiciones de entrada de Señal o SignalOn
//LEANDRO LEE ESTO
//LEANDROOOOOOOOO LEE ESTOOOOOOOOOOOOOO AAAAAAAAAAAAAAAAaaaaaHhhh
//sono a gemido Uwu (?
#define SignalOn 1 //LEANDROOOOOOOOO cambia esto porque no tengo ni idea que pin vas a usar xd igual que los de arriba
bool SignalOnAux=FALSE;

//LEAN cambia esto por si lo queres , para poner una luz o indicador de inicio del proceso
//si lo borras, borra el codigo donde se usa.
#define SignalOut 10

//PD: Las Variables que tenes que configurar son:
// VelocityMotorA
// offsetA
// timeA

void setup() {
  pinMode(SignalOn,INPUT);
  pinMode(SignalOut,OUTPUT);
}

void loop() {
  if(digitalRead(SignalOn)){
    MotorA.drive(VelocityMotorA);
    digitalWrite(SignalOut,HIGH);

    SignalOnAux = TRUE;
  }else if(SignalOnAux==TRUE) { //creo detector de flanco descendente
    MotorA.drive(VelocityMotorA,timeA);
    digitalWrite(SignalOut,LOW);

    SignalOnAux = FALSE;
  }
}
