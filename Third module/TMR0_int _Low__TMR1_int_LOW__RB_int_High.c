//..................librerias......................................................................................
#include <xc.h>

//....................conf_fusibles................................................................................
#pragma config OSC = XT         // Oscillator Selection bits (XT oscillator)
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bit (Brown-out Reset disabled)
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config LVP = OFF        // Low Voltage ICSP Enable bit (Low Voltage ICSP disabled)

#define _XTAL_FREQ  4000000

//..............variables globales y prototipos de funcion.........................................................
unsigned char A = 0;

void conf_puertos(void);
void conf_tmr0(void);
void conf_tmr1(void);
void conf_int(void);

//..............codigo principal................................................................

void main(void) {

    //.........................conf perifericos.................................................
    conf_puertos();
    conf_tmr0();
    conf_tmr1();
    conf_int();

    //.........................bucle.............................................................
    while (1) {

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
    LATC = 0; // limpia puerto C
    PORTC = 0; // limpia puerto C

}

void conf_tmr0(void) {
    T0CON = 0b10001000; //on,16,tcy,no preescaler
    TMR0H = 0xF8;
    TMR0L = 0x89;

}

void conf_tmr1(void) {
    T1CON = 0b10000000;
    TMR1H = 0x9E;
    TMR1L = 0x57;
}

void conf_int(void) {
    //RBPU=0;
    IPEN = 1;
    INTCON = 0b11101000; //int habilitadas con prioridad, int tmr0 int_RB
    TMR1IE = 1;
    TMR1IP = 0;
    TMR0IP = 0;
    RBIP = 1;
}

void __interrupt(high_priority) HighIsr(void) { //0x08
    unsigned char temp = 0;
    if (RBIF == 1) {
        temp = PORTB;
        RBIF = 0;
        //Antirrebote
        RBIE=0;
        TMR1ON=1;
        //
        if (RB7 == 0) {
            RD7 = ~RD7;
        }
        if (RB6 == 0) {
            RD6 = ~RD6;
        }
        if (RB5 == 0) {
            RD5 = ~RD5;
        }
        if (RB4 == 0) {
            RD4 = ~RD4;
        }
    }

}

void __interrupt(low_priority) LowIsr(void) //0x18
{
    if (TMR0IF == 1) {
        TMR0IF = 0;
        TMR0H = 0xF8;
        TMR0L = 0x89;
        RC0 = ~RC0;
    }
    if (TMR1IF == 1) {
        TMR1IF = 0;
        //Antirrebote
        TMR1ON=0;
        RBIE=1;
        //
        TMR1H = 0x9E;
        TMR1L = 0x57;
    }
}
