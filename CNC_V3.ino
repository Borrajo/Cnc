/*
*********************************************
*  Esta es La nueva version Limpia del CNC  *
*  Esperemos que funcione.                  *
*********************************************

*********************************************
*      MAQUINA de CNC para perforacion      *
*       Fecha de inicio : 18/04/2013        *
*********************************************

*********************************************
*       Autor: Manuel Ignacio Borrajo       *
*         Diseño para Arduino UNO           *
*********************************************
*/
#include <motor.h>
#include <LcdCore.h>
#include <Lcd74HC164.h>
#include <Teclado.h>
#include <EEPROM.h>
#include "Configuracion.h"

//    TECLADO    //
#define Tec_PIN A3
#define cantTec 16
#define calibrar A4 

//    TECLADO    //
Teclado teclado(Tec_PIN,cantTec);
char t[cantTec] = {Cero,Uno,Dos,Tres,Cuatro,Cinco,Seis,Siete,Ocho,Nueve,Ast,Num,Menu,Stay,Away,Select}; //este es el vector con las teclas, es mas facil cargar asi el teclado y calibrarlo.

// Creacion de los 3 motores   //
Motor Eje_X(PPR_X,2,3);     //inicializo mi Stepper (motor), es un objeto :  motor eje x (pasos por revolucion, int _pinDir , int _pinStep )
Motor Eje_Y(PPR_Y,4,5); 
Motor Eje_Z(PPR_Z,6,7);

//    LCD    //
Lcd74HC164 lcd (DOUT_PIN, CLK_PIN,EN_PIN);

//Variables Globales
long posx ;     // posicion X que se actualiza a medida que transcurre el programa           
long posy ;     // posicion Y que se actualiza a medida que transcurre el programa
long posz ;
int cant_hoyos; // es la cantidad de hoyos total que tiene el proyecto y sirve para hacer el porcentaje.

// Vector con las medidas de mechas 
String mechas[CANT] ;
//Esta variable queda en falso hasta que termine de cargarse las mechas
boolean configOk = false;
//Indica si es en mm o pulgadas
String unidad ;

void setup()
{
  Serial.begin(115200);    //inicio el puerto serial y le pongo la velocidad de comunicacion
  Eje_X.setSpeed(SPEED_MX);
  Eje_Y.setSpeed(SPEED_MY);
  Eje_Z.setSpeed(SPEED_MZ);
  pinMode(sensorX, INPUT);    //el boton de fin de carrera del ejeX
  pinMode(sensorY, INPUT);    //el boton de fin de carrera del ejeY
  pinMode(sensorZ, INPUT);    //el boton de fin de carrera del ejeZ
  lcd.begin(16,2);
  lcd.noCursor();
  Presentacion_lcd();
  Modo_manual();
  //Cargar_teclado();
  Inicializar_posiciones();
  cant_hoyos = Pedir_cant()-5;
}

void Presentacion_lcd()
{
  lcd.setCursor(0,0);   //fila 1 columna 0
  lcd.print("ImprimeAgujeros ");
  lcd.setCursor(0,1);
  lcd.print("  cnc  V 4.0.0"); 
  delay(5000);
  lcd.clear(); 
}

void  Inicializar_posiciones()
{ 
  lcd.clear(); 
  lcd.home();   //fila 1 columna 0
  lcd.print("Posicionando...");
  lcd.setCursor(0,1);
  iniciar(Eje_X,sensorX,&posx);
  lcd.print(" X:OK ");
  iniciar(Eje_Y,sensorY,&posy);
  lcd.print("Y:OK ");
  iniciar(Eje_Z,sensorZ,&posz);
  lcd.print("Z:OK ");
  Eje_Z.step(ALTURA_Z);
  lcd.clear();
  lcd.print("     Lista");
}

//mueve los motores hasta llegar al (0,0)
void iniciar(Motor motor,int sensor,long *pos)
{
  while ( digitalRead(sensor) != HIGH ) // mientras el sensor este en off el motor se sigue moviendo
  {                
    motor.step(-1);
  }
  *pos = 0;
}

int Pedir_cant()
{
  Serial.print('P');
  delay(1500);
  while (!Serial.available()){}
  String dato = leerDato();
  int num = dato.toInt();
  if( num <= 0 )
  {
    return Pedir_cant();
  }
  return num;
}

  
void loop()
{ 
  Serial.print('1');                    //mandamos un dato para avisar qeu estamos listos
  if (Serial.available())          //esperar que haya datos en el puerto serial
  {     
   //es un String en el que se almacena la linea enviada por la pc o leida desde la SD. 
    String datoLeido = leerDato();
    Ejecutar (datoLeido);
    if(configOk){  Porcentar(); }
  } 
  delay(20);
}

String leerDato()
{
  String linea = "" ;   //es un String en el que se almacena la linea enviada por la pc o leida desde la SD.
  linea.reserve(30);     //reservo 30 caracteres(bytes) para que no pise ninguan otra informacion
  while(Serial.available())
  {
    linea += (char)Serial.read();
  }
  return linea;
}

//decodifica la linea de codigo de la pc y realiza la accion correpondiente.
void Ejecutar(String datoLeido){
  switch (datoLeido.charAt(0)) 
  {
  case 'T':    // T indica MECHA a colocar
     {
       if(configOk)
       { 
         Escribir_LCDT(datoLeido.charAt(2));      //Introduzca la mecha     OK->
       }else
       {
         cargarMecha(datoLeido);  
       }
       break;
     } 
  case 'M':    // M indica COMANDO
     {
       Procesar_Comando(datoLeido);
       break;
     }
  case 'X':    // X indica POSICION
     {
       Procesar_Posicion(datoLeido);
       delay(500);  //Esperamos medio segundo para que la maquina se estabilice luego de posicionarse
       Hacer_agujero();
       break;
     }
   case '%':
   {
     configOk = true;
     break;
   }
   default :
    break;
   }
}


