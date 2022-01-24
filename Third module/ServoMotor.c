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
unsigned char A = 0, flag_tmr0 = 0;
unsigned int DCH = 0, DCL = 0;

void conf_puertos(void);
void conf_TMR0(void);
void conf_int(void);
void servo_1(unsigned int lamda);

//..............codigo principal................................................................

void main(void) {

    //.........................conf perifericos.................................................
    conf_puertos();
    conf_int();
    conf_TMR0();
    
    servo_1(1500);

    //.........................bucle.............................................................
    while (1) {
        LATD=~LATD;
        __delay_ms(250);
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
    
    TRISC = 0x00; // puerto de salida
    LATC = 0; // limpia puerto 
    PORTC = 0; // limpia puerto

}

void conf_TMR0(void) {
    T0CON = 0b00001000;
}

void conf_int(void){
    IPEN=0;
    INTCON=0b10100000;
}

void interrupt HighIsr(void) { //0x08
    if (TMR0IF == 1) {
        TMR0IF = 0;
        if (flag_tmr0 == 1){
            flag_tmr0 = 0;
            RC0=0;
            TMR0H = DCL>>8;
            TMR0L = DCL;
        }else if (flag_tmr0 == 0){
            flag_tmr0 = 1;
            RC0=1;
            TMR0H = DCH>>8;
            TMR0L = DCH;
        }
    }
}

void interrupt low_priority LowIsr(void) //0x18
{

}
void servo_1(unsigned int lamda){
    
    if(lamda>=1 && lamda<=1500){
        DCH = 65535 - (lamda+1000);
        DCL = 65535 - (20000 - (lamda + 1000));
        TMR0H = DCH>>8;
        TMR0L = DCH;
        RC0=1;
        TMR0ON = 1;
        flag_tmr0=1;
    }else{
        TMR0ON = 0;
        RC0 = 0;
        return;
    }
    
}