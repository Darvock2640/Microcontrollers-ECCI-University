//..................librerias......................................................................................
#include <xc.h>
#include<stdio.h>
#include<string.h>

//....................conf_fusibles................................................................................
#pragma config OSC = XT         // Oscillator Selection bits (XT oscillator)
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bit (Brown-out Reset disabled)
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config LVP = OFF        // Low Voltage ICSP Enable bit (Low Voltage ICSP disabled)

#define _XTAL_FREQ	4000000		// Definición del valor del cristal
#define RS      RD0             // Define el pin conectado a RS
#define RW      RD1             // Define el pin conectado a RW
#define E       RD2             // Define el pin conectado a E
#define PLCD    LATD

//..............variables globales y prototipos de funcion.........................................................
unsigned char pos=0,tecla=0;
unsigned char buffer[40];

void conf_puertos(void);
unsigned char teclado(void);
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
    //.........................bucle.............................................................
    while(1)
    {
        if(PORTB!=0xF0){
            tecla=teclado();
            dato_LCD(tecla);
            __delay_ms(20);
            while(PORTB!=0xF0){}
        }
        
    }
    
 }

//......................funciones............................................................
void conf_puertos(void){
    
    TRISB=0xF0;         // puerto B mayor peso de entrada, 4 menor peso salida
    LATB=0xF0;             // limpiar puerto B
    PORTB=0xF0;            // limpiar puerto B
    RBPU=0;
    
    TRISD=0x00;         // puerto D de salida
    LATD=0;             // limpia puerto D
    PORTD=0;            // limpia puerto D
      
}
unsigned char teclado(void){
    unsigned char teclas[16]={'A','B','C','D','9','6','3','#','8','5','2','0','7','4','1','*'};
    unsigned char i=0,j=0;
    
    PORTB=0b11111110;
    
    for(i=0;i<4;i++){
        if(RB4==0){break;}
        j++;
        if(RB5==0){break;}
        j++;
        if(RB6==0){break;}
        j++;
        if(RB7==0){break;}
        j++;    
        PORTB=PORTB<<1;
    }
    
    PORTB=0xF0;
    return teclas[j];
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