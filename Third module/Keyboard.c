//..................librerias......................................................................................
#include <xc.h>

//....................conf_fusibles................................................................................
#pragma config OSC = XT         // Oscillator Selection bits (XT oscillator)
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bit (Brown-out Reset disabled)
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config LVP = OFF        // Low Voltage ICSP Enable bit (Low Voltage ICSP disabled)

#define _XTAL_FREQ	4000000		// Definición del valor del cristal

//..............variables globales y prototipos de funcion.........................................................
unsigned char tecla = 0;
                         //0   1   2   3   4  5  6  7  8  9 10 11  12 13 14 15
unsigned char teclas[16]={13, 12, 11, 10, 14, 3, 6, 9, 0, 2, 5, 8, 15, 1, 4, 7};
void conf_puertos(void);
unsigned char teclado(void);

//..............codigo principal................................................................

void main(void) {

    //.........................conf perifericos.................................................
    conf_puertos();

    //.........................bucle.............................................................
    while (1) {
        if (PORTB != 0xF0) {
            tecla = teclas[teclado()];
            LATD = tecla;
        } else {
            LATD = 255;
        }
    }

}

//......................funciones............................................................

void conf_puertos(void) {

    TRISB = 0xF0; // puerto B <7:4> de entrada y <3:0> salida
    LATB = 0; // limpiar puerto B
    PORTB = 0; // limpiar puerto B

    RBPU = 0; // Habilita resistencias de pull up    

    TRISD = 0x00; // puerto D de salida
    LATD = 0; // limpia puerto D
    PORTD = 0; // limpia puerto D

}

unsigned char teclado(void) {
    unsigned char i=0, j=0;
    
    LATB=0b11111110;
    
    for(i=0;i<4;i++){
        if(RB4==0){break;}
        j++;
        if(RB5==0){break;}
        j++;
        if(RB6==0){break;}
        j++;
        if(RB7==0){break;}
        j++;
        LATB<<=1;
    }
    __delay_ms(25);
    PORTB=0xF0;
        
    return j;
}