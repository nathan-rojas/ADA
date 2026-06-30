#include "GuiApp.h"
#include "SavingsAlgorithm.h"
#include "TwoOptAlgorithm.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>

GuiApp::GuiApp() : cwDistance(0), cwTime(0), nnDistance(0), nnTime(0),twoOptDistance(0), twoOptTime(0),
                   modoActivo(0), useClarkeWright(true), lineasADibujar(0) {
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
    // 2-opt es independiente: construye su propia solución inicial y la refina
    twoOptRoutes = TwoOptAlgorithm::solveTwoOpt(customers, vehicleCapacity, twoOptDistance, twoOptTime);

    // Seleccionar la colección activa según el modo actual (0=CW, 1=NN, 2=2opt)
    const auto& activeRoutes = (modoActivo == 0) ? cwRoutes :
                               (modoActivo == 1) ? nnRoutes : twoOptRoutes;


    // Contar aristas totales para la animación por ENTER
    totalAristasSolucion = 0;
    for (const auto& route : activeRoutes) {
        if (!route.empty()) {
            totalAristasSolucion += (route.size() + 1);
        }
    }
    totalCamionesUsados = activeRoutes.size();
    naiveDistance = 0.0f;
    for (size_t i = 1; i < customers.size(); ++i) {
    // Distancia de ida y vuelta desde el depósito (0) a cada proveedor (i)
    float d = std::sqrt(std::pow(customers[i].x - customers[0].x, 2) + std::pow(customers[i].y - customers[0].y, 2));
    naiveDistance += (d * 2.0f); 
    }
    // El ahorro es simplemente la resta
    float distActiva = (modoActivo == 0) ? cwDistance :
                       (modoActivo == 1) ? nnDistance : twoOptDistance;
    totalSavingsGenerados = naiveDistance - distActiva;
    if (totalSavingsGenerados < 0) totalSavingsGenerados = 0;
}

