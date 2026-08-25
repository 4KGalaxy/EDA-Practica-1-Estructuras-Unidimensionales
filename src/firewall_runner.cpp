#include "ratelimiter.hpp"
#include <filesystem>
#include <iostream>
#include <string>
#include "firewall_runner.hpp"

void correrProblema2(){
    //cuerpo
    int i = 1, op;
    std::string files[10];

    std::cout << "--- ARCHIVOS EN /tests ---\n";
    for (const auto& entry : std::filesystem::directory_iterator("tests")) {
        if (entry.path().filename().string().rfind("p2_", 0) == 0 && entry.path().extension() == ".txt") {
            files[i] = entry.path().string();
            std::cout << " [" << i << "] " << files[i] << "\n";
            i++;
        }
    }
    std::cout << "Selecciona un Caso";
    std::cin >> op;
    if(op < 1 || op >= i){
        std::cout << "Opcion invalida. \n";
        return;
    }

    int C,T,L;

    if (files[op].find("caso1") != std::string::npos) { 
   C=100;
   T=10;
   L=5;
    }else if (files[op].find("caso2") != std::string::npos){
   C=100;
   T=10;
   L=1;
    }else if(files[op].find("caso3") !=std::string::npos){
   C=2;
   T=10;
   L=5;
    }else if(files[op].find("caso4") != std::string::npos){
   C=100;
   T=50;
   L=3;
    }else if(files[op].find("caso5") != std::string::npos){
   C=100;
   T=10;
   L=1;
    }else if(files[op].find("caso7") != std::string::npos){
   C=100;
   T=10;
   L=5;
    }else {
    std::cout <<"No se pudo determinar los parametros. \n";
    return;
    }

    

BuferPaquetes bufer(C);
ColaTimestamps ventana(L);

auto stats = procesarArchivosPaquetes(files[op], bufer, ventana, T, L);

std::cout << "Procesados: " << stats.totalProcesados << "\n";
std::cout << "Aceptados: " << stats.totalAceptados << "\n";
std::cout << "Rechazados por bufer: " << stats.rechazadosPorBufer << "\n";
std::cout << "Rechazados por tasa: " << stats.rechazadosPorTasa << "\n";
std::cout << "Ocupacion maxima: " << stats.ocupacionMaxima << "\n";
}