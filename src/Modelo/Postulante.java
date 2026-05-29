package Modelo;

import java.util.ArrayList;
import java.util.Date;

public class Postulante {
    private String email;
    private String nombres;
    private String apellidos;
    private String direccion;
    private Date nacimiento;
    private String clave;
    private GradoEstudio grado;
    private ArrayList<Postulacion> postulaciones;

    public Postulante(String email, String nombres, String apellidos,
                      String direccion, Date nacimiento, String clave) {
        this.email = email;
        this.nombres = nombres;
        this.apellidos = apellidos;
        this.direccion = direccion;
        this.nacimiento = nacimiento;
        this.clave = clave;
        this.postulaciones = new ArrayList<>();
    }

    public boolean asignarGradoEstudio(GradoEstudio grado) {
        if (grado == null) return false;
        this.grado = grado;
        return true;
    }

    public boolean postular(Oferta oferta) {
        if (oferta == null) return false;
        // Verificar que no haya postulado ya a esta oferta
        // (lógica simplificada: se crea una nueva postulación)
        Postulacion p = new Postulacion();
        return postulaciones.add(p);
    }

    public boolean anularPostulacion(Postulacion postulacion) {
        if (postulacion == null || !postulaciones.contains(postulacion)) return false;
        if (postulacion.isAnulado()) return false;
        postulacion.setAnulado(true);
        postulacion.setFechaAnulacion(new Date());
        return true;
    }

    public Postulacion[] getPostulaciones() {
        return postulaciones.toArray(new Postulacion[0]);
    }

    // Getters y Setters
    public String getEmail() { return email; }
    public void setEmail(String email) { this.email = email; }
    public String getNombres() { return nombres; }
    public void setNombres(String nombres) { this.nombres = nombres; }
    public String getApellidos() { return apellidos; }
    public void setApellidos(String apellidos) { this.apellidos = apellidos; }
    public String getDireccion() { return direccion; }
    public void setDireccion(String direccion) { this.direccion = direccion; }
    public Date getNacimiento() { return nacimiento; }
    public void setNacimiento(Date nacimiento) { this.nacimiento = nacimiento; }
    public String getClave() { return clave; }
    public void setClave(String clave) { this.clave = clave; }
    public GradoEstudio getGrado() { return grado; }

    @Override
    public String toString() {
        return nombres + " " + apellidos;
    }
}