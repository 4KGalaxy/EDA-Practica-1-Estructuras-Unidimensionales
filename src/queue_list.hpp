//Zaimon
#ifndef QUEUE_LIST_HPP 
#define QUEUE_LIST_HPP


struct NodoTimestamp {
    long valor;
    NodoTimestamp* siguiente;
};

class ColaTimestampsLista {
    public:
    ColaTimestampsLista();
    ~ColaTimestampsLista();

    bool enqueue(long valor);
    bool dequeue(long& resultado);
    bool front(long& resultado) const;
    
    bool isEmpty() const;
    int size() const;
    void purgarExpirados(long tiempoActual, long ventanaT);

private:
    NodoTimestamp* head;
    NodoTimestamp* tail;
    int cantidad;


    ColaTimestampsLista(const ColaTimestampsLista&) = delete;
    ColaTimestampsLista& operator=(const ColaTimestampsLista&) = delete;
};

#endif

//no tiene isfull, ya que una lista enlazada no tiene un limite de capacidad fija, por eso tampoco recibe capacidad en el constructor.
//no hay arreglo datos, ya qeu guarda dso punteros head y tail, mas el contador cantidad.
//=delete es para que no creen copias que apuntan a los mismos nodos, y si se elimian un a de las colas, la otra queda con punteros colgantes, y eso es un error. 