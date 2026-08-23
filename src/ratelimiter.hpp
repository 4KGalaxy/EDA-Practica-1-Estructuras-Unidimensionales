// LOGIC 2ND PROBLEM
#ifndef RATELIMITER_HPP
#define RATELIMITER_HPP

#include "queue_circular.hpp"
#include "queue_timestamps.hpp"
#include <string>

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

struct EstadisticasFirewall {
    int totalProcesados = 0;
    int totalAceptados = 0;
    int rechazadosPorBufer = 0;
    int rechazadosPorTasa = 0;
    int ocupacionMaxima = 0;
};// asi cada estadisticas empieza en en cero, para no iniciarlas a manao siempre

EstadisticasFirewall procesarArchivosPaquetes(const std::string& rutaArchivo, BuferPaquetes& bufer, ColaTimestamps& ventana, long T, int L);

bool parsearLineaPaquete(const std::string& linea, Paquete& resultado);

#endif