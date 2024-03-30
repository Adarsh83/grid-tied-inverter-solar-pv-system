char keypadPort at PORTD; // initiaze the keypad port
sbit LCD_RS at RC2_bit;     // connect lcd rd into rc2 port on pic microcontroller
sbit LCD_EN at RC3_bit;         // connect lcd en into rc3 port on pic microcontroller
sbit LCD_D4 at RC4_bit;       // connect lcd d4 into rc4 port on pic microcontroller
sbit LCD_D5 at RC5_bit;    // connect lcd d5 into rc5 port on pic microcontroller
sbit LCD_D6 at RC6_bit;    // connect lcd d6 into rc6 port on pic microcontroller
sbit LCD_D7 at RC7_bit;      // connect lcd d7 into rc7 port on pic microcontroller
sbit LCD_RS_Direction at TRISC2_bit;
sbit LCD_EN_Direction at TRISC3_bit;
sbit LCD_D4_Direction at TRISC4_bit;
sbit LCD_D5_Direction at TRISC5_bit;
sbit LCD_D6_Direction at TRISC6_bit;
sbit LCD_D7_Direction at TRISC7_bit;

#define HEATER PORTB.RB0
#define FAN PORTB.RB1
#define ENTER 15
#define CLEAR 13
#define ON 1
#define OFF 0


void main()
{
 unsigned short kp,Txt[14];
 unsigned short Temp_Mea;
 unsigned char inTemp;
 unsigned int temp;
 float mV, ActualTemp;
 Keypad_Init();   // inititilize keypad
 ADRESH = 0;
 ADRESL = 0;
 ADCON1 = 0;
 TRISA0_Bit = 1;
 TRISC = 0;
 TRISD0_bit = 0;  // RB0 is heater
 TRISD1_bit=0;    // RB1 IS output (Heater)
   Keypad_Init();
   Lcd_Init();
   Lcd_Cmd(_LCD_CLEAR);    // clear lcd
   Lcd_Cmd(_LCD_CURSOR_OFF);   // cursor off in LCD
   Lcd_Out(1,1,"OSCILLATOR TIME");    // DISPLAY THE TEXT IN LCD
   Lcd_Out(2,2,"GRID INVERTER");
   delay_ms(1000);          // TIME OF DELAY

   HEATER = OFF;
   FAN = OFF;

   //On startup, read the reference temeprature from keypad
       START:
   Lcd_Cmd(_LCD_CLEAR);
   Lcd_Out(1, 1, "Automatic temeprature");
   Temp_Mea=0;
   Lcd_Out(2,2, "23 degree Celsius ");
   while(1)
   {
    do
     kp = keypad_Key_Click();
    while (!kp);

    if  (kp == ENTER) break;        // CLICK two times enter on keypad
    if (kp > 3 && kp <8) kp = kp-1;
    if (kp > 8 && kp < 12) kp = kp-2;
    if (kp == 14) kp =0;
    if (kp == CLEAR )goto START;
    Lcd_Chr_Cp(kp +'0');
    Temp_Mea = (10*Temp_Mea) + kp;
    }

    // clear the lcd using command function
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(2,2, "Temp measured: ");
    intToStr(Temp_Mea,Txt);      // Convert string into integer
    inTemp=Ltrim(Txt);       // display the lcd in text
    Lcd_Out(5,5,"Press 0 to Cont.");       // press 0 button of keypad to continue
   //Wait until 0 pressed
    kp=0;
    while(kp!=ENTER)
   {
    do
    kp = keypad_Key_Click();
    while(!kp);
   }
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1,1, " PWM:");
    Lcd_Chr(1,10,123);
    Lcd_Chr(1,11,'C');
     while(1) {
     //Display Referance Temperature and Actual Temperature
     temp = ADC_Read(0);               //Read temperature from AN0
     mV = temp * 5000.0/1024.0;        //Convert to mV
     ActualTemp = mV/20.0 ;              // Convert to degrees Celcius
     intToStr( Temp_Mea,Txt);         //Convert to String
     inTemp=Ltrim(Txt);
     //Lcd_Out(1, 1, "Temp Measured: ");
     Lcd_Out(1, 11, inTemp);        //Display Measured Temp
     Lcd_Out(2, 2, "Temp= ");
     FloatToStr(ActualTemp,Txt);         //Convert to string
     Txt[1] = 0;
     Lcd_Out(2,7,Txt);
     Lcd_Out(2,12," 10  ");

      //Compare ref temp with actual emp
      if (Temp_Mea >  ActualTemp)  //If Temp Ref is less than actual Temp, Switch ON Heater
      {
      HEATER = ON,
      FAN = OFF;
      }
       if (Temp_Mea <  ActualTemp)  //If Temp Ref is greater than actual Temp, Switch ON Fan
      {
      HEATER = OFF,
      FAN = ON;
      }
      if (Temp_Mea ==  ActualTemp)  //If Temp Ref is equal to actual Temp, Switch OFF Fan and Heater
      {
      HEATER = OFF,
      FAN = OFF;
      }
      Delay_ms(40000);        //Wait 20 s then repeat
  }
}

