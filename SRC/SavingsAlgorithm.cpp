#include "SavingsAlgorithm.h"
#include <cmath>
#include <algorithm>
#include <chrono>

// Calcula la distancia en línea recta entre dos nodos usando Pitágoras
float SavingsAlgorithm::calculateDistance(const Customer& c1, const Customer& c2) {
    return std::sqrt(std::pow(c1.x - c2.x, 2) + std::pow(c1.y - c2.y, 2));
}

// Implementación del Algoritmo de Ahorros de Clarke-Wright (CORREGIDO Y ESTRICTO)
std::vector<std::vector<int>> SavingsAlgorithm::solveClarkeWright(const std::vector<Customer>& customers, int vehicleCapacity, float& totalDistance, double& executionTime) {
    auto start = std::chrono::high_resolution_clock::now();
    
    int n = customers.size();
    totalDistance = 0;
    
    if (n <= 1) return {}; 

    const Customer& depot = customers[0]; 
    std::vector<Saving> savingsList;     

    // 1. Calcular los ahorros S_ij para cada par de clientes (i, j)
    for (int i = 1; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            float d_0i = calculateDistance(depot, customers[i]); 
            float d_0j = calculateDistance(depot, customers[j]); 
            float d_ij = calculateDistance(customers[i], customers[j]); 
            
            float s_ij = d_0i + d_0j - d_ij; 
            savingsList.push_back({i, j, s_ij});
        }
    }

    // 2. Ordenar los ahorros en orden descendente
    std::sort(savingsList.begin(), savingsList.end(), [](const Saving& a, const Saving& b) {
        return a.savings > b.savings;
    });

    // Inicializar: cada cliente empieza en su propia ruta exclusiva
    std::vector<std::vector<int>> routes;
    for (int i = 1; i < n; ++i) {
        routes.push_back({i});
    }

    // 3. Fusionar rutas basándonos en la lista ordenada de ahorros
    for (const auto& s : savingsList) {
        int routeIdxI = -1;
        int routeIdxJ = -1;

        // Buscar a qué rutas pertenecen actualmente los clientes 'i' y 'j'
        for (size_t r = 0; r < routes.size(); ++r) {
            if (!routes[r].empty()) {
                if (routes[r].front() == s.i || routes[r].back() == s.i) {
                    if (routeIdxI == -1) routeIdxI = r; 
                }
                if (routes[r].front() == s.j || routes[r].back() == s.j) {
                    if (routeIdxJ == -1) routeIdxJ = r; 
                }
            }
        }

        // Condición de fusión: Deben estar en rutas distintas y válidas
        if (routeIdxI != -1 && routeIdxJ != -1 && routeIdxI != routeIdxJ) {
            
            // --- CORRECCIÓN CRÍTICA DE CAPACIDAD ---
            // Calculamos la demanda total de la ruta I sumando sus nodos reales
            int demandaRutaI = 0;
            for (int node : routes[routeIdxI]) demandaRutaI += customers[node].demand;

            // Calculamos la demanda total de la ruta J sumando sus nodos reales
            int demandaRutaJ = 0;
            for (int node : routes[routeIdxJ]) demandaRutaJ += customers[node].demand;

            // Solo si la unión NO supera el límite real de carga, procedemos
            if (demandaRutaI + demandaRutaJ <= vehicleCapacity) {
                
                bool merged = false;
                // Caso A: 'i' es el final de su ruta y 'j' es el inicio de la suya
                if (routes[routeIdxI].back() == s.i && routes[routeIdxJ].front() == s.j) {
                    routes[routeIdxI].insert(routes[routeIdxI].end(), routes[routeIdxJ].begin(), routes[routeIdxJ].end());
                    merged = true;
                }
                // Caso B: 'j' es el final de su ruta e 'i' es el inicio de la suya
                else if (routes[routeIdxJ].back() == s.j && routes[routeIdxI].front() == s.i) {
                    routes[routeIdxJ].insert(routes[routeIdxJ].end(), routes[routeIdxI].begin(), routes[routeIdxI].end());
                    routes[routeIdxI] = routes[routeIdxJ]; 
                    routeIdxJ = routeIdxI; 
                    merged = true;
                }
                // Caso C: Ambos son extremos iniciales
                else if (routes[routeIdxI].front() == s.i && routes[routeIdxJ].front() == s.j) {
                    std::reverse(routes[routeIdxI].begin(), routes[routeIdxI].end());
                    routes[routeIdxI].insert(routes[routeIdxI].end(), routes[routeIdxJ].begin(), routes[routeIdxJ].end());
                    merged = true;
                }
                // Caso D: Ambos son extremos finales
                else if (routes[routeIdxI].back() == s.i && routes[routeIdxJ].back() == s.j) {
                    std::reverse(routes[routeIdxJ].begin(), routes[routeIdxJ].end());
                    routes[routeIdxI].insert(routes[routeIdxI].end(), routes[routeIdxJ].begin(), routes[routeIdxJ].end());
                    merged = true;
                }

                if (merged) {
                    // Vaciamos de forma limpia la ruta vieja que fue absorbida
                    routes[routeIdxJ].clear();
                }
            }
        }
    }

    // Limpiar el contenedor eliminando las rutas vacías
    std::vector<std::vector<int>> finalRoutes;
    for (const auto& route : routes) {
        if (!route.empty()) {
            finalRoutes.push_back(route);
            
            float currentDist = calculateDistance(depot, customers[route.front()]); 
            for (size_t k = 0; k < route.size() - 1; ++k) {
                currentDist += calculateDistance(customers[route[k]], customers[route[k+1]]);
            }
            currentDist += calculateDistance(customers[route.back()], depot); 
            totalDistance += currentDist;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    executionTime = std::chrono::duration<double, std::milli>(end - start).count();

    return finalRoutes;
}

// ... (Aquí abajo dejas tu función solveNearestNeighbor tal cual la tenías, no necesita cambios)
// Algoritmo de comparación secundario: Vecino Más Cercano (Greedy)
std::vector<std::vector<int>> SavingsAlgorithm::solveNearestNeighbor(const std::vector<Customer>& customers, int vehicleCapacity, float& totalDistance, double& executionTime) {
    auto start = std::chrono::high_resolution_clock::now();
    
    int n = customers.size();
    totalDistance = 0;
    if (n <= 1) return {};

    std::vector<bool> visited(n, false);
    visited[0] = true; // El depósito está "visitado" inicialmente
    std::vector<std::vector<int>> routes;

    int visitedCount = 1;
    while (visitedCount < n) {
        std::vector<int> currentRoute;
        int currentCapacity = 0;
        int currentIdx = 0; // Inicia en el depósito

        while (true) {
            int nextIdx = -1;
            float minDist = 1e9;

            // Buscar el vecino más cercano no visitado que cumpla con la restricción de capacidad
            for (int i = 1; i < n; ++i) {
                if (!visited[i] && (currentCapacity + customers[i].demand <= vehicleCapacity)) {
                    float d = calculateDistance(customers[currentIdx], customers[i]);
                    if (d < minDist) {
                        minDist = d;
                        nextIdx = i;
                    }
                }
            }

            // Si no se encuentra ningún cliente apto, el vehículo regresa al depósito
            if (nextIdx == -1) {
                if (!currentRoute.empty()) {
                    totalDistance += calculateDistance(customers[currentIdx], customers[0]);
                }
                break;
            }

            // Añadir el cliente seleccionado a la ruta actual
            currentRoute.push_back(nextIdx);
            visited[nextIdx] = true;
            currentCapacity += customers[nextIdx].demand;
            totalDistance += minDist;
            currentIdx = nextIdx;
            visitedCount++;
        }
        if (!currentRoute.empty()) {
            routes.push_back(currentRoute);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    executionTime = std::chrono::duration<double, std::milli>(end - start).count();
    return routes;
}