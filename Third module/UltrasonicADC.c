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
unsigned char A=0,flag=0,ADC_1=0,ADC_2=0;
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
void CONF_ADC(void);
void ESTABLECER_CANAL(unsigned char CANAL);

//..............codigo principal................................................................
void main(void) {
    
    //.........................conf perifericos.................................................
    conf_puertos();
    ini_LCD();
    CONF_ADC();
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
        
        ESTABLECER_CANAL(0);
        ADCON0bits.GO = 1;
        while(ADCON0bits.GO){}
        ADC_1=ADRESH;
        
        ESTABLECER_CANAL(1);
        ADCON0bits.GO = 1;
        while(ADCON0bits.GO){}
        ADC_2=ADRESH;       
        
        ins_LCD(128);
        sprintf(buffer,"U_1=%u",(cm_1/58)-10);
        write(strlen(buffer));
        
        ins_LCD(136);
        sprintf(buffer,"U_2=%u",(cm_2/58)-10);
        write(strlen(buffer));
        
        ins_LCD(192);
        sprintf(buffer,"AD1=%u ",ADC_1);
        write(strlen(buffer));
        
        ins_LCD(200);
        sprintf(buffer,"AD2=%u",ADC_2);
        write(strlen(buffer));       
        
      
        
        __delay_ms(100);
    }
    
 }

//......................funciones............................................................
void conf_puertos(void){
    
    TRISB=0xF7;         // puerto B de entrada
    LATB=0;             // limpiar puerto B
    PORTB=0;            // limpiar puerto B
    
    TRISE=0x00;         // puerto E de salida
    LATE=0;             // limpiar puerto E
    PORTE=0;            // limpiar puerto E
    
    TRISD=0x00;         // puerto D de salida
    LATD=0;             // limpia puerto D
    PORTD=0;            // limpia puerto D
    
    TRISA = 0b00000011; // puerto A ADC dos canales
    LATA=0;             // limpia puerto A
    PORTA=0;            // limpia puerto A
          
}
void conf_tmr0(void)
{
    T0CON=0b10001000;  //on,16,tcy,no preescaler
}
void conf_int(void){
    INTCON=0b10010000;  // int habilitadas sin prioridad, int externa 0 RB0
    INTCON2=0b10000000; // flanco descenso RB0,RB1
    INTCON3=0b00001000; // int1 baja prioridad; int1 habilitadas
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
	PORTEbits.RE2=1;	// enable en alto
	__delay_ms(1);
	PORTEbits.RE2=0;	// enable en bajo
}

//...................dato....................................
void dato_LCD(unsigned char dato)
{
	PORTEbits.RE1=0;	//R/W en 0
	PORTEbits.RE0=1;	//RS en 1 (envia dato)
	PORTD=dato;			//dato a publicar 
	enable();
	__delay_ms(2);		//tiempo de ejecucion
}

//...................instruccion..................................
void ins_LCD(unsigned char dato)
{
	PORTEbits.RE1=0;	//R/W en 0
	PORTEbits.RE0=0;	//RS en 0 (envia instruccion)
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

void CONF_ADC(void)
{
             //76543210
	ADCON0 = 0b01000001;
    

		     //76543210
	ADCON1 = 0b00001101;
    // Emplea referencias de Voltaje Externas
    
    Nop();
    Nop();
    // Dos ciclos de guarda antes de iniciar la primera conversion
    // después de configurar el Módulo ADC
}
///////////////////////////////////////////////////////

void ESTABLECER_CANAL(unsigned char CANAL)
{
	ADCON0bits.CHS = CANAL;
    __delay_ms (5);
    // Retardo para cambiar el canal
    // Es necesario para descargar el condensador de S&H
}

void interrupt base(){              //0x08
    
    if(INT0IF==1){
        INT0IF = 0;
        flag++;
                        
        cm_1=TMR0L;
        cm_1=0;
        cm_1=((TMR0H<<8)|TMR0L);
                
        if(flag==2){
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
                
        if(flag==2){
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
