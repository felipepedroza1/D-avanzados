
#include "mbed.h"
#include <cstdio>


// Blinking rate in milliseconds
#define BLINKING_RATE     500ms
#define TIEMPO_CONTEO      10s
#define TIEMPO_BYTE        1ms 


using namespace std::chrono;

UnbufferedSerial pc(USBTX, USBRX);

static char men[30];

Timer t;

static  unsigned long long tiempo=0;

Thread hilo_formateo;
Thread hilo_envio;

Semaphore Semaforo_enviar(0);

void formatear(void);
void enviar (void);


int main()
{
    // Initialise the digital pin LED1 as an output
    DigitalOut led(LED1);
    hilo_formateo.start(formatear);
    hilo_envio.start(enviar);
    t.reset();
    t.start();
    printf("Arranque del programa \n\r");
    t.stop();
    tiempo = t.elapsed_time().count();
    printf("Tiempo printf: %llu micros\n\r", tiempo);

    //pc.attach(&enviar, SerialBase::TxIrq);
    while (true) {
        led = !led;
        ThisThread::sleep_for(BLINKING_RATE);
    }
}

void formatear(void)
{
    int conta=0;
    while(true)
    {
        t.reset();
        t.start();
        sprintf(men, "El valor es: %d \n",conta);
        conta++;
        t.stop();
        tiempo = t.elapsed_time().count();
        printf("Tiempo formateo: %llu micros\n\r", tiempo);
        Semaforo_enviar.release();
        ThisThread::sleep_for(TIEMPO_CONTEO);

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
        Semaforo_enviar.acquire();
        c= men[i];
        i++;
        pc.write(&c,1);
        t.stop();
        tiempo = t.elapsed_time().count();
        printf("Tiempo envio byte i:%u ::%llu micros\n\r", i, tiempo);
        ThisThread::sleep_for(TIEMPO_BYTE);
        if (c /= '\n') Semaforo_enviar.release();
        else i=0;

    }

}

