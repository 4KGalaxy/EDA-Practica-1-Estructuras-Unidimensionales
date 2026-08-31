#include "ratelimiter.hpp"
#include <iostream>
#include <chrono>
#include <cstdlib>

int main(int argc, char* argv[]) {
    std::cout << "Programa iniciado. argc = " << argc << std::endl;

    if (argc < 5) {
        std::cerr << "Uso: " << argv[0] << " <archivo> <C> <T> <L>\n";
        std::cout << "Argumentos insuficientes.\n";
        return 1;
    }

    try {
        std::string archivo = argv[1];
        int C = std::atoi(argv[2]);
        long T = std::atol(argv[3]);
        int L = std::atoi(argv[4]);

        std::cout << "Parametros: C=" << C << " T=" << T << " L=" << L << std::endl;

        BuferPaquetes bufer(C);
        ColaTimestamps ventana(L);

        std::cout << "Colas creadas correctamente." << std::endl;

        auto inicio = std::chrono::high_resolution_clock::now();
        auto stats = procesarArchivosPaquetes(archivo, bufer, ventana, T, L);
        auto fin = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> duracion = fin - inicio;

        std::cout << "Archivo: " << archivo << "\n";
        std::cout << "Procesados: " << stats.totalProcesados << "\n";
        std::cout << "Aceptados: " << stats.totalAceptados << "\n";
        std::cout << "Rechazados por bufer: " << stats.rechazadosPorBufer << "\n";
        std::cout << "Rechazados por tasa: " << stats.rechazadosPorTasa << "\n";
        std::cout << "Tiempo: " << duracion.count() << " ms" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "EXCEPCION ATRAPADA: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}