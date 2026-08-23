#include <iostream>
#include <fstream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    int n=1000;
    unsigned int semilla=42;
 if (argc >= 2){//argc-argv permite pasar n y la semilla como argumentos de linea de comandos al ejecutar el programa.
    n = std::atoi(argv[1]);
}
if (argc >= 3) {
   semilla = std::atoi(argv[2]);
}
std::srand(semilla);//fija la semilla del generador de numeros pseudoaleatorios, con la misma semilla std::radn(9 siemrpe da la misma semilla, por ende da reproducibilidad)

std::string nombreArchivo ="data/paquetes_" + std::to_string(n) + ".txt";//convierte n de int a string, y arma el nombre completo
std::ofstream archivo(nombreArchivo);
if(!archivo.is_open()){
    std::cerr <<"No se puede crear el archivod de salida.\n";
    return 1;
}
long timestamp=0;
for (int i=0; i<n; i++){ 
 timestamp += std::rand() % 20+1;// cada paqeute llega entre 1 y 20 ms despues del anterior 
 int tamano =std::rand() % 1500 + 64;// tamaño de paqeute entre 64 y 1563 bytes....ajustable
 archivo << timestamp << ", " << tamano << "\n";// coincide como el timestamp, tamaño que se acordo.
}
archivo.close();
std::cout << "Generados " << n << " paquetes en " << nombreArchivo << "(semilla = " << semilla << ")\n";
return 0;
}