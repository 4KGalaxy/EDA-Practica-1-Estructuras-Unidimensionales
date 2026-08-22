#include "queue_timestamps.hpp"
#include <stdexcept>

ColaTimestamps::ColaTimestamps(int capacidad) 
{   
    if(capacidad <= 0)
    {
        throw std::invalid_argument("La Capacidad de cola de timestamps debe ser un entero positivo");     
    }
    this->capacidad = capacidad;
    datos = new long[capacidad];
    head = 0;
    tail = 0;
    cantidad = 0;

}

ColaTimestamps::~ColaTimestamps() {
    delete[] datos;
}

 bool ColaTimestamps::isEmpty() const {
    return cantidad == 0;
 }

 int ColaTimestamps::size() const {
    return cantidad;
 }
 bool ColaTimestamps::isFull() const {
    return cantidad == capacidad;
 }

 bool ColaTimestamps::enqueue(long valor) {
    if(isFull()){
        return false;
    }
    datos[tail] = valor;
    tail = (tail + 1)% capacidad;
    cantidad++;
    return true;
}

bool ColaTimestamps::dequeue(long&resultado) {
    if(isEmpty()) { 
        return false;
    }
    resultado=datos[head];
    head =(head + 1)% capacidad;
    cantidad--;
    return true;
}

bool ColaTimestamps::front(long& resultado) const { 
    if(isEmpty()){
        return false;
    }
    resultado = datos[head];
    return true;
}

void ColaTimestamps::purgarExpirados(long tiempoActual, long ventanaT) {
    long limite = tiempoActual - ventanaT; //se calcula el punto de corte para que expiren los timestamp anteriores 
    long ValorFrente;

        while(!isEmpty() && front(ValorFrente) && ValorFrente < limite){
            dequeue(ValorFrente);
            // se saca el timestap, expirado ya que no esta vacio por el anterior while.
        }
        //mientras la cola no este vacia y el valorfrente sea mas viejo que el limite, se sigue purgando,  front(VF) llena la variable y retornan true/false
}