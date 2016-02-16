/*
*****************************************
*        CALIBRACION DEL TECLADO        *
*      Y CARGA DEL TECLADO              *
*****************************************
*/


void Cargar_teclado()
{
  if( digitalRead(calibrar) == HIGH )
  {
    calibrar_teclado();
  }
  else
  {
    for(int i= 0; i<cantTec;i++)
    {
      teclado.addKey(leer_EEPROM(),t[i]); //recorre todas las posiciones del vector de caracteres y los agrega como teclas
    }
  }
}
void calibrar_teclado()
{
  lcd.clear();
lcd.home();
  lcd.print("  CALIBRACION   ");
  delay(200);
  for( int i = 0 ; i<cantTec; i++)
  {
    cargar_tecla(t[i]);
  }
}

void cargar_tecla(char nombre)
{
lcd.home();
  lcd.clear();
  lcd.print("Presione tecla:");
  lcd.print(nombre);
  lcd.setCursor(0,1);
  lcd.print("durante 2 seg   ");
  delay(3000);
  int valor = analogRead(Tec_PIN);
  if ( valor > 50 )
  {
    teclado.addKey(valor,nombre);
    grabar_EEPROM(valor);
    delay(1000);
  }
}
