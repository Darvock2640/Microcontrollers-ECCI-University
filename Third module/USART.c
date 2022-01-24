//..................librerias......................................................................................
#include <xc.h>
#include <stdio.h>

//....................conf_fusibles................................................................................
#pragma config OSC = XT         // Oscillator Selection bits (XT oscillator)
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bit (Brown-out Reset disabled)
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config LVP = OFF        // Low Voltage ICSP Enable bit (Low Voltage ICSP disabled)

#define _XTAL_FREQ	4000000		// Definición del valor del cristal

//..............variables globales y prototipos de funcion.........................................................
unsigned char A = 0, data = 0;
unsigned char buffer[40];

void conf_puertos(void);

void conf_USART(void);

void TX_USART(char dato);
char RX_USART(void);
void write_USART(unsigned char tam);


//..............codigo principal................................................................
void main(void) {
    
    //.........................conf perifericos.................................................
    conf_puertos();
    conf_USART();
    
    write_USART(sprintf(buffer,"Prueba de puerto serial \n\r"));
    
    //.........................bucle.............................................................
    while(1)
    {
        if(RCIF == 1){
            RCIF = 0;
            data = RX_USART();
            LATD = data;
        }
        
        write_USART(sprintf(buffer,"el contador va en : %u \n\r", A));
        A++;
        __delay_ms(250);
        
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
    
    TRISC=0x80;         // puerto C7 entrada para RX resto de salida
    LATC=0;             // limpia puerto 
    PORTC=0;            // limpia puerto 
      
}
void conf_USART(void){
    SPBRG = 25;
    TXSTA = 0b00100100;
    RCSTA = 0b10010000;
}
void TX_USART(char dato){
    while(TRMT == 0);
    TXREG = dato;
}
char RX_USART(void){
    return RCREG;
}
void write_USART(unsigned char tam){
    unsigned char i = 0;
    
    for(i = 0; i < tam; i++){
        TX_USART(buffer[i]);
    }
}