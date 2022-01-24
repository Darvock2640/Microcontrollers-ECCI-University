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

#define _XTAL_FREQ  4000000

//..............variables globales y prototipos de funcion.........................................................
unsigned char A=0,flag=0;
unsigned char buffer[40];
unsigned int cm_1=0,cm_2=0,cm_3=0;

void conf_puertos(void);
void ini_LCD(void);
void enable(void);
void dato_LCD(unsigned char dato);
void ins_LCD(unsigned char dato);
void write(unsigned char tam);
void conf_tmr0(void);
void conf_int(void);

//..............codigo principal................................................................
void main(void) {
    
    //.........................conf perifericos.................................................
    conf_puertos();
    ini_LCD();
    conf_tmr0();
    conf_int();
        
//    sprintf(buffer,"Hola mundo");
//    write(strlen(buffer));
    
    //.........................bucle.............................................................
    while(1)
    {
        ins_LCD(1);
        
        if(flag==0){
            RB3=1;
            TMR0ON=1;
            __delay_us(12);
            RB3=0;
        }
              
        ins_LCD(128);
        sprintf(buffer,"U_1=%u",(cm_1/58)-10);
        write(strlen(buffer));
        
        ins_LCD(136);
        sprintf(buffer,"U_2=%u",(cm_2/58)-10);
        write(strlen(buffer));
        
        ins_LCD(192);
        sprintf(buffer,"U_3=%u",(cm_3/58)-10);
        write(strlen(buffer));
        
        
      
        
        __delay_ms(100);
    }
    
 }

//......................funciones............................................................
void conf_puertos(void){
    
    TRISB=0xF7;         // puerto B de entrada
    LATB=0;             // limpiar puerto B
    PORTB=0;            // limpiar puerto B
    
    TRISD=0x00;         // puerto D de salida
    LATD=0;             // limpia puerto D
    PORTD=0;            // limpia puerto D
    
    TRISA=0x00;         // puerto C de salida
    LATA=0;             // limpia puerto C
    PORTA=0;            // limpia puerto C
    ADCON1=0x06;        // Puerto A digital
      
}
void conf_tmr0(void)
{
    T0CON=0b10001000;  //on,16,tcy,no preescaler
}
void conf_int(void){
    INTCON=0b10010000;  // int habilitadas sin prioridad, int externa 0 RB0
    INTCON2=0b10000000; // flanco descenso RB0,RB1,RB2
    INTCON3=0b00011000; // int1,int2 baja prioridad; int1,int2 habilitadas
}

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

void interrupt base(){              //0x08
    
    if(INT0IF==1){
        INT0IF = 0;
        flag++;
                        
        cm_1=TMR0L;
        cm_1=0;
        cm_1=((TMR0H<<8)|TMR0L);
                
        if(flag==3){
            flag=0;
            TMR0ON=0;
            TMR0H=0;
            TMR0L=0;
        }
    }    
    if(INT1IF==1){
        INT1IF = 0;
        flag++;
                        
        cm_2=TMR0L;
        cm_2=0;
        cm_2=((TMR0H<<8)|TMR0L);
                
        if(flag==3){
            flag=0;
            TMR0ON=0;
            TMR0H=0;
            TMR0L=0;
        }
    }    
    if(INT2IF==1){
        INT2IF = 0;
        flag++;
                        
        cm_3=TMR0L;
        cm_3=0;
        cm_3=((TMR0H<<8)|TMR0L);
                
        if(flag==3){
            flag=0;
            TMR0ON=0;
            TMR0H=0;
            TMR0L=0;
        }
    }    
}

//void interrupt low_priority LowIsr(void)  //0x18
//{
//	
//}
