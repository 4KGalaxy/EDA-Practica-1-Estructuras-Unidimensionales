try {
    BuferPaquetes bufer(C);
} catch (const std::invalid_argument& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
}
// esto es para despues, pero lo dejo aqui mientras.