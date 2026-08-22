#ifndef QUEUE_TIMESTAMPS_HPP 
#define QUEUE_TIMESTAMPS_HPP

class ColaTimestamps {
    public: 
    explicit ColaTimestamps(int capacidad); //en el constructor 
    ~ColaTimestamps();

    //LONG recibe por valor y no por referencia constante, long es un tipo de dato primitivo pequeño, que no vlae la pena usar referencia.
    bool enqueue(long valor);
    bool dequeue(long& resultado);
    bool front(long& resultado) const;
  
    bool isEmpty() const;
    int size() const;
    bool isFull() const;

void purgarExpirados(long tiempoActual, long ventanaT);//para mas adelante

private:
  long* datos;
  int capacidad;
  int head;
  int tail;
  int cantidad;
};

#endif