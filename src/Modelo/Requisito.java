package Modelo;

public class Requisito {
    private int orden;
    private String descripcion;
    private boolean estado;

    public Requisito(int orden, String descripcion) {
        this.orden = orden;
        this.descripcion = descripcion;
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
    public int getOrden() { return orden; }
    public void setOrden(int orden) { this.orden = orden; }
    public String getDescripcion() { return descripcion; }
    public void setDescripcion(String descripcion) { this.descripcion = descripcion; }
    public boolean isEstado() { return estado; }
}