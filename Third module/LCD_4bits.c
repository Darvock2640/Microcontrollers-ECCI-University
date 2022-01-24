//..................librerias......................................................................................
#include <xc.h>
#include <stdio.h>
#include <string.h>
//....................conf_fusibles................................................................................
#pragma config OSC = XT         // Oscillator Selection bits (XT oscillator)
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bit (Brown-out Reset disabled)
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config LVP = OFF        // Low Voltage ICSP Enable bit (Low Voltage ICSP disabled)

#define _XTAL_FREQ	4000000		// Definición del valor del cristal
#define RS      RB0             // Define el pin conectado a RS
#define RW      RB1             // Define el pin conectado a RW
#define E       RB2             // Define el pin conectado a E
#define PLCD    LATB

//..............variables globales y prototipos de funcion.........................................................
unsigned char buffer[40];
unsigned char i=0;
unsigned int num=0;

void conf_puertos(void);
void ini_LCD(void);
void enable(void);
void dato_LCD(unsigned char dato);
void ins_LCD(unsigned char dato);
void write(unsigned char tam);

//..............codigo principal................................................................
void main(void) {
    
    //.........................conf perifericos.................................................
    conf_puertos();
    ini_LCD();
    
    sprintf(buffer,"0 para todos"); 
    write(strlen(buffer));
    
    ins_LCD(192);
    
    sprintf(buffer,"menos para Perendejo");
    write(strlen(buffer));
    
    for(i=0;i<10;i++)
    {
        __delay_ms(100);
    }

    ins_LCD(1);
    
    //.........................bucle.............................................................
    while(1)
    {
         __delay_ms(100);
        ins_LCD(2);
        num++;
        sprintf(buffer,"Num: %5u",num);
        write(strlen(buffer));
    }
    
 }

//......................funciones............................................................
void conf_puertos(void){
    
    TRISB=0x00;         // puerto B de entrada
    LATB=0;             // limpiar puerto B
    PORTB=0;            // limpiar puerto B
}

void ini_LCD(void)
{
    //INICIALIZACION
    __delay_ms(20);     //RETARDO DE MAS DE 15 mS
    PLCD=0B00110000;	//VALOR DE INICIALIZACION 
    enable();
    __delay_ms(5);		//RETARDO DE MAS DE 4.1 mS
    PLCD=0B00110000;	//VALOR DE INICIALIZACION	
    enable();
    __delay_ms(1);		//RETARDO DE MAS DE 100 uS
    PLCD=0B00110000;	//VALOR DE INICIALIZACION
    enable();
    
    PLCD=0b00100000;    //Modo 4 bits 
    enable();
    
    PLCD=0b00100000;    //Modo 4 bits 2 renglones
    enable();
    PLCD=0b10000000;
    enable();
    
    PLCD=0b00000000;    //Display off cursor off blink off
    enable();
    PLCD=0b10000000;
    enable();
    
    PLCD=0b00000000;    //Display clear
    enable();
    PLCD=0b00010000;
    enable();
    
    PLCD=0b00000000;    //Entry mode set 
    enable();
    PLCD=0b01100000;
    enable();
    
    PLCD=0b00000000;
    enable();
    PLCD=0b11000000;	// LCD on - blink off - cursor off
    enable();
}
void enable(void)
{
	E=1;	// pone el pin de enable en 1 
	__delay_ms(1);		// delay de 2 mS
	E=0;	// pone el pin de enable en 0
}
void dato_LCD(unsigned char dato)
{
    PLCD=0;
	RS=1;           	//pone RS en uno para escribir en la LCD
    RW=0;
	PLCD|=dato&0xF0;			//envia parte alta del dato
	enable();	
    __delay_ms(2);
	PLCD&=0x0F;
    PLCD|=(dato<<4)&0xF0;		//envia parte baja
	enable();			
    __delay_ms(2);
}

void ins_LCD(unsigned char dato)
{
    PLCD=0;
	RS=0;           	//pone RSen cero para enviar un comando a la LCD
    RW=0;
	PLCD|=dato&0xF0;    //envia parte alta del comando
	enable();
    __delay_ms(2);
    PLCD&=0x0F;
	PLCD|=(dato<<4)&0xF0;		//envia parte baja del comando
	enable();
    __delay_ms(2);
}

void write(unsigned char tam){
    
    unsigned char i;
    
    for(i=0;i<tam;i++){
        dato_LCD(buffer[i]);
    }
}