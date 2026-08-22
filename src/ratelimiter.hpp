// LOGIC 2ND PROBLEM
#ifndef RATELIMITER_HPP
#define RATELIMITER_HPP

#include "queue_circular.hpp"
#include "queue_timestamps.hpp"

enum class EstadoPaquete {
    ACEPTADO,
    RECHAZADO_BUFER_LLENO,
    RECHAZADO_LIMITE_TASA
};

struct ResultadoPaquete {// agrupa el paquete original juntando con su resultado, para que quien llame  la funcion tenga todo ahi mismo
    Paquete paquete;
    EstadoPaquete estado;

};

ResultadoPaquete procesarPaquete(BuferPaquetes& bufer, ColaTimestamps& ventana, const Paquete& p, long T, int L); // la funcion recibe por referencia (&) y const por qeu procesarpaquete va a modificarlas.

#endif