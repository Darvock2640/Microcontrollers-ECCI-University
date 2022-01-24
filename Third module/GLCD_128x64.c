#include <xc.h>

#pragma config OSC = XT         // Oscillator Selection bits (XT oscillator)
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bit (Brown-out Reset disabled)
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config LVP = OFF        // Low Voltage ICSP Enable bit (Low Voltage ICSP disabled)

#define _XTAL_FREQ	4000000		// Definición del valor del cristal

#define CS1		RB0         //seleccion chip1
#define CS2		RB1         //seleccion chip2
#define EN  	RB2         //ENABLE
#define RS		RB3         //DI=RS
#define bus     LATD        //Bus de datos

unsigned char H[5]={0xFF,0x08,0x08,0x08,0xFF};
unsigned char o[5]={0x78,0x84,0x84,0x84,0x78};
unsigned char l[5]={0xFF,0,0,0,0};
unsigned char a[5]={0x78,0x84,0x84,0x48,0xFC};

unsigned char M[5]={0xFF,0x02,0x04,0x02,0xFF};
unsigned char u[5]={0x7C,0x80,0x80,0x40,0xFC};
unsigned char n[5]={0xFC,0x08,0x04,0x04,0xF8};
unsigned char d[5]={0x78,0x84,0x84,0x48,0xFF};

unsigned char i=0;

void conf_puertos(void);
void sel_CS(unsigned char CS);
void send_DATA(unsigned char dato);
void send_INST(unsigned char dato);
void set_XY(unsigned char columna, unsigned char pagina);
void clear_LCD(void);
void conf_LCD(void);

//----------------inicio del programa----------------------------

void main() {
    conf_puertos();
    conf_LCD();

    sel_CS(1);
    set_XY(0, 0);
     
    for(i=0;i<5;i++){
        send_DATA(H[i]);
    }
    send_DATA(0);
    for(i=0;i<5;i++){
        send_DATA(o[i]);
    }
    
    send_DATA(0);
    send_DATA(0xFF);
    send_DATA(0);
    
    for(i=0;i<5;i++){
        send_DATA(a[i]);
    }

    send_DATA(0);
    send_DATA(0);
    send_DATA(0);
    send_DATA(0);
    
    for(i=0;i<5;i++){
        send_DATA(M[i]);
    }

    send_DATA(0);
    
    for(i=0;i<5;i++){
        send_DATA(u[i]);
    }

    send_DATA(0);
    
    for(i=0;i<5;i++){
        send_DATA(n[i]);
    }

    send_DATA(0);
    
    for(i=0;i<5;i++){
        send_DATA(d[i]);
    }

    send_DATA(0);
    
    for(i=0;i<5;i++){
        send_DATA(o[i]);
    }

    while (1) {

    }
}

//---------------------CONFIGURA EL PIC----------------------------//

void conf_puertos(void) {

    TRISB = 0X00; //RB0->CS1 RB1->CS2 RB2->ENABLE RB3->DI/RS pines de control de los chips GLCD
    LATB = 0; //Limpiar puerto
    PORTB = 0; //Limpiar puerto

    TRISD = 0X00; //PINES DE DATOS PARA LA PANTALLA GLCD
    LATD = 0; //Limpiar puerto
    PORTD = 0; //Limpiar puerto
}

//-----------SELECCIONA LA PAGINA DE LA PANTALLLA-----------------//

void sel_CS(unsigned char CS) {
    

    if(CS == 0){
        CS1 = 0;
        CS2 = 0;
    }else if (CS == 1) {
        CS1 = 1;
        CS2 = 0;
    } else if(CS == 2){
        CS1 = 0;
        CS2 = 1;
    }else{
        CS1 = 1;
        CS2 = 1;
    }
    __delay_us(50);
}

//-------------DATO PARA LA PANTALLA----------------//
void send_DATA(unsigned char dato) {

    EN = 0;
    RS = 1; //ENVIO DATO
    bus = dato;
    __delay_us(50);
    EN = 1;
    __delay_us(50);
    EN = 0;
}

//-------------INSTRUCCIÓN PARA LA PANTALLA----------------//

void send_INST(unsigned char dato) {

    EN = 0;
    RS = 0; //ENVIO INSTRUCCION
    bus = dato;
    __delay_us(50);
    EN = 1;
    __delay_us(50);
    EN = 0;
}

//-----------SELECCIONA LA COORDENADA DE ESCRITURA----------------//

void set_XY(unsigned char columna, unsigned char pagina) {
    //columna-> determina la posicion de la columna dentro de la pagina seleccionada valor entre 0 y 63
    //pagina-> determina la pagina seleccionada valor entre 0 y 7        
    send_INST(0x40 + columna);
    send_INST(0xB8 + pagina);
}

//----------------------BORRA LA PANTALLA--------------------------//

void clear_LCD(void) {
    unsigned char columna, pagina;

    sel_CS(1);
    for (pagina = 0; pagina < 8; pagina++) {
        for (columna = 0; columna < 64; columna++) {
            set_XY(columna, pagina);
            send_DATA(0);
        }
    }

    sel_CS(2);
    for (pagina = 0; pagina < 8; pagina++) {
        //SET_XY(0,yy);
        for (columna = 0; columna < 64; columna++) {
            set_XY(columna, pagina);
            send_DATA(0);
        }
    }
}

//-------------------------CONFIGURA LA GLCD--------------------------------//

void conf_LCD(void) {
    //0X3F:encender display,0x3E:apagar display,0xC0:especificamente la primera la linea de la ram de la pantalla
    //0x40:posicionarse en la columna 0,0xB8:posicionarse en la fila 

    sel_CS(1);
    send_INST(0X3F); //enciendo la GLCD
    send_INST(0XC0); //Establecer el origen del sistema de coordenadas
    send_INST(0X40); //posiciono en la columna 0
    send_INST(0XB8); //posiciono en la fila 0

    sel_CS(2);
    send_INST(0X3F); //enciendo la GLCD
    send_INST(0XC0); //Establecer el origen del sistema de coordenadas
    send_INST(0X40); //posiciono en la columna 0
    send_INST(0XB8); //posiciono en la fila 0

    CS1 = 0;
    CS2 = 0;
}