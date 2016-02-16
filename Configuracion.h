/* Este archivo es para configurar 
   parametros principales del CNC. 
*/

//  Motores   //
#define PPR_X       48  //PASOS POR REVOLUCION
#define PPR_Y       48  //PASOS POR REVOLUCION
#define PPR_Z       48  //PASOS POR REVOLUCION

#define D_VUELTA_X  1.27   //Distancia realizada en una vuelta en X expresada en milimetros
#define D_VUELTA_Y  1.27   //Distancia realizada en una vuelta en X expresada en milimetros
#define D_VUELTA_Z  1.27   //Distancia realizada en una vuelta en X expresada en milimetros

// Velocidad de motores //
#define SPEED_MX 620 // REVOLUCIONES POR MINUTO DEL EJE X
#define SPEED_MY 620 // REVOLUCIONES POR MINUTO DEL EJE Y
#define SPEED_MZ 620 // REVOLUCIONES POR MINUTO DEL EJE Z

// Placa //
#define ALTURA_Z PPR_Z/D_VUELTA_Z*ESPESOR_PLACA/2  //luego de encontrar el 0, levanta al taladro una altura igual a la mitad del espesor de la placa
#define PUNTA_MECHA 0.4
#define PUNTA PPR_Z/D_VUELTA_Z*PUNTA_MECHA
#define ESPESOR_PLACA 2 //Espesor de la placa medido en milimetros.
#define CANT 12 //cantidad maxima de mechas


//  Sensores   //
#define sensorX    8   //pin en el que se encuentra el sensor de fin de carrera X 
#define sensorY    9   //pin en el que se encuentra el sensor de fin de carrera Y 
#define sensorZ    A5   //pin en el que se encuentra el sensor de fin de carrera Z 

#define altura 300  //Es la altura (en pasos) que se levanta el eje_Z  

//    LCD     //
#define LCD_LINES  2  // numero de filas
#define CLK_PIN   A0  // Clock pin
#define DOUT_PIN  A1  // Data pin
#define EN_PIN   A2  // Enable pin

//Definimos las medidas
#define MM "mm"
#define INCH "inch"

//Teclado
#define Uno '1'
#define Dos '2'
#define Tres '3'
#define Cuatro '4'
#define Cinco '5'
#define Seis '6'
#define Siete '7'
#define Ocho '8'
#define Nueve '9'
#define Cero '0'
#define Ast '*'
#define Num '#'
#define Menu 'M'
#define Stay 'S'
#define Away 'A'
#define Select 'E'



