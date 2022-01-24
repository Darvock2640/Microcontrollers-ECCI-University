//..................librerias......................................................................................
#include <xc.h>
#include <stdio.h>

//....................conf_fusibles................................................................................
#pragma config OSC = XT         // Oscillator Selection bits (XT oscillator)
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bit (Brown-out Reset disabled)
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config LVP = OFF        // Low Voltage ICSP Enable bit (Low Voltage ICSP disabled)

#define _XTAL_FREQ 4000000
#define EN RE2
#define RS RE0
#define cota_sup    531
#define cota_inf    490
#define izquierda   1
#define sup_izq     2
#define arriba      3
#define sup_der     4
#define derecha     5
#define inf_der     6
#define abajo       7
#define inf_izq     8
#define nulo        0
#define personaje   1
//..............variables globales y prototipos de funcion.........................................................

unsigned char buffer[40];
unsigned char pos_per = 212, joystick = 0;

void conf_puertos(void);
void ini_LCD(void);
void enable(void);
void dato_LCD(unsigned char dato);
void ins_LCD(unsigned char dato);
void write_LCD(unsigned char tam);

void conf_ADC(void);
unsigned int conv_ADC(unsigned char canal);

unsigned char leer_joystick(void);
void crear_caracteres(void);

//..............codigo principal................................................................

void main(void) {

    //.........................conf perifericos.................................................
    conf_puertos();
    conf_ADC();
    ini_LCD(); //inicializa la LCD
    crear_caracteres();

    ins_LCD(128);
    write_LCD(sprintf(buffer, "Movimiento"));

    ins_LCD(192);
    write_LCD(sprintf(buffer, "del"));

    ins_LCD(148);
    write_LCD(sprintf(buffer, "Personaje"));

    __delay_ms(2000);
    ins_LCD(1);

    ins_LCD(pos_per);
    dato_LCD(personaje);

    //.........................bucle.............................................................
    while (1) {
        joystick = leer_joystick();
        if (joystick == arriba) {
            if (pos_per >= 148 && pos_per <= 167) {
                ins_LCD(pos_per);
                dato_LCD(' ');
                pos_per += 44;
                ins_LCD(pos_per);
                dato_LCD(personaje);
            }else if ((pos_per >= 192 && pos_per <= 211) || (pos_per >= 212 && pos_per <= 231)) {
                ins_LCD(pos_per);
                dato_LCD(' ');
                pos_per -= 64;
                ins_LCD(pos_per);
                dato_LCD(personaje);
            }
        }
        if (joystick == abajo) {
            if ((pos_per >= 128 && pos_per <= 147) || (pos_per >= 148 && pos_per <= 167)) {
                ins_LCD(pos_per);
                dato_LCD(' ');
                pos_per += 64;
                ins_LCD(pos_per);
                dato_LCD(personaje);
            }else if (pos_per >= 192 && pos_per <= 211) {
                ins_LCD(pos_per);
                dato_LCD(' ');
                pos_per -= 44;
                ins_LCD(pos_per);
                dato_LCD(personaje);
            }
        }
        if(joystick==derecha){
            if ((pos_per >= 128 && pos_per < 147) || (pos_per >= 192 && pos_per < 211) || (pos_per >= 148 && pos_per < 167) || (pos_per >= 212 && pos_per < 231)) {
                ins_LCD(pos_per);
                dato_LCD(' ');
                pos_per ++;
                ins_LCD(pos_per);
                dato_LCD(personaje);
            }
        }
        if(joystick==izquierda){
            if ((pos_per > 128 && pos_per <= 147) || (pos_per > 192 && pos_per <= 211) || (pos_per > 148 && pos_per <= 167) || (pos_per > 212 && pos_per <= 231)) {
                ins_LCD(pos_per);
                dato_LCD(' ');
                pos_per --;
                ins_LCD(pos_per);
                dato_LCD(personaje);
            }
        }
        if (joystick == sup_der) {
            if (pos_per >= 148 && pos_per < 167) {
                ins_LCD(pos_per);
                dato_LCD(' ');
                pos_per += 45;
                ins_LCD(pos_per);
                dato_LCD(personaje);
            }else if ((pos_per >= 192 && pos_per < 211) || (pos_per >= 212 && pos_per < 231)) {
                ins_LCD(pos_per);
                dato_LCD(' ');
                pos_per -= 63;
                ins_LCD(pos_per);
                dato_LCD(personaje);
            }
        }
        if (joystick == sup_izq) {
            if (pos_per > 148 && pos_per <= 167) {
                ins_LCD(pos_per);
                dato_LCD(' ');
                pos_per += 43;
                ins_LCD(pos_per);
                dato_LCD(personaje);
            }else if ((pos_per > 192 && pos_per <= 211) || (pos_per > 212 && pos_per <= 231)) {
                ins_LCD(pos_per);
                dato_LCD(' ');
                pos_per -= 65;
                ins_LCD(pos_per);
                dato_LCD(personaje);
            }
        }
        if (joystick == inf_izq) {
            if ((pos_per > 128 && pos_per <= 147) || (pos_per > 148 && pos_per <= 167)) {
                ins_LCD(pos_per);
                dato_LCD(' ');
                pos_per += 63;
                ins_LCD(pos_per);
                dato_LCD(personaje);
            }else if (pos_per > 192 && pos_per <= 211) {
                ins_LCD(pos_per);
                dato_LCD(' ');
                pos_per -= 45;
                ins_LCD(pos_per);
                dato_LCD(personaje);
            }
        }
        if (joystick == inf_der) {
            if ((pos_per >= 128 && pos_per < 147) || (pos_per >= 148 && pos_per < 167)) {
                ins_LCD(pos_per);
                dato_LCD(' ');
                pos_per += 65;
                ins_LCD(pos_per);
                dato_LCD(personaje);
            }else if (pos_per >= 192 && pos_per < 211) {
                ins_LCD(pos_per);
                dato_LCD(' ');
                pos_per -= 43;
                ins_LCD(pos_per);
                dato_LCD(personaje);
            }
        }
        __delay_ms(250);
    }
}

