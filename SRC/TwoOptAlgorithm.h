#ifndef TWO_OPT_ALGORITHM_H
#define TWO_OPT_ALGORITHM_H

#include <vector>
#include "Customer.h"

// Algoritmo de Búsqueda Local 2-opt para el CVRP.
// Es un algoritmo INDEPENDIENTE: construye su propia solución inicial
// (igual que SavingsAlgorithm::solveClarkeWright o solveNearestNeighbor)
// y luego la refina eliminando cruces de aristas hasta alcanzar un óptimo local.
class TwoOptAlgorithm {
private:
    // Distancia euclidiana entre dos nodos
    static float calculateDistance(const Customer& c1, const Customer& c2);

    // Calcula el costo total de una ruta: DEPOT -> v1 -> v2 -> ... -> vn -> DEPOT
    static float routeCost(const std::vector<Customer>& customers, const std::vector<int>& route);

    // Construye una solución inicial PROPIA del algoritmo (criterio greedy simple
    // por cercanía y capacidad), totalmente independiente de Clarke-Wright o NN.
    static std::vector<std::vector<int>> construirSolucionInicial(
        const std::vector<Customer>& customers, int vehicleCapacity);

    // Una pasada completa de 2-opt sobre UNA ruta.
    // Evalúa todos los pares (i,j) de aristas y acepta el intercambio si Delta > 0.
    // Retorna true si hubo al menos una mejora en esta pasada.
    static bool twoOptPass(const std::vector<Customer>& customers, std::vector<int>& route);

public:
    // Función principal: construye su propia solución inicial y la refina con 2-opt
    // hasta convergencia (igual de independiente que solveClarkeWright/solveNearestNeighbor).
    static std::vector<std::vector<int>> solveTwoOpt(
        const std::vector<Customer>& customers,
        int vehicleCapacity,
        float& totalDistance,
        double& executionTime
    );
};

#endif
