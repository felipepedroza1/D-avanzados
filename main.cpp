#include "mbed.h"
#include <cstdio>
#include <string>


// Blinking rate in milliseconds
#define BLINKING_RATE     5ms
#define TIEMPO_CONTEO      10s
#define TIEMPO_BYTE        1ms 


using namespace std::chrono;

UnbufferedSerial pc(USBTX, USBRX);

static char men[30];
char men_in[1];
char out[30];
char t[30];
string sensor1;
int sensor2=1;
bool potenciometro=0;
bool sensar=0; 
DigitalOut led1 (A0);
DigitalOut led2 (A1);
DigitalOut led3 (A2);
DigitalOut led4 (A3);
AnalogIn potsup (A4);
static  unsigned long long tiempo=0;

Thread hilo_envio;
Thread hilo_recibir;

Semaphore Semaforo_enviar(0);

void recibir(void);
void enviar (void);


int main()
{
    // Initialise the digital pin LED1 as an output
    
    hilo_recibir.start(recibir);
    hilo_envio.start(enviar);
    while (true) {

   if(potenciometro == 1){
       float alt =100*potsup;
       sensor2=alt;
       sprintf(t, "sa %u \n\r",sensor2);}

    ThisThread::sleep_for(BLINKING_RATE);
 
    }
}

void recibir(void)
{
    while(true)
    {
        
        pc.read(men_in,1);
        if      (!strcmp (men_in , "a" ))  {sensar=1;} 
        else if (!strcmp (men_in , "b" ))  {potenciometro=1;}
        else if(!strcmp (men_in , "c" )) {potenciometro=0;}
        else if (!strcmp (men_in , "d" )) {led1 = 1; led2=0;} 
        else if (!strcmp (men_in , "e" ))  {led1 = 0; led2=1;}
        else if (!strcmp (men_in , "f" ))  {led1 = 0; led2=0;}
        else if (!strcmp (men_in , "g" ))  {led3 = 1; led4=0;}
        else if (!strcmp (men_in , "h" ))  {led3 = 0; led4=1;}
        else if (!strcmp (men_in , "i" ))  {led3 = 0; led4=0;}
        else    pc.write("Error en el puerto\n\r", 8); 
        Semaforo_enviar.release();
        ThisThread::sleep_for(TIEMPO_BYTE);
    }
}


void enviar(void)
{
    

    while(true)
    {
          Semaforo_enviar.acquire();
          strcpy(out, t);
          pc.write(out, sizeof(out));
          Semaforo_enviar.release();
          ThisThread::sleep_for(100ms);

    }

}