//......................funciones............................................................

void conf_puertos(void) {

    TRISD = 0x00; // puerto D de salida
    LATD = 0; // limpia puerto D
    PORTD = 0; // limpia puerto D

    TRISE = 0x00; // puerto E de salida
    LATE = 0x00; // Limpia puerto 
    PORTE = 0x00; // Limpia puerto

    TRISA = 0xFF; // puerto de entrada
    LATA = 0; // limpiar puerto 
    PORTA = 0; // limpiar puerto 
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
    EN = 1; // enable en alto
    __delay_ms(1);
    EN = 0; // enable en bajo
}

//...................dato....................................

void dato_LCD(unsigned char dato) {
    RS = 1; //RS en 1 (envia dato)
    PORTD = dato; //dato a publicar 
    enable();
    __delay_ms(2); //tiempo de ejecucion
}

//...................instruccion..................................

void ins_LCD(unsigned char dato) {
    RS = 0; //RS en 0 (envia instruccion)
    PORTD = dato; //dato a publicar 
    enable();
    __delay_ms(2); //tiempo de ejecucion
}

void write_LCD(unsigned char tam) {

    unsigned char i;

    for (i = 0; i < tam; i++) {
        dato_LCD(buffer[i]);
    }
}

void conf_ADC(void) {
    ADCON0 = 0x01; //fosc/2, canal 0, on
    ADCON1 = 0x02; //just izq, an567 digitales resto analógicos
}

unsigned int conv_ADC(unsigned char canal) {
    unsigned int temp = 0;

    canal <<= 3;
    ADCON0 &= 0xC7;
    ADCON0 |= canal;
    __delay_us(50);
    GO = 1;
    while (GO == 1);

    ADRESL >>= 6;
    temp = ADRESH;
    temp <<= 2;
    temp |= ADRESL;

    return temp;
}

unsigned char leer_joystick(void) {

    unsigned int Y = 0, X = 0;

    Y = conv_ADC(0);
    X = conv_ADC(1);

    if ((Y > cota_sup)&&((X >= cota_inf)&&(X <= cota_sup))) {
        return 3;
    }
    if ((Y < cota_inf)&&((X >= cota_inf)&&(X <= cota_sup))) {
        return 7;
    }
    if ((X > cota_sup)&&((Y >= cota_inf)&&(Y <= cota_sup))) {
        return 5;
    }
    if ((X < cota_inf)&&((Y >= cota_inf)&&(Y <= cota_sup))) {
        return 1;
    }
    if ((Y > cota_sup)&&(X > cota_sup)) {
        return 4;
    }
    if ((Y < cota_inf)&&(X > cota_sup)) {
        return 6;
    }
    if ((Y < cota_inf)&&(X < cota_inf)) {
        return 8;
    }
    if ((Y > cota_sup)&&(X < cota_inf)) {
        return 2;
    }
    if (((X >= cota_inf)&&(X <= cota_sup))&&((Y >= cota_inf)&&(Y <= cota_sup))) {
        return 0;
    }
}
void crear_caracteres(void){
    ins_LCD(64);
    // primer caracter (0)
    dato_LCD(0b00000000);
    dato_LCD(0b00011011);
    dato_LCD(0b00011011);
    dato_LCD(0b00000000);
    dato_LCD(0b00000100);
    dato_LCD(0b00010001);
    dato_LCD(0b00001110);
    dato_LCD(0b00000000);
    // segundo caracter (1)
    dato_LCD(0b00000010);
    dato_LCD(0b00000010);
    dato_LCD(0b00000010);
    dato_LCD(0b00000010);
    dato_LCD(0b00000110);
    dato_LCD(0b00001110);
    dato_LCD(0b00000100);
    dato_LCD(0b00000000);
    
    ins_LCD(1);
}