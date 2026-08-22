#include "ratelimiter.hpp"

ResultadoPaquete procesarPaquete(BuferPaquetes& bufer, ColaTimestamps& ventana, const Paquete& p, long T, int L){
    ventana.purgarExpirados(p.timestamp_ms,T); // priemro se purga, siemrpe antes de decidir nada, se usa el timestamp para el paqeute llegar en tiemmpo actual

    ResultadoPaquete resultado;
    resultado.paquete = p; // se arma la respuesta desde ya, guardando el original package

    if(bufer.isFull()){
        resultado.estado = EstadoPaquete:: RECHAZADO_BUFER_LLENO;
        return resultado;
    }//¿por que revisar primero lleno y luego tasa, y no al reves? 

    if(ventana.size() >= L){
        resultado.estado = EstadoPaquete::RECHAZADO_LIMITE_TASA;
        return resultado;
    }//si ya hay L o timesatpms vigentes, se rechaza por exceso de tasa

    bufer.enqueue(p);
    ventana.enqueue(p.timestamp_ms);
    resultado.estado = EstadoPaquete::ACEPTADO;
    return resultado;

    // si pasa ambos chequeos bufer.enqueue(p); guarda en el bufer, ventana.enqueue(p.timestamps_ms);registra su llegada para futuros chequos de tasa y marcas el resultado como ACEPTADO.
}// En esta secciion no verificamso bools, ya que en estos momentos ya garantizamos con los otrso IF que ninguna de las colas estan llenas, por ende es un caso de 100% de exito,
