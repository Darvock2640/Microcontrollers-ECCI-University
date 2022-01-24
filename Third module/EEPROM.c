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
void write_EE(unsigned char addr, unsigned char dato);
unsigned char read_EE(unsigned char addr);

//..............codigo principal................................................................
void main(void) {
    
    //.........................conf perifericos.................................................
    conf_puertos();
    write_EE(0,6);
    write_EE(1,'@');
    write_EE(2,'0');
    write_EE(3,'1');
    write_EE(4,'2');
    
    LATD=read_EE(0);
    __delay_ms(250);
    LATD=read_EE(1);
    __delay_ms(250);
    LATD=read_EE(2);
    __delay_ms(250);
    LATD=read_EE(3);
    __delay_ms(250);
    LATD=read_EE(4);
    __delay_ms(250);
    
    //.........................bucle.............................................................
    while(1)
    {
        
             
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
      
}
void write_EE(unsigned char addr, unsigned char dato){
    
    while(WR==1){};
    
    EEADR=addr;         //carga direccion
    EEDATA=dato;        // carga dato
    EEPGD=0;            //Acceso a eeprom
    CFGS=0;             //Hailita acceso eeprom
    WREN=1;             //Habilita escritura
    //---------------------------rutina de control de eeprom--------------------
    GIE=0;              //Des-habilitar interrupciones
    EECON2=0x55;
    EECON2=0xAA;
    
    WR=1;               //Inicia escritura
}
unsigned char read_EE(unsigned char addr){
    EEADR=addr;         //carga direccion
    EEPGD=0;            //Acceso a eeprom
    CFGS=0;             //Hailita acceso eeprom
    RD=1;               //Inicia lectura
    while(RD==1){};
    return EEDATA;
}