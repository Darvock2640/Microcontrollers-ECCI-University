//..................librerias......................................................................................
#include <xc.h>

//....................conf_fusibles................................................................................
#pragma config OSC = XT         // Oscillator Selection bits (XT oscillator)
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bit (Brown-out Reset disabled)
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config LVP = OFF        // Low Voltage ICSP Enable bit (Low Voltage ICSP disabled)

//..............variables globales y prototipos de funcion.........................................................
unsigned char A=0;

void conf_puertos(void);
void conf_timer0(void);

//..............codigo principal................................................................
void main(void) {
    
    //.........................conf perifericos.................................................
    conf_puertos();
    conf_timer0();
    
    //.........................bucle.............................................................
    while(1)
    {
        LATD=TMR0L;
        LATC=TMR0H;
        
        if(TMR0IF==1)
        {
            TMR0H=0x2A;
            TMR0L=0x60;
            TMR0IF=0;
        }
             
    }
    
 }

//......................funciones............................................................
void conf_puertos(void){
    
    TRISB=0xFF;         // puerto B de entrada
    LATB=0;             // limpiar puerto B
    PORTB=0;            // limpiar puerto B
    
    TRISD=0x00;         // puerto D de salida
    LATD=0;             // limpia puerto D
    PORTD=0;            // limpia puerto D
    
    TRISC=0x00;         // puerto C de salida
    LATC=0;             // limpia puerto C
    PORTC=0;            // limpia puerto C
      
}

void conf_timer0(void){
    T0CON=0b10000111;
    TMR0H=0x2A;
    TMR0L=0x60;
}