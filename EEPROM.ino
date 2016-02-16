/*
***************************************
*   LECTURA y ESCRITURA de EEPROM     *
*               ----                  *
* Se usa para escribir el valor del   *
* divisor de tension a fin de no tener*
* que calibrar el teclado cada vez que*
* se inicia el CNC                    *
***************************************
*/


void grabar_EEPROM(int valor )
{
  static int direccion = 0;
  int dir = direccion;
  while(valor > 255)
  {
    EEPROM.write(dir,255);
    valor -= 255;
    dir++;
  }
  EEPROM.write(dir,valor);
  direccion += 4;
}

int leer_EEPROM()
{
  static int dir = 0;
  int valor = 0;
  for(int i = dir; i < (dir+4); i++)
  {
    valor += EEPROM.read(i);
  }
  dir +=4;
  return valor;
}
