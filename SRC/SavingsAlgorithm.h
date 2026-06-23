#ifndef SAVINGS_ALGORITHM_H
#define SAVINGS_ALGORITHM_H

#include <vector>
#include "Customer.h"

class SavingsAlgorithm {
private:
    // Función auxiliar para calcular la distancia euclidiana entre dos puntos
    static float calculateDistance(const Customer& c1, const Customer& c2);

public:
    // Ejecuta Clarke-Wright y devuelve un vector de rutas (cada ruta es un vector de IDs de clientes)
    static std::vector<std::vector<int>> solveClarkeWright(const std::vector<Customer>& customers, int vehicleCapacity, float& totalDistance, double& executionTime);

    // Algoritmo Secundario (Vecino más cercano básico por ruta) para fines de comparación requerida
    static std::vector<std::vector<int>> solveNearestNeighbor(const std::vector<Customer>& customers, int vehicleCapacity, float& totalDistance, double& executionTime);
};

#endif