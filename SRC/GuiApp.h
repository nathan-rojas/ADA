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
    float naiveDistance = 0.0f;       // Distancia enviando un camión por cada proveedor
    float totalSavingsGenerados = 0.0f; // Ahorro total (Clarke-Wright)
    int totalSavingsEvaluados = 0;   // Cuántos pares i,j se analizaron
    int totalSavingsAceptados = 0;   // Cuántas fusiones pasaron los filtros de capacidad

    // Modo activo: 0=Clarke-Wright  1=Vecino Más Cercano  2=2-opt
    int modoActivo;

    std::vector<Customer> customers;
    int vehicleCapacity;
    
    // Resultados Clarke-Wright
    std::vector<std::vector<int>> cwRoutes;
    float cwDistance;
    double cwTime;

    // Resultados Vecino Más Cercano
    std::vector<std::vector<int>> nnRoutes;
    float nnDistance;
    double nnTime;

    // Resultados 2-opt 
    std::vector<std::vector<int>> twoOptRoutes;
    float twoOptDistance;
    double twoOptTime;


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