#include "ratelimiter.hpp"
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>


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

bool parsearLineaPaquete(const std::string& linea, Paquete& resultado){
    std::stringstream ss(linea);// convierte elstrign de la linea en un flujo para leer,pedacito a pedadcito
    std::string parteTimestamp, parteTamano;

    if (!std::getline(ss, parteTimestamp,  ',')){
        return false;// lee desde el flujo hasta enconrtra una coma, guardandolo en parsetimesta, la coma con comillas raras es el qeu delimita, osea , hasta donde corta

    }

    if (!std::getline(ss, parteTamano)){
        return false;// lee el tamaño del resto de la linea
    }
      

    resultado.timestamp_ms =std::stol(parteTimestamp);// convierte el texto a numero, string to long
    resultado.tamano_bytes = std::stoi(parteTamano);//convierte string to int., estos dos cositos, ignorando espacios en balcno asi que no importa
    return true;
}

    EstadisticasFirewall procesarArchivosPaquetes(const std::string& rutaArchivo, BuferPaquetes& bufer, ColaTimestamps& ventana, long T, int L) {

        EstadisticasFirewall stats;
        std::ifstream archivo(rutaArchivo);//abre el archvio para leerlo, 

        if(!archivo.is_open()){
            std::cerr << "No se pudo abrir el archivo: " << rutaArchivo << "\n";
            return stats;
        };

        std::string linea;
        while (std::getline(archivo, linea)){
            if(linea.empty()){
                continue;
            }//lee el archivo linea a linea, y termina cunado no hay mas lineas, se salta tambien lineas vacias

            Paquete p;
            if (!parsearLineaPaquete(linea, p)) {
                std::cerr << "Linea invalida, se omite: " << linea << "\n";
                continue;
            }
            ResultadoPaquete resultado = procesarPaquete(bufer, ventana, p, T, L);
            stats.totalProcesados++;

            switch (resultado.estado) {//el swtihc es la mejor forma de manejar 3 posibles valores.
                case EstadoPaquete::ACEPTADO:
                stats.totalAceptados++;
                break;
                case EstadoPaquete::RECHAZADO_BUFER_LLENO:
                stats.rechazadosPorBufer++;
                break;
                case EstadoPaquete::RECHAZADO_LIMITE_TASA:
                stats.rechazadosPorTasa++;
                break;
            }
            if (bufer.size() > stats.ocupacionMaxima) {
                stats.ocupacionMaxima = bufer.size();
            }// registra paquete a paquete, pa decir cual fue la ocupacion mas alta que tuvo el bufer en toda la ejecucion para la seccion 5
        }

        archivo.close();
        return stats;

    }

