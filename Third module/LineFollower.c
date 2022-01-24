//..................librerias......................................................................................
#include <xc.h>

//....................conf_fusibles................................................................................
#pragma config OSC = XT         // Oscillator Selection bits (XT oscillator)
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bit (Brown-out Reset disabled)
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config LVP = OFF        // Low Voltage ICSP Enable bit (Low Voltage ICSP disabled)

#define _XTAL_FREQ	4000000		// Definición del valor del cristal
#define umbral  614             // Definicion del valor digital de conversión  en linea negra

//..............variables globales y prototipos de funcion.........................................................
unsigned char sensores = 0;

void conf_puertos(void);
void conf_ADC(void);
unsigned int conv_ADC(unsigned char canal);
unsigned char leer_sensores(void);
void seguidor(unsigned char valor);
//..............codigo principal................................................................

void main(void) {

    //.........................conf perifericos.................................................
    conf_puertos();
    conf_ADC();

    //.........................bucle.............................................................
    while (1) {
        sensores = leer_sensores();
		LATD = sensores;
        seguidor(sensores);
    }
}

//......................funciones............................................................

void conf_puertos(void) {

    TRISB = 0xFF; // puerto B de entrada
    LATB = 0; // limpiar puerto B
    PORTB = 0; // limpiar puerto B

    TRISC = 0x00; // puerto C de salida
    LATC = 0; // limpia puerto C
    PORTC = 0; // limpia puerto C

    TRISA = 0xFF; // puerto A de Entrada
    LATA = 0; // limpia puerto A
    PORTA = 0; // limpia puerto A

    TRISE = 0x07; // puerto E de Entrada
    LATE = 0; // limpia puerto E
    PORTE = 0; // limpia puerto E

    TRISD = 0x00; // puerto D de salida
    LATD = 0; // limpia puerto D
    PORTD = 0; // limpia puerto D     

}

void conf_ADC(void) {
    ADCON0 = 0b00000001;
    ADCON1 = 0b00000000;
}

unsigned int conv_ADC(unsigned char canal) {

    unsigned int temp = 0;

    if (canal >= 0 && canal <= 7) {
        canal <<= 3;
        ADCON0 &= 0xC7;
        ADCON0 |= canal;
        __delay_us(50);
        GO = 1;
        while (GO) {
        };
        temp = ADRESH;
        temp <<= 2;
        temp |= ADRESL >> 6;
        return temp;
    } else {
        return 0;
    }
}

unsigned char leer_sensores(void) {
    unsigned char temp = 0, i = 0;
    unsigned int x = 0;

    for (i = 0; i < 8; i++) {
        x = conv_ADC(i);
        if (x > umbral) {
            temp |= 128;
        }
        if (i < 7) {
            temp >>= 1;
        }
    }
    return temp;
}
void seguidor(unsigned char valor){
    switch (valor) {
        case 0b11000000:
        //                mot_iz(20);
        //                mot_de(100);
        break;
        case 0b01100000:
        //                mot_iz(20);
        //                mot_de(100);
        break;
    }
}