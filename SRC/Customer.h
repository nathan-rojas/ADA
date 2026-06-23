#ifndef CUSTOMER_H
#define CUSTOMER_H

// Estructura que representa a un cliente o al depósito en el problema VRP
struct Customer {
    int id;          // Identificador único (0 suele ser el depósito)
    float x;         // Coordenada X en el plano estandarizado
    float y;         // Coordenada Y en el plano estandarizado
    int demand;      // Cantidad de carga que solicita el cliente
    bool isRouted;   // Bandera para saber si ya fue asignado a una ruta interna
};

// Estructura auxiliar para representar los "Ahorros" entre dos clientes
struct Saving {
    int i;           // Cliente i
    int j;           // Cliente j
    float savings;   // Valor del ahorro: S_ij = d_0i + d_0j - d_ij
};

#endif