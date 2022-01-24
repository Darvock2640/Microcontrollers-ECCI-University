//......................................................................................
#include <xc.h>
//....................conf_fusibles................................................................................
#pragma config OSC = XT         // Oscillator Selection bits (XT oscillator)
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bit (Brown-out Reset disabled)
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config LVP = OFF        // Low Voltage ICSP Enable bit (Low Voltage ICSP disabled)

#define _XTAL_FREQ  4000000

//..............variables globales y prototipos de funcion.........................................................
unsigned char A=0,flag=0,i;
unsigned int cm_1=0,cm_2=0,cm_3=0;

void conf_puertos(void);
void conf_tmr0(void);
void conf_int(void);
void conf_PWM(void);
void M_I(unsigned char valor);
void M_D(unsigned char valor);

//..............codigo principal................................................................
void main(void) {
    
    //.........................conf perifericos.................................................
    conf_puertos();
    conf_tmr0();
    conf_int();
    conf_PWM();

    M_I(0);
    M_D(0);
        
    
    //.........................bucle.............................................................
    while(1)
    {
        if(flag==0){
            RB3=1;
            TMR0ON=1;
            __delay_us(12);
            RB3=0;
        }
              
        __delay_ms(100);
    }
    
 }

//......................funciones............................................................
void conf_puertos(void){
    
    TRISB=0xF7;         // RB7 piso,RB6 piso,RB5 piso,RB4,RB2 ultra,RB1 ultra,RB0 ultra, de entrada. RB3 salida trigger ultra
    LATB=0;             // limpiar puerto B
    PORTB=0;            // limpiar puerto B
    
    TRISD=0x00;         // puerto D de salida
    LATD=0;             // limpia puerto D
    PORTD=0;            // limpia puerto D
    
    TRISA=0x00;         // puerto A de salida
    LATA=0;             // limpia puerto A
    PORTA=0;            // limpia puerto A
    ADCON1=0x06;        // Puerto A digital
    
    TRISC=0x00;         // puerto C de salida,  RC1 y RC2 salida PWM
    LATC=0;             // limpia puerto C
    PORTC=0;            // limpia puerto C
      
}
//............................conf timer 0................................................
void conf_tmr0(void)
{
    T0CON=0b10001000;  //on,16,tcy,no preescaler
}
//............................conf interrupciones..........................................
void conf_int(void){
    IPEN=1;             // Habilita prioridades de interrupción
    INTCON=0b11011000;  // int habilitadas sin prioridad, int externa 0 RB0 enciende int RB4:7
    INTCON2=0b10000000; // flanco descenso RB0,RB1,RB2, RB prioridad baja 
    INTCON3=0b11011000; // int1,int2 prioridad alta, int1,int2 habilitadas
}
//............................conf PWM....................................................
void conf_PWM(void){
    PR2=166;				//PWM a 1.5KHz
    CCPR1L=0;				//PWM apagado
    CCPR2L=0;				//PWM apagado
    T2CON=0b00000101;		//Timer 2 encendido prescaler de 4
    CCP1CON=0b00001100;		//modulo CCP1 como PWM
    CCP2CON=0b00001100;		//modulo CCP2 como PWM
}
//............................asignar porcentaje duty a PWM1...............................
void M_I(unsigned char valor){

    unsigned int temp=0;
    
    temp=valor;				// guarda valor de porcentaje en 16 bits
    temp=(66*temp)/10;		// despeja ecuación de duty cicle
    CCPR1L=temp>>2;			// carga los 8 bits de mayor peso del duty
    temp=(temp<<4)&0x0030;	// limpia los bits de menor peso
    CCP1CON&=0xCF;			// limpia los bits 5 y 4 del ccpxcon
    CCP1CON|=temp;			// carga los 2 bits de menor peso del duty
       
}
//............................asignar porcentaje duty a PWM2...............................
void M_D(unsigned char valor){

    unsigned int temp=0;
    
    temp=valor;				// guarda valor de porcentaje en 16 bits
    temp=(66*temp)/10;		// despeja ecuación de duty cicle
    CCPR2L=temp>>2;			// carga los 8 bits de mayor peso del duty
    temp=(temp<<4)&0x0030;	// limpia los bits de menor peso
    CCP2CON&=0xCF;			// limpia los bits 5 y 4 del ccpxcon
    CCP2CON|=temp;			// carga los 2 bits de menor peso del duty
       
}
//............................interrupcion prioridad alta...................................
void interrupt hight(){              //0x08
    if(INT0IF==1){			//pregunta por int0
        INT0IF = 0;			//baja bandera de interrupcion
        flag++;				//incrementa bandera de final de los tres ultra
                        
        cm_1=TMR0L;			//lee el valor del sensor de ultra
        cm_1=0;
        cm_1=((TMR0H<<8)|TMR0L)/58;
                
        if(flag==3){		// ya terminaron los tres ultrasonidos?
            flag=0;
            TMR0ON=0;		// apaga el timer 0
            TMR0H=0;		// reinicia timer 0
            TMR0L=0;
        }
    }    
    if(INT1IF==1){
        INT1IF = 0;
        flag++;
                        
        cm_2=TMR0L;
        cm_2=0;
        cm_2=((TMR0H<<8)|TMR0L)/58;
                
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
        cm_3=((TMR0H<<8)|TMR0L)/58;
                
        if(flag==3){
            flag=0;
            TMR0ON=0;
            TMR0H=0;
            TMR0L=0;
        }
    }     
}
//............................interrupcion prioridad baja...................................
void interrupt low_priority low(void){      //0x18
    if(RBIF==1){		//hubo interrupción por los sensores de piso
        RBIF=0;			//limpia bandera de interrupción
        
		if(RB7==0){		//fue el sensor conectado a RB7?
           
        }
        if(RB6==0){		//fue el sensor conectado a RB6?
            
        }
        if(RB5==0){		//fue el sensor conectado a RB5?
            
        }
    }
}
