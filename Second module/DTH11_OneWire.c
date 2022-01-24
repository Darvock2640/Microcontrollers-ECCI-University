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

#define _XTAL_FREQ 4000000

//..............variables globales y prototipos de funcion.........................................................

unsigned char buff_LCD[40];
unsigned char buff_USART[40];
unsigned char j = 0, flag = 0, hum_int = 0, temp_int = 0;

void conf_puertos(void);

void one_second(void);

void conf_USART(void); //prototipo de función 
void tx(unsigned char dato);
unsigned char rx(void);
void write_USART(unsigned char tam);

void ini_LCD(void);
void enable(void);
void dato_LCD(unsigned char dato);
void ins_LCD(unsigned char dato);
void write_LCD(unsigned char tam);

void sensor(void);
unsigned char lectura(void);

void main(void) {
    conf_puertos();
    ini_LCD();
    conf_USART();
    
    sprintf(buff_LCD,"   Sensor de   ");
    write_LCD(strlen(buff_LCD));
    
    ins_LCD(192);
    
    sprintf(buff_LCD,"    Humedad    ");
    write_LCD(strlen(buff_LCD));
    
    one_second();
    one_second();
    one_second();
    
    ins_LCD(1);
    
    while (1) {
        
        
        if(RB0==0){
            sprintf(buff_USART,"no");
            write_USART(strlen(buff_USART));
            __delay_ms(50);
            while(RB0==0){}
        }
        
        ins_LCD(128);
               
        sensor();
        
        sprintf(buff_LCD,"Humedad: %i",hum_int);
        sprintf(buff_USART,"%i",hum_int);
        write_LCD(strlen(buff_LCD));
        write_USART(strlen(buff_USART));
        
        one_second();
        
        
       
    }
}

void conf_puertos(void) {

    TRISD = 0x00; // puerto D de salida
    LATD = 0; // limpia puerto D
    PORTD = 0; // limpia puerto D

    TRISC = 0x80; //Puerto C conf para Usart y sensor
    LATC = 0x01;
    PORTC = 0x01;

    TRISA = 0x00; // puerto A de salida
    LATA = 0x00; // Limpia puerto C
    PORTA = 0x00; // Limpia puerto C
    ADCON1 = 0x06; //Puerto A digital

    TRISB=0XFF;
    LATB=0;
    PORTB=0;
    INTCON2bits.RBPU=0;
    
}

//...................ini_LCD....................................

void ini_LCD(void) {
    __delay_ms(20); // delay 20 ms
    PORTD = 0x30; // VALOR DE INICIALIZACION
    enable();

    __delay_ms(5); // delay 5 ms
    PORTD = 0x30; // VALOR DE INICIALIZACION
    enable();

    __delay_ms(1); // delay 1 ms
    PORTD = 0x30; // VALOR DE INICIALIZACION
    enable();

    PORTD = 0b00111000; // Function set 8 bits - 2 lineas - 5*8
    enable();

    PORTD = 0b00001000; // display off
    enable();

    PORTD = 0b00000001; // display clear
    enable();

    PORTD = 0b00000110; // entry mode set - incrementa la posicion del cursor
    enable();

    PORTD = 0b00001100; // display on/off control - LCD on - cursor on - blink off
    enable();
}

//...................enable....................................

void enable(void) {
    PORTAbits.RA2 = 1; // enable en alto
    __delay_ms(1);
    PORTAbits.RA2 = 0; // enable en bajo
}

//...................dato....................................

void dato_LCD(unsigned char dato) {
    PORTAbits.RA1 = 0; //R/W en 0
    PORTAbits.RA0 = 1; //RS en 1 (envia dato)
    PORTD = dato; //dato a publicar 
    enable();
    __delay_ms(2); //tiempo de ejecucion
}

//...................instruccion..................................

void ins_LCD(unsigned char dato) {
    PORTAbits.RA1 = 0; //R/W en 0
    PORTAbits.RA0 = 0; //RS en 0 (envia instruccion)
    PORTD = dato; //dato a publicar 
    enable();
    __delay_ms(2); //tiempo de ejecucion
}

void write_LCD(unsigned char tam) {

    unsigned char i;

    for (i = 0; i < tam; i++) {
        dato_LCD(buff_LCD[i]);
    }
}

void write_USART(unsigned char tam) {

    unsigned char i;

    for (i = 0; i < tam; i++) {
        tx(buff_USART[i]);
    }
}

