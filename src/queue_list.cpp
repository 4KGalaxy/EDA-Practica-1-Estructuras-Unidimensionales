#include "queue_list.hpp"

ColaTimestampsLista::ColaTimestampsLista() {
    head = nullptr;
    tail = nullptr;
    cantidad = 0;
}
 ColaTimestampsLista::~ColaTimestampsLista(){
    NodoTimestamp* actual = head;
    while (actual !=nullptr){
        NodoTimestamp* siguiente = actual -> siguiente;
        delete actual;
        actual = siguiente;
        //Se guarda el acutal en el siguente para antes de hacer el delete acutal, por que si se borra el nodo ya no se puede leer nada de el, liberando memoria en el proceso.
    }   
    
 }

   bool ColaTimestampsLista::isEmpty() const {
            return cantidad == 0;
        }
        int ColaTimestampsLista::size() const{
            return cantidad;
        }


bool ColaTimestampsLista::enqueue(long valor){
    NodoTimestamp* nuevo = new NodoTimestamp;// se reserva memoria para un solo nodo, no un arreglo por eso sin corchetes
    nuevo ->valor = valor;// se guarda el dato -> me indica como accedo a los campos de una struct a travez de un puntero
    nuevo ->siguiente = nullptr;// este nodo va a ser el ultimo por ahora asi que no apunta a nada aun,

    if(isEmpty()){
        head=nuevo;// si esta vacia el nuevo nodo debe ser el el primero, asi qeu la cabeza apunta hacia el.
    }else {
        tail->siguiente= nuevo;//si ya habia elementos, engancha el nuevo nod despues del que habia sido el ultimo (tail) conectando la cadena
    }
    tail=nuevo; // da igual que caso pase, el nuevo nodo sera el ultimo
    cantidad++;//denota un elemento mas, pero no falla, ya que no hay lista llena, como pasa en un arreglo
    return true;    
}
//justo aquí está una diferencia real entre representaciones que puedes usar en la sección 6/10: la lista enlazada nunca rechaza un enqueue por falta de espacio (mientras haya memoria del sistema), mientras que el arreglo circular sí tiene ese límite físico — trade-off entre flexibilidad y control de recursos.

bool ColaTimestampsLista::dequeue(long& resultado){
    if(isEmpty()){
        return false;
    }

    NodoTimestamp* viejo = head;//se gudarda un puntero hacia el  nodo que se va a sacar, antes de mover nada
    resultado = viejo->valor;//se copia el dato hacia afuer
    head = head->siguiente;//avanza hacea el siguiente noda

    if( head == nullptr){
 tail = nullptr;
    }// si al sacar el ultimo no head esta en null, entonces tail tambien debe quedar en nullptr para que tail no apunte a un nodo ya liberado, osea puntero colgante, 

    delete viejo;
    cantidad--;
    return true;

}

bool ColaTimestampsLista::front(long& resultado) const{
    if(isEmpty()){
        return false;
    }
    resultado = head->valor;
    return true;
}
//solo lee lo que tenga head.

void ColaTimestampsLista::purgarExpirados(long tiempoActual, long ventanaT){
    long limite= tiempoActual - ventanaT;
    long valorFrente;

    while (!isEmpty() && front(valorFrente) && valorFrente < limite) {
            dequeue(valorFrente);

    }
}