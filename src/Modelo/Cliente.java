package Modelo;

import java.util.ArrayList;

public class Cliente {
    private String RUC;
    private String razonSocial;
    private String email;
    private String contacto;
    private String telefono;
    private String clave;
    private ArrayList<Oferta> ofertas;

    public Cliente(String RUC, String razonSocial, String email,
                   String contacto, String telefono, String clave) {
        this.RUC = RUC;
        this.razonSocial = razonSocial;
        this.email = email;
        this.contacto = contacto;
        this.telefono = telefono;
        this.clave = clave;
        this.ofertas = new ArrayList<>();
    }

    public boolean agregarOferta(Oferta oferta) {
        if (oferta == null) return false;
        return ofertas.add(oferta);
    }

    public boolean eliminarOferta(Oferta oferta) {
        if (oferta == null || !ofertas.contains(oferta)) return false;
        return ofertas.remove(oferta);
    }

    public Oferta[] getOfertas() {
        return ofertas.toArray(new Oferta[0]);
    }

    // Getters y Setters
    public String getRUC() { return RUC; }
    public void setRUC(String RUC) { this.RUC = RUC; }
    public String getRazonSocial() { return razonSocial; }
    public void setRazonSocial(String razonSocial) { this.razonSocial = razonSocial; }
    public String getEmail() { return email; }
    public void setEmail(String email) { this.email = email; }
    public String getContacto() { return contacto; }
    public void setContacto(String contacto) { this.contacto = contacto; }
    public String getTelefono() { return telefono; }
    public void setTelefono(String telefono) { this.telefono = telefono; }
    public String getClave() { return clave; }
    public void setClave(String clave) { this.clave = clave; }

    @Override
    public String toString() {
        return razonSocial + " (RUC: " + RUC + ")";
    }
}