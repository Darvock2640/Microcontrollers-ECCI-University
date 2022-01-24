//..................librerias......................................................................................
#include <xc.h>

//....................conf_fusibles................................................................................
#pragma config OSC = XT         // Oscillator Selection bits (XT oscillator)
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bit (Brown-out Reset disabled)
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config LVP = OFF        // Low Voltage ICSP Enable bit (Low Voltage ICSP disabled)

#define _XTAL_FREQ 4000000

//..............variables globales y prototipos de funcion.........................................................
unsigned char cronometro = 0, anodo = 0;
unsigned int contador = 0;
                          // 0    1     2    3     4     5     6     7     8	  9 
unsigned char deco[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x83, 0xF8, 0x80, 0x98};
unsigned char display[2] = {0xFF, 0xFF};

void conf_puertos(void);
void asignar(unsigned char dato);
void visualizacion(void);

//..............codigo principal................................................................

void main(void) {

    //.........................conf perifericos.................................................
    conf_puertos();
    asignar(cronometro);
    
    //.........................bucle.............................................................
    while (1) {

        __delay_ms(1);
        visualizacion();

        contador++;
        if (contador == 300){
 //		if(contador==2500){
        
            contador = 0;
            cronometro++;
            if (cronometro == 100) {
                cronometro = 0;
            }
            asignar(cronometro);
        }
    }
}

//......................funciones............................................................

void conf_puertos(void) {

    TRISB = 0xFF; // puerto B de entrada
    LATB = 0; // limpiar puerto B
    PORTB = 0; // limpiar puerto B

    TRISD = 0x00; // puerto D de salida
    LATD = 0; // limpia puerto D
    PORTD = 0; // limpia puerto D

    TRISC = 0x00; // puerto C de salida
    LATC = 0x00; // Limpia puerto C
    PORTC = 0x00; // Limpia puerto C

}

void asignar(unsigned char dato) {
    //................. Variables locales FUNCION.........

    display[0] = dato / 10;
    display[1] = dato % 10;

}

void visualizacion(void) {
    //................. Variables locales FUNCION.........

    LATC = 0b00000001 << anodo;
    LATD = (deco[display[anodo]]);
    anodo++;
    if (anodo == 2) {
        anodo = 0;
    }
}