void cargarMecha(String dato)
{
  static int i = 1 ;
  mechas[i] = dato.substring(dato.indexOf('C')+1,dato.length());
  i++;
}

void Procesar_Comando(String dato)
{
  int codigo = (dato.substring(dato.indexOf('M')+1,dato.length())).toInt();
  switch(codigo)
  {
    case 48 :
    {
      Escribir_LCDM(codigo);
      break;
    }
    case 71 :
    {
      unidad = MM;
      break;
    }
    case 72 :
    {
      unidad = INCH;
      break;
    }
    case 30 :
    {
      Escribir_LCDM(codigo);
      Serial.end();
      break;
    }
    default:break;
  }
}

//Escribe en el lcd cuando termina
void Escribir_LCDM(int codigo)
{
  lcd.clear();
  lcd.home();
  lcd.println(" El CNC ha      ");
  lcd.setCursor(0,1);
  switch (codigo)
  {
    case 48 :
    { 
      lcd.println("   comenzado    ");
      break;
    }
    case 30 :
    {
      lcd.println("   terminado    ");
      break;
    }
    default:break;
  }
  delay(1000);
}


//Escribe en el lcd que debes cambiar la mecha  
void Escribir_LCDT(char n)
{
  lcd.home();
  lcd.clear();
  lcd.print("Introduzca la   ");
  lcd.setCursor(0,1);
  lcd.print("mecha : ");
  lcd.print(mechas[n -'0']);
  lcd.print(unidad);
  //lcd.print("    OK ");
  //lcd.write(199);
  
  /*while(teclado.getKey()!= Select )
  {  }*/
  delay(3000);
  lcd.clear();
}

//Escribe en el lcd las posiciones de X e Y 
void Escribir_LCDP()
{
  lcd.setCursor(0, 1);
  lcd.print("pos X");
  lcd.print(posx);
  lcd.print(" Y");
  lcd.print(posy);
  lcd.print("        ");
}

//Convierte el vector en posiciones validas para motores
void Procesar_Posicion(String linea)
{
  Escribir_LCDP();
  String X = linea.substring(linea.indexOf('X')+1,linea.indexOf('Y'));
  String Y = linea.substring(linea.indexOf('Y')+1,linea.length());
  int x = X.toInt();
  int y = Y.toInt();
  Mover_Motores(x,y);
}

void Mover_Motores(long posX , long posY)
{
    long PosNuevaX = posX;     //se pone la posicion absoluta nueva en la variable
    long PosNuevaY = posY;     // aca igual 
    long posRx = PosNuevaX - posx;           //acutal menos anterior real
    long posRy = PosNuevaY - posy;              //la POSR es la posicion relativa, lo que debe moverse el motor
    posx = PosNuevaX;              //ahora actualizamos la posicion actual. 
    posy = PosNuevaY; 
    long pasos_X = posRx * (PPR_X/D_VUELTA_X) / 100;   //     PPR_X/D_VUELTA_X = cantidad de pasos que se necesitan para hacer 1 mm de recorrido 
    long pasos_Y = posRy * (PPR_Y/D_VUELTA_Y) / 100;   //     PPR_Y/D_VUELTA_Y = cantidad de pasos que se necesitan para hacer 1 mm de recorrido 
    mover(pasos_X,pasos_Y);
    // el valor que proviene del archivo debe multiplicarse por la cantidad de pasos por mm propia de la varilla y despues dividir por 100 y tener el valor entero de eso
}

//Calcula cuantos agujeros faltan e indica el porcentaje.
void Porcentar()
{
  static int cont = 0;
  lcd.home();
  lcd.print(cont * 100.0 / cant_hoyos);
  lcd.print("% ");
  cont++;
}

//motor en Z, baja una cierta cantidad de tiempo y vuelve a subir 
void Hacer_agujero()
{
  // -1 * ( distancia de elevacion inicial + espesor de placa + punta de mecha ) 
  // eso debe moverse la mecha
    Eje_Z.step( (ESPESOR_PLACA*2 + PUNTA_MECHA )*-PPR_Z/D_VUELTA_Z  );
    delay(200);
    Eje_Z.step( (ESPESOR_PLACA*2 + PUNTA_MECHA )* PPR_Z/D_VUELTA_Z  ); 
}

//mueve a los dos motores "simultaneamente"
void mover(int posRx, int posRy )
{
  while( abs(posRx)!= 0 && abs(posRy) != 0)   // mientras haya pasos en algun motor 
  {
    Eje_X.step( (posRx/abs(posRx)) );         //si posRx es negativo dividido su valor absoluto, da -1, mueve un paso en la direccion correspondiente.
    Eje_Y.step( (posRy/abs(posRy)) );         // si la posicion relativa es positiva dividido su valor absoluto, da 1. mueve un paso en la direccion correspondiente
    posRx = (abs(posRx) - 1) * (posRx/abs(posRx)) ; //tomamos el valor absoluto de la posicion y la disminuimos en 1, y luego multiplicamos por 1 o -1 segun corresponda.
    posRy = (abs(posRy) - 1) * (posRy/abs(posRy)) ;
  }
    Eje_X.step(posRx);  // luego movemos los dos motores con los pasos que le falten, un motor siempre va a tener 0 pasos, ese no se moverá.
    Eje_Y.step(posRy);
}

