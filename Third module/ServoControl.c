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
unsigned char A=0;

void conf_puertos(void);
void conf_TIMER(void);
void servo(unsigned char angulo, unsigned char angulo2);

//..............codigo principal................................................................
void main(void) {
    
    //.........................conf perifericos.................................................
    conf_puertos();
    conf_TIMER();
    
    //.........................bucle.............................................................
    while(1){
        
        if(RB0==0){
            servo(0,90);
            __delay_ms(20);
            while(RB0==0){}
        }
    }
    
 }

//......................funciones............................................................
void conf_puertos(void){
    
    TRISB=0xFF;         // puerto B de entrada
    LATB=0;             // limpiar puerto B
    PORTB=0;            // limpiar puerto B
    RBPU=0;
    
    TRISD=0x00;         // puerto D de salida
    LATD=0;             // limpia puerto D
    PORTD=0;            // limpia puerto D
      
}
void conf_TIMER(void){
    T0CON=0b00001000;   //Timer off, TCY, No Press
    TMR0H=0xB1;         //Temporizador de 20ms
    TMR0L=0xDF;
}
void servo(unsigned char angulo,unsigned char angulo2){
    unsigned char i=0,high=0,low=0,l,h,high2,low2;
    unsigned int val_1=0,val_2=0;
    val_1=angulo;
    val_1=(val_1*50)/9;
    val_1+=1000;
    val_1+=45535;               // valor de inicialización del timer
    high=val_1>>8;
    low=val_1;
    
    val_2=angulo2;
    val_2=(val_2*50)/9;
    val_2+=1000;
    val_2+=45535;               // valor de inicialización del timer
    high2=val_2>>8;
    low2=val_2;
    
    RD0=1;
    TMR0ON=1;
    TMR0H=0xB1;         //Temporizador de 20ms
    TMR0L=0xDF;
    
    for(i=0;i<20;i++){
        TMR0IF=0;
        RD0=1;
        RD1=1;
        TMR0H=0xB1;         //Temporizador de 20ms
        TMR0L=0xDF;
        while(TMR0IF==0){
            l=TMR0L;
            h=TMR0H;
            if(l>=low){
                if(h>=high){
                    RD0=0;
                }
            }
            if(l>=low2){
                if(h>=high2){
                    RD1=0;
                }
            }
        }
    }
    
}