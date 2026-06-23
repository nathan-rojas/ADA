#include "GuiApp.h"
#include "SavingsAlgorithm.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>

GuiApp::GuiApp() : cwDistance(0), cwTime(0), nnDistance(0), nnTime(0), useClarkeWright(true), lineasADibujar(0) {
    std::srand(std::time(nullptr)); // Semilla para generación aleatoria escalable
    pedirDatosPorConsola();
    actualizarAlgoritmos();
}

void GuiApp::pedirDatosPorConsola() {
    std::cout << "=========================================================\n";
    std::cout << "   CONFIGURADOR LOGISTICO VRP - N CAMIONES Y Z PROVEEDORES\n";
    std::cout << "=========================================================\n\n";

    std::cout << "1. Ingrese la cantidad de PROVEEDORES (Z): ";
    std::cin >> cantidadProveedores;
    if (cantidadProveedores < 1) cantidadProveedores = 5;

    std::cout << "2. Ingrese la cantidad MAXIMA DE CAMIONES (N): ";
    std::cin >> cantidadCamionesMax;
    if (cantidadCamionesMax < 1) cantidadCamionesMax = 3;

    std::cout << "3. Ingrese la CAPACIDAD de carga por camion (unidades): ";
    std::cin >> vehicleCapacity;
    if (vehicleCapacity < 1) vehicleCapacity = 100;

    char opcion;
    std::cout << "4. ¿Desea generar las posiciones y cargas automaticamente? (s/n): ";
    std::cin >> opcion;

    customers.clear();
    // El deposito central siempre es el punto [0] fijado en el centro del mapa
    customers.push_back({0, 400.0f, 200.0f, 0, false});

    if (opcion == 'n' || opcion == 'N') {
        for (int i = 1; i <= cantidadProveedores; ++i) {
            float x, y;
            int demanda;
            std::cout << "\n--- Proveedor P" << i << " ---\n";
            std::cout << "Coordenada X (Recomendado 50 a 750): ";
            std::cin >> x;
            std::cout << "Coordenada Y (Recomendado 50 a 350): ";
            std::cin >> y;
            std::cout << "Demanda de carga: ";
            std::cin >> demanda;
            customers.push_back({i, x, y, demanda, false});
        }
    } else {
        // Generación matemática aleatoria bien distribuida en el plano espacial de SFML
        for (int i = 1; i <= cantidadProveedores; ++i) {
            float x = 80.0f + static_cast<float>(std::rand() % 640);
            float y = 60.0f + static_cast<float>(std::rand() % 280);
            int demanda = 10 + (std::rand() % 35); // Demandas de entre 10u y 45u
            customers.push_back({i, x, y, demanda, false});
        }
    }
}

void GuiApp::actualizarAlgoritmos() {
    cwRoutes = SavingsAlgorithm::solveClarkeWright(customers, vehicleCapacity, cwDistance, cwTime);
    nnRoutes = SavingsAlgorithm::solveNearestNeighbor(customers, vehicleCapacity, nnDistance, nnTime);
    
    // Contar aristas totales para la animación por ENTER
    totalAristasSolucion = 0;
    const auto& activeRoutes = useClarkeWright ? cwRoutes : nnRoutes;
    for (const auto& route : activeRoutes) {
        if (!route.empty()) {
            totalAristasSolucion += (route.size() + 1);
        }
    }
    totalCamionesUsados = activeRoutes.size();
}

void GuiApp::initWindow() {
    // Ventana más alta (800x750) para dar espacio a los cuadros informativos de los camiones abajo
    window.create(sf::VideoMode({800, 750}), "VRP Solver Escalable - N Camiones & Z Proveedores", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60); 
}

void GuiApp::loadFont() {
    auto loadedFont = sf::Font();
    if (loadedFont.openFromFile("arial.ttf")) {
        font = std::move(loadedFont);
    } else {
        std::cout << "[Error] No se pudo cargar arial.ttf" << std::endl;
    }
}

void GuiApp::processEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::Tab) {
                useClarkeWright = !useClarkeWright;
                lineasADibujar = 0; 
                actualizarAlgoritmos();
            }
            else if (keyPressed->code == sf::Keyboard::Key::Enter) {
                if (lineasADibujar < totalAristasSolucion) {
                    lineasADibujar++;
                }
            }
        }
    }
}

