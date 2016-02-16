
//   MODO MANUAL   /// 
/*
*********************************************
*       MODO MANUAL ~ MODO JOYSTICK ~       *
* Puede usarse en:                          *
* modo Manual :  Ingresando posiciones de   *
*                de X y de Y absolutas      *
* modo Joystick : con los botones del te-   *
*                -clado puede moverse el    *
*                eje X, Y y Z               *
* Funciona correctamente, no hay que        *
* hacer ninguna modificacion                *
*********************************************
*/
void Modo_manual() 
{
  if(teclado.getKey() == Menu )                          ///esta es la CONTRASEÑA para poder entrar al MODO MANUAL
  {                    
    lcd.home();(1,0);
    lcd.print("  Modo manual   ");
    delay(2000);
    lcd.home();
    lcd.print("1)  Coordenadas ");
    lcd.setCursor(0,1);
    lcd.print("4)       JoyPad ");
    while(teclado.getKey() != Stay )       
    { 
      switch (teclado.ulTecla()) 
      {
      case 1 :   ///Modo Coordenada 
         Modo_coord();
         break;
      case 4 :   ///Modo Joypad 
         Modo_joypad();
         break;
      default :
        break;
      }
    }
  }
}

long Pedir_pos(String eje){
 char cadena[16] ;
 String cad("");
 lcd.clear();
 lcd.home();
 lcd.print("ingrese Coord ");
 lcd.print(eje);
 lcd.setCursor(0,1);
 while( teclado.getKey() != 'C')
 {
   if(teclado.ulTecla() != '&' && teclado.ulTecla() != '*')
   { 
     cad += teclado.ulTecla();
     lcd.print(teclado.ulTecla());
     delay(250);
   }
 }
 cad.toCharArray(cadena,cad.length()+1);
 //Serial.println(cadena);
 return strtol(cadena,NULL,10);   // lo solucione de esta manera, CONCATENE los numeros como STRING ( espero que funciones ) y despues lo convierto a numero y lo retorno
}


void Siguiente_pos()
{                                                      /// aca se me arma un bucle in finito del que no puedo salir ( SOLUCIONADO ) 
   lcd.clear();                                              
   lcd.print("siguiente Pos ? ");
   lcd.setCursor(0,1);
   lcd.print(" ESC         OK ");
   while ( teclado.getKey() != '0')
   {
      if(teclado.ulTecla() == 'C')
        Modo_coord();
   }
}

void Modo_coord()                                      // sale del modo COORD cuando en la sigueinte_pos le decis que no queres otra pos mas y ahi se empiezan a cerrar y sale.
{ 
   while(teclado.getKey() != '0')
   {           
     if(teclado.ulTecla() == '1' || teclado.ulTecla() == 'C')
     {
       Mover_Motores(Pedir_pos("X"),Pedir_pos("Y"));
       Siguiente_pos();
     }
   }
}
    
void Modo_joypad()
{ 
  lcd.home();
  lcd.print("ESC para salir    ");
  while(teclado.getKey() != '0')
  { 
    delay(50);
    switch (teclado.ulTecla())
    { 
      case '2':                //<-----  +X
        Eje_X.step(1);
        posx++;
        break;
      case '3':                //<-----  +Z
        Eje_Z.step(1);
        break;
      case '4':                //<-----  -Y
        if( posy > 0 )
        {
          Eje_Y.step(-1);
          posy--;
        }
        break;
      case '6':                //<-----  +Y
        Eje_Y.step(1);
        posy++;
        break;
      case '8':                //<-----  -X
        if( posx > 0 )
        {
          Eje_X.step(-1);
          posx--;
          break;
        }
      case '9':                //<-----  -Z
        Eje_Z.step(-1);
        break;
      case 'A': {              //<-----  HOME
        Eje_X.step(-posx);
        Eje_Y.step(-posy);
        posx = 0; posy = 0;}
        break;
      default:                //<-----  OTRA TECLA
        break;   
    }
    Escribir_LCDP();    
    teclado.getKey();
  }
}
/* FIN DE MODO MANUAL*/
