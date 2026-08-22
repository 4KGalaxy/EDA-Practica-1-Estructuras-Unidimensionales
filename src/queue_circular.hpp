//Zaimon
// la clase, sus métodos, sus atributos — pero sin el cuerpo de las funciones (o solo con cuerpos muy simples). Es lo que otros archivos incluyen con #include para saber que esa clase existe y cómo usarla

//FIFO --- First in, First out

//enqueue(x) --- inserta x al final --- O(1)
//dequeue(x) --- elimina y retona al frente --- la cola no puede estar vacia --- O(1)
//peel(x)    --- consulta el frente sin eliminar -- la cola no puede estar vacia
//isEmpty()  --- esta vacia?
//size ()    --- cuantos elementos hay

#ifndef QUEUE_CIRCULAR_HPP 
#define QUEUE_CIRCULAR_HPP
//EVITA QUE EL ARCHIVO SE INCLUYA DOS VECES SI SE INPORTAN DE DOS .cpp

struct Paquete {
    long timestamp_ms;
    int tamano_bytes;
};

class BuferPaquetes {
    public: 
    explicit BuferPaquetes(int capacidad); //en el constructor evita conversaciones explicitas raras, 
    ~BuferPaquetes();

    bool enqueue(const Paquete& p);
    bool dequeue(Paquete& resultado);
    bool front(Paquete& resultado) const;
    //y se habla en boleanos, F o V,
    // reciben un resutlado por referencia(Paquete&), asi se evita un "paquete vacio por defecto" cuando la cola esta vacia
    
    bool isEmpty() const;
    int size() const;
    bool isFull() const;
// sirve para las estadisitcas del reporte final, osea de la ocupacion del bufer
private:
  Paquete* datos;
  int capacidad;
  int head;
  int tail;
  int cantidad;

};

#endif