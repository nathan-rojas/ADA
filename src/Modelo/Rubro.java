package Modelo;

public class Rubro {
    private String nombre;
    private boolean estado;

    public Rubro(String nombre) {
        this.nombre = nombre;
        this.estado = true;
    }

    public boolean habilitar() {
        if (!this.estado) {
            this.estado = true;
            return true;
        }
        return false;
    }

    public boolean deshabilitar() {
        if (this.estado) {
            this.estado = false;
            return true;
        }
        return false;
    }

    // Getters y Setters
    public String getNombre() { return nombre; }
    public void setNombre(String nombre) { this.nombre = nombre; }
    public boolean isEstado() { return estado; }
}