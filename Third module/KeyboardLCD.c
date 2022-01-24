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
#define EN RE2
#define RS RE0
#define bus LATD

//..............variables globales y prototipos de funcion.........................................................
unsigned char tecla_ascci = 0, tecla = 0, tecla_num = 0;

//0   1   2   3   4  5    6   7   8   9  10  11   12 13  14  15
unsigned char  teclas[16]={13, 12, 11, 10, 14, 3,   6,  9,  0,  2,  5,  8,  15, 1,  4,  7};
unsigned char t_ascci[16]={'D','C','B','A','E','3','6','9','0','2','5','8','F','1','4','7'};
unsigned char buffer[40];

void conf_puertos(void);
unsigned char teclado(void);
void ini_LCD(void);
void enable(void);
void dato_LCD(unsigned char dato);  
void ins_LCD(unsigned char dato);
void write_LCD(unsigned char tam);

//..............codigo principal................................................................

void main(void) {

    //.........................conf perifericos.................................................
    conf_puertos();
    ini_LCD();
    write_LCD(sprintf(buffer,"Hola Mundo"));
    __delay_ms(1000);
    ins_LCD(1);

    //.........................bucle.............................................................
    while (1) {
        if (PORTB != 0xF0) {
            tecla = teclado();
            tecla_ascci=t_ascci[tecla];
            tecla_num=teclas[tecla];
            dato_LCD(tecla_ascci);
        } else {
            LATD = 255;
        }
    }

}

//......................funciones............................................................

void conf_puertos(void) {

    TRISB = 0xF0; // puerto B <7:4> de entrada y <3:0> salida
    LATB = 0; // limpiar puerto B
    PORTB = 0; // limpiar puerto B

    RBPU = 0; // Habilita resistencias de pull up    

    TRISD = 0x00; // puerto D de salida
    LATD = 0; // limpia puerto D
    PORTD = 0; // limpia puerto D
    
    TRISE = 0x00; // puerto E de salida
    LATE = 0x00; // Limpia puerto 
    PORTE = 0x00; // Limpia puerto
    ADCON1 = 0x06; //Puerto E digital

}

unsigned char teclado(void) {
    unsigned char i=0, j=0;
    
    LATB=0b11111110;
    
    for(i=0;i<4;i++){
        if(RB4==0){break;}
        j++;
        if(RB5==0){break;}
        j++;
        if(RB6==0){break;}
        j++;
        if(RB7==0){break;}
        j++;
        LATB<<=1;
    }
    __delay_ms(25);
    PORTB=0xF0;
    while(PORTB!=0xF0);
    return j;
}
//...................ini_LCD....................................
void ini_LCD (void)
{
	__delay_ms(20);     // delay 20 ms
	bus=0x30;			// VALOR DE INICIALIZACION
	enable();

	__delay_ms(5);		// delay 5 ms
	bus=0x30;			// VALOR DE INICIALIZACION
	enable();

	__delay_ms(1);		// delay 1 ms
	bus=0x30;			// VALOR DE INICIALIZACION
	enable();

	bus=0b00111000;	// Function set 8 bits - 2 lineas - 5*8
	enable();

	bus=0b00001000;	// display off
	enable();

	bus=0b00000001;	// display clear
	enable();

	bus=0b00000110;	// entry mode set - incrementa la posicion del cursor
	enable();

	bus=0b00001110;	// display on/off control - LCD on - cursor on - blink off
	enable();
    
    
}

//...................enable....................................
void enable (void)
{
	EN=1;	// enable en alto
	__delay_ms(1);
	EN=0;	// enable en bajo
}

//...................dato....................................
void dato_LCD(unsigned char dato)
{
	RS=1;               //RS en 1 (envia dato)
	bus=dato;			//dato a publicar 
	enable();
	__delay_ms(2);		//tiempo de ejecucion
}

//...................instruccion..................................
void ins_LCD(unsigned char dato)
{
	RS=0;	//RS en 0 (envia instruccion)
	bus=dato;			//dato a publicar 
	enable();
	__delay_ms(2);		//tiempo de ejecucion
}
void write_LCD(unsigned char tam){
    
    unsigned char i;
    
    for(i=0;i<tam;i++){
        dato_LCD(buffer[i]);
    }
}