from  serial import*
import sys
import time
from tkinter import *
from tkinter import ttk
import threading

serialPort =   "COM3" #"/dev/ttyACM0"
baudRate = 9600
ser = Serial(serialPort , baudRate) #ensure non-blocking

ventana = Tk()

ventana.title('primer proyecto')
ventana.geometry('400x400')

var_read_Mbed = IntVar()
read_Mbed=Scale(ventana,variable = var_read_Mbed, from_=0, to=100, orient = HORIZONTAL)


a = list()
dato = 0

def cerrar():
    ser.flush()
    ser.close()
    ventana.destroy()

def recibir():
    while (True):
        a.append(ser.readline().decode('utf-8')) 
        dato = str(a[-1])
        print(dato) 
        #var_read_Mbed.set(float (int(dato)))
        a.clear()
        time.sleep(0.5)
        
   
if __name__ == "__main__":
    hilo_recibe = threading.Thread(name = "recibiendo", target = recibir)
    read_Mbed.pack()
    close_button = Button(ventana, text="Close", command=cerrar)
    close_button.pack()
    hilo_recibe.start()
    ventana.mainloop()