package Modelo;

import java.util.ArrayList;
import java.util.Date;

public class Oferta {
    private String puesto;
    private String descripcion;
    private String area;
    private Date fechaInicio;
    private Date fechaTermino;
    private ArrayList<Requisito> requisitos;

    public Oferta(String puesto, String descripcion, String area,
                  Date fechaInicio, Date fechaTermino) {
        this.puesto = puesto;
        this.descripcion = descripcion;
        this.area = area;
        this.fechaInicio = fechaInicio;
        this.fechaTermino = fechaTermino;
        this.requisitos = new ArrayList<>();
    }

    public boolean agregarRequisito(int orden, String descripcion) {
        // Verificar que no exista el mismo orden
        for (Requisito r : requisitos) {
            if (r.getOrden() == orden) return false;
        }
        return requisitos.add(new Requisito(orden, descripcion));
    }

    public Requisito[] getRequisitos() {
        return requisitos.toArray(new Requisito[0]);
    }

    public boolean eliminarRequisito(int orden) {
        for (Requisito r : requisitos) {
            if (r.getOrden() == orden) {
                return requisitos.remove(r);
            }
        }
        return false;
    }

    // Getters y Setters
    public String getPuesto() { return puesto; }
    public void setPuesto(String puesto) { this.puesto = puesto; }
    public String getDescripcion() { return descripcion; }
    public void setDescripcion(String descripcion) { this.descripcion = descripcion; }
    public String getArea() { return area; }
    public void setArea(String area) { this.area = area; }
    public Date getFechaInicio() { return fechaInicio; }
    public void setFechaInicio(Date fechaInicio) { this.fechaInicio = fechaInicio; }
    public Date getFechaTermino() { return fechaTermino; }
    public void setFechaTermino(Date fechaTermino) { this.fechaTermino = fechaTermino; }

    @Override
    public String toString() {
        return puesto + " - " + area;
    }
}