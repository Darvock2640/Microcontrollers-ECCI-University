//..................librerias......................................................................................
#include <xc.h>

//....................conf_fusibles................................................................................
#pragma config OSC = XT         // Oscillator Selection bits (XT oscillator)
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bit (Brown-out Reset disabled)
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config LVP = OFF        // Low Voltage ICSP Enable bit (Low Voltage ICSP disabled)

#define _XTAL_FREQ	4000000		// Definición del valor del cristal
/*
 RB0->ULTRA SONIDO DERECHA
 RB1->ULTRA SONIDO CENTRO
 RB2->ULTRA SONIDO IZQUIERDA
 RB4->PISO FRENTE DERECHA
 RB5->PISO CENTRO ATRAS
 RB6->PISO FRENTE IZQUIERDA
 
 RC1->PWM2->duty_2->motor_izquierda
 Adelante izquierda
 RC5=1;
 RC6=0;
 RC2->PWM1->duty_1->motor_derecha
 Adelante derecha
 RC3=1;
 RC4=0;
 
 Puerto D xpppxuuu leds
 
 Puerto E trigger ultrasonidos
*/
//..............variables globales y prototipos de funcion.........................................................
unsigned char piso=0, sensores=0;

void conf_puertos(void);
void conf_int(void);
unsigned char ultra(void);
void conf_PWM(void);
void duty_1(unsigned char dc);
void duty_2(unsigned char dc);
void cazador(void);
void ataque(void);

//..............codigo principal................................................................
void main(void) {
    
    //.........................conf perifericos.................................................
    conf_puertos();
    conf_int();   
    conf_PWM();
    
    
    //.........................bucle.............................................................
    while(1)
    {
        sensores=ultra();
        LATD=(piso<<4)&0XF0|sensores&0x0F;
        __delay_ms(60);
    }
    
 }

//......................funciones............................................................
void conf_puertos(void){
    
    TRISB=0x7F;         // puerto B de entrada
    LATB=0;             // limpiar puerto B
    PORTB=0;            // limpiar puerto B
    
    TRISD=0x00;         // puerto D de salida
    LATD=0;             // limpia puerto D
    PORTD=0;            // limpia puerto D
    
    TRISC=0x00;         // Puerto C de salida
    LATC=0;             // Limpiar puerto
    PORTC=0;            // Limpiar puerto
    
    TRISE=0x00;         // Puerto E de salida
    LATE=0;             // Limpiar puerto
    PORTE=0;            // Limpiar puerto
    
}
void conf_int(void){
    INTCON=0b10001000;  // int habilitadas sin prioridad, int externa 0 RB0 enciende int RB4:7
}
//............................interrupcion prioridad alta...................................
void interrupt defensa(){              //0x08
    
    unsigned int i=0;
    piso=0;
    if(RBIF==1){
        piso=PORTB>>4;
        LATD=(piso<<4)&0XF0|sensores&0x0F;        
        switch (piso)
        {
            case 0b00000101:

                break;
            case 0b00000100:

                break;
            case 0b00000110:

                break;
            case 0b00000010:

                break;
            case 0b00000011:

                break;
            case 0b00000001:

                break;
            default:

                break;        
        }      
        RBIF=0;
    }
}
unsigned char ultra(void){
    
    unsigned char f1=0, f2=0, f3=0, a=0;
        
    LATE=0x07;
    __delay_us(10);
    LATE=0;
    while(RB0==0&&RB1==0&&RB2==0){}
    
    while(RB0==1||RB1==1||RB2==1){
        if(RB0==0){
            f1=1;
            break;
        }
        if(RB1==0){
            f2=1;
            break;
        }
        if(RB2==0){
            f3=1;
            break;
        }
        a++;
        if(a>=252){
            f1=0;
            f2=0;
            f3=0;
            break;
        }
    }
    f1|=f2<<1;
    f1|=f3<<2;
    return f1;
}
void conf_PWM(void){
    PR2=124;
    CCPR1L=0;
    CCPR2L=0;
    T2CON=0b00000101;
    CCP1CON=0b00001100;
    CCP2CON=0b00001100;
}
void duty_1(unsigned char dc){
    unsigned int lamda=0;
    
    lamda=dc;
    lamda*=5;
    CCPR1L=lamda>>2;
    lamda<<=4;
    lamda&=0x30;
    CCP1CON&=0xCF;
    CCP1CON|=lamda;
}
void duty_2(unsigned char dc){
    unsigned int lamda=0;
    
    lamda=dc;
    lamda*=5;
    CCPR2L=lamda>>2;
    lamda<<=4;
    lamda&=0x30;
    CCP2CON&=0xCF;
    CCP2CON|=lamda;
}
void cazador(void){
    
}
void ataque(void){
    
}