void GuiApp::initWindow() {
    // Ventana más alta (800x950) para dar espacio a los cuadros informativos de los camiones abajo
    window.create(sf::VideoMode({800, 950}), "VRP Solver Escalable - N Camiones & Z Proveedores", sf::Style::Titlebar | sf::Style::Close);
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
                // TAB cicla entre 3 modos: 0=Clarke-Wright -> 1=Vecino Más Cercano -> 2=2-opt -> 0
                modoActivo = (modoActivo + 1) % 3;
                useClarkeWright = (modoActivo == 0);
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
    const auto& activeRoutes = (modoActivo == 0) ? cwRoutes :
                               (modoActivo == 1) ? nnRoutes : twoOptRoutes;
    
    sf::Color routeColors[] = {sf::Color::Green, sf::Color::Cyan, sf::Color::Magenta, sf::Color::Yellow, sf::Color(255, 165, 0), sf::Color(138, 43, 226)};
    int colorCount = 6;
    
    size_t lineasPintadas = 0;
    sf::Text textoMapa(font);
    textoMapa.setCharacterSize(11);
    textoMapa.setFillColor(sf::Color(170, 170, 170));

    // Arreglos para guardar dinámicamente la telemetría/cuadros de cada camión
    std::vector<float> distanciaPorCamion(activeRoutes.size(), 0.0f);
    std::vector<int> cargaPorCamion(activeRoutes.size(), 0);

    // --- PRIMERA PASADA: CALCULAR CARGAS POR RUTA ---
    int routeIdx = 0;
    for (const auto& route : activeRoutes) {
        if (route.empty()) { routeIdx++; continue; }
        for (int node : route) {
            cargaPorCamion[routeIdx] += customers[node].demand;
        }
        routeIdx++;
    }

    // --- SEGUNDA PASADA: RENDERIZAR ARISTAS PASO A PASO Y CALCULAR DISTANCIAS ---
    routeIdx = 0;
    for (const auto& route : activeRoutes) {
        if (route.empty()) { routeIdx++; continue; }
        
        sf::Color currentLineColor = routeColors[routeIdx % colorCount];

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

    // --- LOGICA DE METRICAS GENERALES Y COMPARATIVAS ---
    float totalCargaFlota = 0;
    for(int c : cargaPorCamion) totalCargaFlota += c;
    float eficienciaOcupacion = (totalCamionesUsados > 0) ? (totalCargaFlota / (totalCamionesUsados * vehicleCapacity)) * 100.0f : 0.0f;

    float distanciaActual = (modoActivo == 0) ? cwDistance :
                            (modoActivo == 1) ? nnDistance : twoOptDistance;
    double tiempoActual = (modoActivo == 0) ? cwTime :
                          (modoActivo == 1) ? nnTime : twoOptTime;                   
    float litrosConsumidos = (distanciaActual / 100.0f) * 1.5f;
    float CO2Generado = litrosConsumidos * 2.6f;

    // Cálculo dinámico de la solución Naive (Cada proveedor visitado individualmente por separado)
    float naiveDistance = 0.0f;
    for (size_t i = 1; i < customers.size(); ++i) {
        float dDepot = std::sqrt(std::pow(customers[i].x - customers[0].x, 2) + std::pow(customers[i].y - customers[0].y, 2));
        naiveDistance += (dDepot * 2.0f);
    }
    float totalSavingsGenerados = naiveDistance - distanciaActual;
    if (totalSavingsGenerados < 0.0f) totalSavingsGenerados = 0.0f;

    // Sostenibilidad Avanzada
    float co2PorUnidad = (totalCargaFlota > 0) ? (CO2Generado / totalCargaFlota) : 0.0f;
    float litrosNaive = (naiveDistance / 100.0f) * 1.5f;
    float CO2Naive = litrosNaive * 2.6f;
    float CO2Ahorrado = (CO2Naive - CO2Generado > 0) ? CO2Naive - CO2Generado : 0.0f;

    // Ratio de Compactación
    float ratioCompactacion = (totalCamionesUsados > 0) ? (float)cantidadProveedores / totalCamionesUsados : 0.0f;

    // Conteo de camiones y carga rechazada en caso de infactibilidad física
    int rutasNoAsignadasCount = 0;
    int cargaNoAtendidaTotal = 0;
    for (size_t i = 0; i < activeRoutes.size(); ++i) {
        if (!activeRoutes[i].empty() && i >= (size_t)cantidadCamionesMax) {
            rutasNoAsignadasCount++;
            cargaNoAtendidaTotal += cargaPorCamion[i];
        }
    }
    // Nombre del algoritmo activo en el panel
    std::string nombreAlgo = (modoActivo == 0) ? "CLARKE-WRIGHT SAVINGS ENGINE" :
                             (modoActivo == 1) ? "VECINO MAS CERCANO" : "BUSQUEDA LOCAL 2-OPT";
    
    // --- RE-RENDER PANEL SUPERIOR COMPACTO ---
    sf::Text uiText(font); 
    uiText.setCharacterSize(11);
    uiText.setFillColor(sf::Color::White);
    uiText.setPosition({15.0f, 5.0f});

    std::stringstream ssSuperior;
    ssSuperior << "=== PANEL ANALITICO DE LOGISTICA GLOBAL ===\n"
               << "Proveedores: " << cantidadProveedores << " | Flota Max (N): " << cantidadCamionesMax 
               << " | Camiones Req: " << totalCamionesUsados << " | Utilizacion Media Carga: " << (int)eficienciaOcupacion << "%\n"
               << "Algoritmo: " << nombreAlgo
               << " | Ratio Compactacion: " << std::fixed << std::setprecision(2) << ratioCompactacion << " prov/vco\n"
               << "Dist. Flota: " << (int)distanciaActual << " px (Ahorro vs Naive: " << (int)totalSavingsGenerados << " px) | CPU Alg: " << std::fixed << std::setprecision(2) << tiempoActual << " ms\n"
               << "DIAGNOSTICO: " << (totalCamionesUsados <= cantidadCamionesMax ? "[SOLUCION FACTIBLE]" : "[¡SOLUCION INFACTIBLE - AJUSTAR EXCESO!]") << " ";
    
    if (totalCamionesUsados > cantidadCamionesMax) {
        ssSuperior << "| Pendientes: " << rutasNoAsignadasCount << " ruta(s) | Carga Abandonada: " << cargaNoAtendidaTotal << " u\n";
    } else {
        ssSuperior << "| Toda la demanda fue cubierta con la flota dispuesta.\n";
    }

    ssSuperior << "LOGISTICA VERDE -> Consumo: " << std::fixed << std::setprecision(1) << litrosConsumidos << " L | Emisiones: " << CO2Generado << " kg CO2\n"
               << "                 > CO2/Unidad: " << std::fixed << std::setprecision(3) << co2PorUnidad << " kg/u | CO2 Evitado por ADA: " << std::fixed << std::setprecision(1) << CO2Ahorrado << " kg";
               
    uiText.setString(ssSuperior.str());
    window.draw(uiText);

// ===============================================================================
    // Separador Estético Central (Se adapta automáticamente al ancho de la ventana)
    // ===============================================================================
    float ventanaAncho = static_cast<float>(window.getSize().x);
    sf::RectangleShape lineaSeparadora({ventanaAncho, 2.0f});
    lineaSeparadora.setPosition({0.0f, 500.0f});
    lineaSeparadora.setFillColor(sf::Color(80, 80, 80));
    window.draw(lineaSeparadora);

    // ===============================================================================
    // --- CUADRÍCULA BIDIMENSIONAL EN PARES (COLUMNA IZQUIERDA Y DERECHA) ---
    // ===============================================================================
    sf::Text uiCamiones(font);
    uiCamiones.setCharacterSize(11);

    float baseY = 510.0f; // Altura inicial donde empiezan los camiones
    
    uiCamiones.setFillColor(sf::Color::White);
    uiCamiones.setString("=== TELEMETRIA EN TIEMPO REAL POR UNIDAD DE TRANSPORTE (DISTRIBUCION EN PARES) ===");
    uiCamiones.setPosition({20.0f, baseY});
    window.draw(uiCamiones);
    baseY += 20.0f;

    int indexCamionActivo = 0; // Contador secuencial para calcular las posiciones en la matriz

    for (size_t i = 0; i < activeRoutes.size(); ++i) {
        if (!activeRoutes[i].empty()) {
            bool esVehiculoExcedente = (i >= (size_t)cantidadCamionesMax);
            sf::Color colorCamion = esVehiculoExcedente ? sf::Color(240, 70, 70) : routeColors[i % colorCount]; 
            
            // --- MATEMÁTICA DE MATRIZ DINÁMICA ---
            // Dividimos el espacio disponible a la mitad
            float colAncho = ventanaAncho / 2.0f;
            
            // Si el índice es par va a la izquierda (columna 0). Si es impar va a la derecha (columna 1).
            float currentX = (indexCamionActivo % 2 == 0) ? 20.0f : (colAncho + 20.0f);
            
            // Cada dos camiones dibujados, bajamos de fila (sumamos 65 píxeles verticales)
            float currentY = baseY + (indexCamionActivo / 2) * 65.0f;

            // 1. Barra de Carga Visual Horizontal
            int porcentaje = (vehicleCapacity > 0) ? (cargaPorCamion[i] * 100) / vehicleCapacity : 0;
            int bloquesEncendidos = porcentaje / 10;
            std::string barraProgreso = "[";
            for (int b = 0; b < 10; ++b) {
                barraProgreso += (b < bloquesEncendidos) ? "X" : ".";
            }
            barraProgreso += "] " + std::to_string(porcentaje) + "%";

            // 2. Paradas e indicadores de distancias intermedias
            int paradas = activeRoutes[i].size();
            float distPromedio = (paradas > 0) ? distanciaPorCamion[i] / (paradas + 1) : 0.0f;

            // 3. Tiempo de tramo estimado
            float tiempoEstimado = distanciaPorCamion[i] / 45.0f;

            std::stringstream ssCamion;
            ssCamion << "VEHICULO " << (i + 1) << ": "
         << (esVehiculoExcedente ? "[INFACTIBLE]" : (lineasADibujar >= totalAristasSolucion ? "[COMPLETADO]" : "[EN RUTA]")) << "\n"
         << "  Carga: " << cargaPorCamion[i] << "/" << vehicleCapacity << " u. " << barraProgreso << "\n"
         << "  Tramos: " << paradas << " | Temp: " << std::fixed << std::setprecision(1) << tiempoEstimado << "s | Dist: " << (int)distanciaPorCamion[i] << "px\n"
         << "  Ruta: DEP -> ";
        for (int node : activeRoutes[i]) { ssCamion << "P" << node << " -> "; }
            ssCamion << "DEP\n"
         << "........................................................";

            uiCamiones.setFillColor(colorCamion); 
            uiCamiones.setString(ssCamion.str());
            uiCamiones.setPosition({currentX, currentY});
            window.draw(uiCamiones);
            
            indexCamionActivo++; // Incrementamos solo para los camiones que realmente tienen datos
        }
    }

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