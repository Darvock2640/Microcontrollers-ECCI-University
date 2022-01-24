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

//..............variables globales y prototipos de funcion.........................................................
unsigned char A=0;
unsigned int cm1=0, cm2=0, cm3=0, temp=0;
unsigned char buffer[40];

void conf_puertos(void);
void ini_LCD(void);
void enable(void);
void dato_LCD(unsigned char dato);
void ins_LCD(unsigned char dato);
void write(unsigned char tam);
void conf_TMR0(void);
void ultra(void);

//..............codigo principal................................................................
void main(void) {
    
    //.........................conf perifericos.................................................
    conf_puertos();
    ini_LCD();
    conf_TMR0();
    
    sprintf(buffer,"Hola clase de");
    write(strlen(buffer));
    
    ins_LCD(192);
    
    sprintf(buffer,"Microcontroladores");
    write(strlen(buffer));
    
    __delay_ms(3000);
    ins_LCD(1);
    
    //.........................bucle.............................................................
    while(1)
    {
        ultra();
        ins_LCD(1);
        sprintf(buffer,"cm1: %u",cm1);
        write(strlen(buffer));
        ins_LCD(192);
        sprintf(buffer,"cm2:%3u ",cm2);
        write(strlen(buffer));
        sprintf(buffer,"cm3:%3u",cm3);
        write(strlen(buffer));
        __delay_ms(100);
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
    
    TRISC=0x00;         // Puerto C de salida
    LATC=0;             // Limpiar puerto
    PORTC=0;            // Limpiar puerto
    
    TRISA=0x00;         // Puerto A de salida
    LATA=0;             // Limpiar puerto
    PORTA=0;            // Limpiar puerto
    ADCON1=0x06;        // Puerto A digital
}
//...................ini_LCD....................................
void ini_LCD (void)
{
	__delay_ms(20);     // delay 20 ms
	PORTD=0x30;			// VALOR DE INICIALIZACION
	enable();

	__delay_ms(5);		// delay 5 ms
	PORTD=0x30;			// VALOR DE INICIALIZACION
	enable();

	__delay_ms(1);		// delay 1 ms
	PORTD=0x30;			// VALOR DE INICIALIZACION
	enable();

	PORTD=0b00111000;	// Function set 8 bits - 2 lineas - 5*8
	enable();

	PORTD=0b00001000;	// display off
	enable();

	PORTD=0b00000001;	// display clear
	enable();

	PORTD=0b00000110;	// entry mode set - incrementa la posicion del cursor
	enable();

	PORTD=0b00001110;	// display on/off control - LCD on - cursor on - blink off
	enable();
    
    
}

//...................enable....................................
void enable (void)
{
	PORTAbits.RA2=1;	// enable en alto
	__delay_ms(1);
	PORTAbits.RA2=0;	// enable en bajo
}

//...................dato....................................
void dato_LCD(unsigned char dato)
{
	PORTAbits.RA1=0;	//R/W en 0
	PORTAbits.RA0=1;	//RS en 1 (envia dato)
	PORTD=dato;			//dato a publicar 
	enable();
	__delay_ms(2);		//tiempo de ejecucion
}

//...................instruccion..................................
void ins_LCD(unsigned char dato)
{
	PORTAbits.RA1=0;	//R/W en 0
	PORTAbits.RA0=0;	//RS en 0 (envia instruccion)
	PORTD=dato;			//dato a publicar 
	enable();
	__delay_ms(2);		//tiempo de ejecucion
}
void write(unsigned char tam){
    
    unsigned char i;
    
    for(i=0;i<tam;i++){
        dato_LCD(buffer[i]);
    }
}
void conf_TMR0(void){
    T0CON=0b00001000;
}
void ultra(void){
    
    unsigned char f1=0, f2=0, f3=0;
    
    TMR0H=0;
    TMR0L=0;
    RC3=1;
    RC4=1;
    RC5=1;
    __delay_us(10);
    RC3=0;
    RC4=0;
    RC5=0;
    TMR0ON=1;
    __delay_us(232);
    
    while(((f1&f2)&f3)==0){
        if(RB0==0&&f1==0){
            f1=1;
            cm1=TMR0L;
            temp=TMR0H;
            cm1|=temp<<8;
            cm1/=58;
        }
        if(RB1==0&&f2==0){
            f2=1;
            cm2=TMR0L;
            temp=TMR0H;
            cm2|=temp<<8;
            cm2/=58;
        }
        if(RB2==0&&f3==0){
            f3=1;
            cm3=TMR0L;
            temp=TMR0H;
            cm3|=temp<<8;
            cm3/=58;
        }
    }
    TMR0ON=0;
}