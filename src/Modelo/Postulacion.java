package Modelo;

import java.util.Date;

public class Postulacion {
    private Date fecha;
    private boolean anulado;
    private Date fechaAnulacion;

    public Postulacion() {
        this.fecha = new Date();
        this.anulado = false;
        this.fechaAnulacion = null;
    }

    // Getters y Setters
    public Date getFecha() { return fecha; }
    public void setFecha(Date fecha) { this.fecha = fecha; }
    public boolean isAnulado() { return anulado; }
    public void setAnulado(boolean anulado) { this.anulado = anulado; }
    public Date getFechaAnulacion() { return fechaAnulacion; }
    public void setFechaAnulacion(Date fechaAnulacion) { this.fechaAnulacion = fechaAnulacion; }

    @Override
    public String toString() {
        return "Postulacion{fecha=" + fecha + ", anulado=" + anulado + "}";
    }
}