void GuiApp::render() {
    window.clear(sf::Color(20, 20, 20)); 

    float Y_OFFSET = 120.0f; // Ajuste para desplazar el mapa debajo del panel principal de métricas
    const auto& activeRoutes = useClarkeWright ? cwRoutes : nnRoutes;
    
    sf::Color routeColors[] = {sf::Color::Green, sf::Color::Cyan, sf::Color::Magenta, sf::Color::Yellow, sf::Color(255, 165, 0), sf::Color(138, 43, 226)};
    int colorCount = 6;
    
    size_t lineasPintadas = 0;
    sf::Text textoMapa(font);
    textoMapa.setCharacterSize(11);
    textoMapa.setFillColor(sf::Color(170, 170, 170));

    // Arreglo para guardar dinámicamente la telemetría/cuadros de cada camión
    std::vector<std::string> reporteCamiones(activeRoutes.size(), "No asignado");
    std::vector<float> distanciaPorCamion(activeRoutes.size(), 0.0f);
    std::vector<int> cargaPorCamion(activeRoutes.size(), 0);

    int routeIdx = 0;
    for (const auto& route : activeRoutes) {
        if (route.empty()) { routeIdx++; continue; }
        
        sf::Color currentLineColor = routeColors[routeIdx % colorCount];

        // Calcular datos reales acumulados para los cuadros informativos inferiores
        for (int node : route) {
            cargaPorCamion[routeIdx] += customers[node].demand;
        }

        // 1. Depósito -> Primer Nodo
        sf::Vector2f pDepot(customers[0].x, customers[0].y + Y_OFFSET);
        if (lineasPintadas < lineasADibujar) {
            sf::Vector2f pFirst(customers[route[0]].x, customers[route[0]].y + Y_OFFSET);
            sf::Vertex l[] = { {pDepot, currentLineColor}, {pFirst, currentLineColor} };
            window.draw(l, 2, sf::PrimitiveType::Lines);

            float d = std::sqrt(std::pow(pFirst.x - pDepot.x, 2) + std::pow(pFirst.y - pDepot.y, 2));
            distanciaPorCamion[routeIdx] += d;

            textoMapa.setString(std::to_string((int)d) + "px");
            textoMapa.setPosition({(pDepot.x + pFirst.x)/2, (pDepot.y + pFirst.y)/2});
            window.draw(textoMapa);
            lineasPintadas++;
        }

        // 2. Entre Clientes intermediarios
        for (size_t i = 0; i < route.size() - 1; ++i) {
            sf::Vector2f pA(customers[route[i]].x, customers[route[i]].y + Y_OFFSET);
            sf::Vector2f pB(customers[route[i+1]].x, customers[route[i+1]].y + Y_OFFSET);
            
            float d = std::sqrt(std::pow(pB.x - pA.x, 2) + std::pow(pB.y - pA.y, 2));
            if (lineasPintadas < lineasADibujar) {
                distanciaPorCamion[routeIdx] += d;
            }

            if (lineasPintadas < lineasADibujar) {
                sf::Vertex l[] = { {pA, currentLineColor}, {pB, currentLineColor} };
                window.draw(l, 2, sf::PrimitiveType::Lines);

                textoMapa.setString(std::to_string((int)d) + "px");
                textoMapa.setPosition({(pA.x + pB.x)/2, (pA.y + pB.y)/2});
                window.draw(textoMapa);
                lineasPintadas++;
            }
        }

        // 3. Último Nodo -> Depósito
        if (lineasPintadas < lineasADibujar) {
            sf::Vector2f pLast(customers[route.back()].x, customers[route.back()].y + Y_OFFSET);
            sf::Vertex l[] = { {pLast, currentLineColor}, {pDepot, currentLineColor} };
            window.draw(l, 2, sf::PrimitiveType::Lines);

            float d = std::sqrt(std::pow(pDepot.x - pLast.x, 2) + std::pow(pDepot.y - pLast.y, 2));
            distanciaPorCamion[routeIdx] += d;

            textoMapa.setString(std::to_string((int)d) + "px");
            textoMapa.setPosition({(pLast.x + pDepot.x)/2, (pLast.y + pDepot.y)/2});
            window.draw(textoMapa);
            lineasPintadas++;
        }

        // Construir string formateado del CUADRO DEL CAMIÓN solicitado
        std::stringstream ssReporte;
        ssReporte << "CAMION " << (routeIdx + 1) << ": ";
        if (lineasPintadas > 0 && lineasADibujar > (lineasPintadas - route.size() - 1)) {
            ssReporte << "[EN RUTA] ";
        } else if (lineasADibujar >= lineasPintadas) {
            ssReporte << "[COMPLETADO] ";
        } else {
            ssReporte << "[EN ESPERA] ";
        }
        
        ssReporte << "| Carga: " << cargaPorCamion[routeIdx] << "/" << vehicleCapacity << " un. "
                  << "| Dist: " << (int)distanciaPorCamion[routeIdx] << " px\n"
                  << "   Recorrido: DEPOT -> ";
        for (int node : route) { ssReporte << "P" << node << " -> "; }
        ssReporte << "DEPOT";
        
        reporteCamiones[routeIdx] = ssReporte.str();
        routeIdx++;
    }

    // Dibujar nodos (Puntos blancos de Proveedores y Rojo de Depósito)
    for (const auto& c : customers) {
        sf::CircleShape nodeCircle(8.0f); 
        nodeCircle.setOrigin({8.0f, 8.0f}); 
        nodeCircle.setPosition({c.x, c.y + Y_OFFSET});

        if (c.id == 0) {
            nodeCircle.setFillColor(sf::Color::Red);
            window.draw(nodeCircle);
            textoMapa.setString("DEPOT");
            textoMapa.setFillColor(sf::Color::Red);
            textoMapa.setPosition({c.x - 15.0f, c.y + Y_OFFSET - 22.0f});
            window.draw(textoMapa);
        } else {
            nodeCircle.setFillColor(sf::Color::White);
            window.draw(nodeCircle);
            textoMapa.setString("P" + std::to_string(c.id) + " (" + std::to_string(c.demand) + "u)");
            textoMapa.setFillColor(sf::Color::White);
            textoMapa.setPosition({c.x - 20.0f, c.y + Y_OFFSET - 22.0f});
            window.draw(textoMapa);
        }
    }

    // Panel Superior: Métricas Generales del Sistema
    sf::Text uiText(font); 
    uiText.setCharacterSize(12);
    uiText.setFillColor(sf::Color::White);
    uiText.setPosition({15.0f, 10.0f});

    float totalCargaFlota = 0;
    for(int c : cargaPorCamion) totalCargaFlota += c;
    float eficienciaOcupacion = (totalCamionesUsados > 0) ? (totalCargaFlota / (totalCamionesUsados * vehicleCapacity)) * 100.0f : 0.0f;

    std::stringstream ssSuperior;
    ssSuperior << "=== SISTEMA VRP GLOBAL (SCALABLE ENGINE) ===\n"
               << "Proveedores (Z): " << cantidadProveedores << " | Flota Disp (N): " << cantidadCamionesMax 
               << " | Camiones Requeridos: " << totalCamionesUsados << " | Eficiencia de Carga: " << (int)eficienciaOcupacion << "%\n"
               << "Controles: [ENTER] Tramos Paso a Paso (" << lineasADibujar << "/" << totalAristasSolucion << ") | [TAB] Alternar Algoritmo\n"
               << "Algoritmo Activo: " << (useClarkeWright ? "CLARKE-WRIGHT SAVINGS ENGINE" : "VECINO MAS CERCANO") << "\n"
               << "Distancia Combinada Flota: " << (useClarkeWright ? cwDistance : nnDistance) << " px | Tiempo de Calculo: " << (useClarkeWright ? cwTime : nnTime) << " ms";
    uiText.setString(ssSuperior.str());
    window.draw(uiText);

    // Separador Estético Central para delimitar el Mapa del Panel de Camiones Inferior
    sf::RectangleShape lineaSeparadora({800.0f, 2.0f});
    lineaSeparadora.setPosition({0.0f, 500.0f});
    lineaSeparadora.setFillColor(sf::Color(80, 80, 80));
    window.draw(lineaSeparadora);

    // === CUADROS INFERIORES: ESTADO EN TIEMPO REAL DE CADA CAMIÓN ===
    sf::Text uiCamiones(font);
    uiCamiones.setCharacterSize(12);
    uiCamiones.setPosition({20.0f, 515.0f});

    std::stringstream ssInferior;
    ssInferior << "=== TELEMETRIA DE CUADROS POR VEHICULO (ESTADO DE LA FLOTA BAJO DEMANDA) ===\n\n";
    
    // Alerta crítica si el algoritmo requiere más camiones de los que configuraste por consola (N)
    if (totalCamionesUsados > cantidadCamionesMax) {
        ssInferior << "¡ALERTA LOGISTICA! Se requieren " << totalCamionesUsados 
                   << " camiones para la demanda actual, pero solo dispone de N=" << cantidadCamionesMax << ".\n\n";
    }

    for (size_t i = 0; i < reporteCamiones.size(); ++i) {
        if (i < 3) { // Limitar dibujo visual a los primeros 3 camiones para no desbordar la pantalla
            ssInferior << reporteCamiones[i] << "\n---------------------------------------------------------------------------------\n";
        }
    }
    if(reporteCamiones.size() > 3) {
        ssInferior << "... y otros " << (reporteCamiones.size() - 3) << " camiones operando rutas adicionales.";
    }

    uiCamiones.setString(ssInferior.str());
    
    // Cambiar dinámicamente el color del cuadro inferior para identificar el algoritmo en ejecución
    uiCamiones.setFillColor(useClarkeWright ? sf::Color(144, 238, 144) : sf::Color(173, 216, 230)); 
    window.draw(uiCamiones);

    window.display(); 
}

void GuiApp::run() {
    initWindow();
    loadFont();
    while (window.isOpen()) {
        processEvents();
        render();
    }
}