#ifndef GUI_APP_H
#define GUI_APP_H

#include <SFML/Graphics.hpp>
#include "Customer.h"
#include <vector>
#include <string>

class GuiApp {
private:
    sf::RenderWindow window;     // Ventana principal de SFML
    sf::Font font;               // Fuente tipográfica

    // === CONFIGURACIÓN LOGÍSTICA DINÁMICA ===
    int cantidadProveedores;     // 'Z' proveedores
    int cantidadCamionesMax;     // 'N' camiones disponibles en la flota
    int totalCamionesUsados;     // Cuántos camiones requirió realmente el algoritmo
    size_t lineasADibujar;       // Control de animación por ENTER
    size_t totalAristasSolucion; // Límite de la animación
    bool useClarkeWright;        // Alternar con TAB

    std::vector<Customer> customers;
    int vehicleCapacity;
    
    // Resultados de Algoritmos
    std::vector<std::vector<int>> cwRoutes;
    float cwDistance;
    double cwTime;

    std::vector<std::vector<int>> nnRoutes;
    float nnDistance;
    double nnTime;

    // Métodos internos
    void pedirDatosPorConsola(); // Captura Z, N y Capacidad antes de abrir la ventana
    void initWindow();
    void loadFont();
    void processEvents();
    void render();
    void actualizarAlgoritmos();

public:
    GuiApp();
    void run();
};

#endif