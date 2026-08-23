#include "ratelimiter.hpp"
#include <iostream>
#include <chrono>

int main() {
    int C = 100;          // capacidad del bufer
    int capacidadVentana = 10;  // capacidad interna de ColaTimestamps
    long T = 10;           // duracion de la ventana en ms
    int L = 1;             // limite de paquetes en la ventana
    //std::string archivoPrueba = "tests/2-Pruebas.txt";
    std::string archivoPrueba ="data/paquetes:1000.txt";

    BuferPaquetes bufer(C);
    ColaTimestamps ventana(capacidadVentana);

    auto inicio = std::chrono::high_resolution_clock::now();
    auto stats = procesarArchivosPaquetes(archivoPrueba, bufer, ventana, T, L);
    auto fin = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duracion= fin - inicio;

    std::cout << "Procesados: " << stats.totalProcesados << "\n";
    std::cout << "Aceptados: " << stats.totalAceptados << "\n";
    std::cout << "Rechazados por bufer: " << stats.rechazadosPorBufer << "\n";
    std::cout << "Rechazados por tasa: " << stats.rechazadosPorTasa << "\n";
    std::cout << "Ocupacion maxima: " << stats.ocupacionMaxima << "\n";

    return 0;
}