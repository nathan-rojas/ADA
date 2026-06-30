#include "TwoOptAlgorithm.h"
#include <cmath>
#include <algorithm>
#include <chrono>

// Distancia euclidiana entre dos nodos
float TwoOptAlgorithm::calculateDistance(const Customer& c1, const Customer& c2) {
    return std::sqrt(std::pow(c1.x - c2.x, 2) + std::pow(c1.y - c2.y, 2));
}

// Costo total de una ruta incluyendo salida y retorno al depósito (customers[0])
float TwoOptAlgorithm::routeCost(const std::vector<Customer>& customers, const std::vector<int>& route) {
    if (route.empty()) return 0.0f;
    const Customer& depot = customers[0];

    float cost = calculateDistance(depot, customers[route[0]]);
    for (size_t k = 0; k < route.size() - 1; ++k) {
        cost += calculateDistance(customers[route[k]], customers[route[k + 1]]);
    }
    cost += calculateDistance(customers[route.back()], depot);
    return cost;
}

// ============================================================
// CONSTRUCCIÓN DE SOLUCIÓN INICIAL PROPIA (independiente de CW y NN)
// Criterio: recorrido secuencial simple por ID de cliente, agrupando
// en una misma ruta mientras no se exceda la capacidad del vehículo.
// Esta es la "materia prima" sobre la que el propio 2-opt trabajará
// para demostrar su capacidad de mejora por sí solo.
// ============================================================
std::vector<std::vector<int>> TwoOptAlgorithm::construirSolucionInicial(
    const std::vector<Customer>& customers, int vehicleCapacity)
{
    std::vector<std::vector<int>> routes;
    int n = static_cast<int>(customers.size());
    if (n <= 1) return routes;

    std::vector<int> rutaActual;
    int cargaActual = 0;

    // Recorre los clientes en su orden natural (1..n-1), abriendo una
    // nueva ruta cada vez que la capacidad del vehículo se agotaría.
    for (int i = 1; i < n; ++i) {
        int demanda = customers[i].demand;

        if (cargaActual + demanda > vehicleCapacity) {
            // Cerrar la ruta actual y empezar una nueva
            if (!rutaActual.empty()) {
                routes.push_back(rutaActual);
            }
            rutaActual.clear();
            cargaActual = 0;
        }

        rutaActual.push_back(i);
        cargaActual += demanda;
    }

    if (!rutaActual.empty()) {
        routes.push_back(rutaActual);
    }

    return routes;
}

// ============================================================
// Una pasada completa de 2-opt sobre UNA ruta.
// Para cada par de aristas (A1->A2) y (B1->B2):
//   Delta = d(A1,A2) + d(B1,B2) - d(A1,B1) - d(A2,B2)
//   Si Delta > 0 => invertir el segmento [i..j] mejora la ruta.
// ============================================================
bool TwoOptAlgorithm::twoOptPass(const std::vector<Customer>& customers, std::vector<int>& route) {
    int n = static_cast<int>(route.size());
    if (n < 2) return false;

    bool improved = false;

    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {

            // Nodo antes de i: depósito si i es el primero
            const Customer& nodeA1 = (i == 0) ? customers[0] : customers[route[i - 1]];
            const Customer& nodeA2 = customers[route[i]];
            const Customer& nodeB1 = customers[route[j]];
            // Nodo después de j: depósito si j es el último
            const Customer& nodeB2 = (j == n - 1) ? customers[0] : customers[route[j + 1]];

            float costActual = calculateDistance(nodeA1, nodeA2) + calculateDistance(nodeB1, nodeB2);
            float costNuevo  = calculateDistance(nodeA1, nodeB1) + calculateDistance(nodeA2, nodeB2);

            // Delta positivo = el intercambio reduce la distancia => aceptar
            if (costActual - costNuevo > 1e-6f) {
                std::reverse(route.begin() + i, route.begin() + j + 1);
                improved = true;
            }
        }
    }
    return improved;
}

// ============================================================
// Función pública principal. Algoritmo INDEPENDIENTE:
// 1) Construye su propia solución inicial (no usa CW ni NN).
// 2) Aplica 2-opt iterativamente sobre cada ruta hasta convergencia.
// ============================================================
std::vector<std::vector<int>> TwoOptAlgorithm::solveTwoOpt(
    const std::vector<Customer>& customers,
    int vehicleCapacity,
    float& totalDistance,
    double& executionTime)
{
    auto start = std::chrono::high_resolution_clock::now();

    totalDistance = 0.0f;

    // 1) Solución inicial propia del algoritmo
    std::vector<std::vector<int>> routes = construirSolucionInicial(customers, vehicleCapacity);

    // 2) Refinamiento: 2-opt ruta por ruta hasta que ninguna mejora sea posible
    for (auto& route : routes) {
        bool improved = true;
        while (improved) {
            improved = twoOptPass(customers, route);
        }
    }

    // Recalcular distancia total sobre las rutas ya optimizadas
    for (const auto& route : routes) {
        if (!route.empty()) {
            totalDistance += routeCost(customers, route);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    executionTime = std::chrono::duration<double, std::milli>(end - start).count();
    return routes;
}