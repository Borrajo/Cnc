/*
  ************************************
  *            CODIGO G              *
  *                                  *
  ************************************
  *        Lista de Comandos         *
  ************************************
 
 
//  PASOS A SEGUIR EN EL ARCHIVO EXCELLON //
  Comienza con %
  define los parametros iniciales 
  define las mechas
  cierra con %
  Solicita mecha n
  Comienza con los agujeros
  Termina con M30
  Vuelve a (0,0,0)

//  COMANDOS  //
  M48 : COMIENZA el programa
  M71 : medido en milimetros
  M72 : medido en pulgadas
  M30 : FINALIZA el programa

//  MECHAS  //
  T01 C1.00
  T0x CY.YY  ;  x es el numero de mecha
             ;  Y es la medida de mecha, esta medida se muestra en el LCD
  luego solo indica el nombre de la mecha
  T01
  T02
  ...
  Tn
*/
