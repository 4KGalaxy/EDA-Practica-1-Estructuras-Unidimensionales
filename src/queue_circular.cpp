// .cpp → es la implementación real. Aquí va el código que realmente hace el trabajo, el cuerpo de cada método.
#include "queue_circular.hpp"
#include <stdexcept>
//el cpp sabe qeu es buferpaqutes y paqute , para que el compliar pueda reocnocer
BuferPaquetes::BuferPaquetes(int capacidad) // es como definnes fuera del header, el cuerpo de un metodo declarado en el .hpp el :: se llama operado de resolucion de ambito
 {
    if(capacidad <= 0)
    {
        throw std::invalid_argument("La Capacidad del buefer debe ser un entero positivo");     
    }
    this->capacidad = capacidad;
    // como el parametreo y el atributo se llaman igual this declara que el de la izquierda es el atributo de la clase, no el parametro
    datos = new Paquete[capacidad];
    // aqui se reserva el bloque de memoria crudo tal comom lo pide el enunciado, osea este es el arreglo de tamñaño C.
    head = 0;
    tail = 0;
    cantidad = 0;
    //arranca con cola vacia, con los indices apuntando al inicio,
}

BuferPaquetes::~BuferPaquetes() {
    delete[] datos;
}

bool BuferPaquetes::isEmpty() const {
    return cantidad == 0;
}

bool BuferPaquetes::isFull() const {
    return cantidad == capacidad;
}

int BuferPaquetes::size() const {
    return cantidad;
}
// el const, indica que no se modifica ningun atributo del objeto, solo lo consulta

bool BuferPaquetes::enqueue(const Paquete& p) 
// recibe el paquete por referencia constante, no por copia para no copiar toda la struct, cada que se calle enqueue
{
    if(isFull()){
        return false;
    }
    datos[tail] = p;
    tail = (tail + 1)% capacidad;
    capacidad++;
    return true;
}
//si tail estaba en la ultima posicion capacidad -1 sumarle 1 te da la capacidad, que esta afuera del arregllo. El %capacidad lo envuelve de vuelta a 0, ej capacidad=5: si tail=4, entonces (4+1)% 5=0
//esta operacion es O(1) en el peor caso, siempre - no hay ningun escenario que tome mas tiempo qeu otro, a diferencia del push con redimensionamiento de la pila, por eso es mas facil analizar una cola circula fija

bool BuferPaquetes::dequeue(Paquete& resultado) {// aqui si es referenica sin const, porque vas a escribir esta variable para devolver, el paqeute extraido, asi como se utiliza en enqueue/front, osea no se retona un paqeute pero si un bool de exito utilizando un parametro de salia, asi se evita tener que inventar unn paquete vacio
    if(isEmpty()) { 
        return false;
    }
    resultado=datos[head];// se copia el paquete que esta al frente hacia la varibale de salida antes de mover el indice
    head =(head + 1)% capacidad;
    cantidad--;
    return true;
}

bool BuferPaquetes::front(Paquete& resultado) const {//conts por fuera es para que reciba el dato y lo cambie, , si estuviera adentro del paqeute resultado no seria capas de cambiar lo de afuera que ps no es lo que necesitasmos
    if(isEmpty()){
        return false;
    }
    resultado = datos[head];
    return true;
}