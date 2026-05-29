package Modelo;

public class GradoEstudio {
    private String descripcion;

    public GradoEstudio(String descripcion) {
        this.descripcion = descripcion;
    }

    // Getters y Setters
    public String getDescripcion() { return descripcion; }
    public void setDescripcion(String descripcion) { this.descripcion = descripcion; }

    @Override
    public String toString() {
        return descripcion;
    }
}
