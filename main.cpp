#include "mbed.h"
#include <cstdio>


// Blinking rate in milliseconds
#define BLINKING_RATE     5ms
#define TIEMPO_CONTEO      10s
#define TIEMPO_BYTE        1ms 


using namespace std::chrono;

UnbufferedSerial pc(USBTX, USBRX);

static char men[30];
char men_in[1];
Timer t;
DigitalOut led1 (A0);
DigitalOut led2 (A1);
DigitalOut led3 (A2);
DigitalOut led4 (A3);
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
    
    while (true) {
    ThisThread::sleep_for(BLINKING_RATE);
 
    }
}

void recibir(void)
{
    while(true)
    {
        pc.read(men_in,1);
        if      (!strcmp (men_in , "a" ))  {} 
        else if (!strcmp (men_in , "b" ))  {}
        else if(!strcmp (men_in , "c" )) {}
        else if (!strcmp (men_in , "d" )) {led1 = 1; led2=0;} 
        else if (!strcmp (men_in , "e" ))  {led1 = 0; led2=1;}
        else if (!strcmp (men_in , "f" ))  {led1 = 0; led2=0;}
        else if (!strcmp (men_in , "g" ))  {led3 = 1; led4=0;}
        else if (!strcmp (men_in , "h" ))  {led3 = 0; led4=1;}
        else if (!strcmp (men_in , "i" ))  {led3 = 0; led4=0;}
        else    pc.write("Error\n\r", 8); 
        Semaforo_enviar.release();
        ThisThread::sleep_for(TIEMPO_BYTE);
    }
}


void enviar(void)
{
    char c;
    int i=0;
    while(true)
    {
        t.reset();
        t.start();
        
        c= men[i];
        i++;
        pc.write(&c,1);
        t.stop();
        tiempo = t.elapsed_time().count();
        printf("Tiempo envio byte i:%u ::%llu micros %hhu\n\r", i, tiempo, c);
        ThisThread::sleep_for(TIEMPO_BYTE);
        if (c /= '\n') Semaforo_enviar.release();
        else i=0;

    }

}