void conf_USART(void) {

    SPBRG = 25; //Velocidad de transmision a 9600
    TXSTA = 0b00100100;
    RCSTA = 0b10010000;

}

void tx(unsigned char dato) {
    while (TXSTAbits.TRMT == 0) {
    }
    TXREG = dato;
}

unsigned char rx(void) {
    return RCREG;
}

void sensor(void) {
    unsigned char p = 0, o = 0;

    hum_int = 0;
    temp_int = 0;
    flag = 0;

    //::::::::::inicio de lectura::::::::::::::::::::::::::::::::::::

    TRISCbits.RC0 = 0; //CONFIGURO PIN DE SALIDA
    PORTCbits.RC0 = 1; //PONGO EN 1 LINEA DE DATOS
    PORTCbits.RC0 = 0; //PONGO EN 0 LA LINE DE DATOS
    __delay_ms(18); //Espera 18 ms
    PORTCbits.RC0 = 1; //PONGO EN 1 LA LINE DE DATOS

    //:::::::::::::::::preparacion:::::::::::::::::::::::::::::::::::::::

    TRISCbits.RC0 = 1; //CONFIGURO PIN DE ENTRADA
    while (flag != 3) {
        while (PORTCbits.RC0 == 1) {
        }   
        flag++;
        while (PORTCbits.RC0 == 0) {
        }
        flag++;
        while (PORTCbits.RC0 == 1) {
        }
        flag++;
    }
    //::::::::::::::::humedad entero :::::::::::::::::::::::::::::::::::::::::::::::

    __delay_us(20); //inicio bit dato del sensor

    for (j = 0; j <= 7; j++) {
        __delay_us(20); //inicio bit dato del sensor
        PORTCbits.RC1 = ~PORTCbits.RC1;
        p = lectura();
        hum_int |= p;

        if (j < 7) {
            hum_int = hum_int << 1;
        }
    }

    //:::::::::::::::::::::humedad decimas ::::::::::::::::::::::::::::::::::::::	

    __delay_us(10); //inicio bit dato del sensor

    for (j = 0; j <= 7; j++) {
        __delay_us(20); //inicio bit dato del sensor
        PORTCbits.RC1 = ~PORTCbits.RC1;
        p = lectura();
        o |= p;

        if (j < 7) {
            o = o << 1;
        }
    }

    //:::::::::::::::::::::temperarura enteros::::::::::::::::::::::::::::::::::::::	

    __delay_us(10); //inicio bit dato del sensor

    for (j = 0; j <= 7; j++) {
        __delay_us(20); //inicio bit dato del sensor
        PORTCbits.RC1 = ~PORTCbits.RC1;
        p = lectura();
        temp_int |= p;

        if (j < 7) {
            temp_int = temp_int << 1;
        }
    }

    //:::::::::::::::::::::temperarura decimas::::::::::::::::::::::::::::::::::::::	

    __delay_us(10); //inicio bit dato del sensor

    for (j = 0; j <= 7; j++) {
        __delay_us(20); //inicio bit dato del sensor
        PORTCbits.RC1 = ~PORTCbits.RC1;
        p = lectura();
        o |= p;

        if (j < 7) {
            o = o << 1;
        }
    }

    //:::::::::::::::::::::check sum::::::::::::::::::::::::::::::::::::::	

    __delay_us(10); //inicio bit dato del sensor

    for (j = 0; j <= 7; j++) {
        __delay_us(20); //inicio bit dato del sensor
        PORTCbits.RC1 = ~PORTCbits.RC1;
        p = lectura();
        o |= p;

        if (j < 7) {
            o = o << 1;
        }
    }

    __delay_ms(4);

    TRISCbits.RC0 = 0; //CONFIGURO PIN DE SALIDA
    PORTCbits.RC0 = 1; //PONGO EN 1 LINEA DE DATOS

}

unsigned char lectura(void) {
    unsigned char o = 0, temp = 0;
    while (PORTCbits.RC0 == 1) {
        o++;
    }

    if (o < 7) {
        temp = 0;
    } else {
        temp = 1;
    }

    return temp;

}

void one_second(void){
    __delay_ms(150);
    __delay_ms(150);
    __delay_ms(150);
    __delay_ms(150);
    __delay_ms(150);
    __delay_ms(150);
    __delay_ms(100);
}