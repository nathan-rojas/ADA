package Modelo;

import java.util.Date;
import Vista.vista;

public class main {
    public static void main(String[] args) {

        // --- Crear Rubros ---
        Rubro rubro1 = new Rubro("Tecnología");
        Rubro rubro2 = new Rubro("Finanzas");

        // --- Crear GradoEstudio ---
        GradoEstudio grado1 = new GradoEstudio("Universitario");
        GradoEstudio grado2 = new GradoEstudio("Técnico");

        // --- Crear Cliente ---
        Cliente cliente = new Cliente(
            "20501234567", "Tech Corp SAC",
            "tech@corp.com", "Juan Pérez",
            "999888777", "clave123"
        );

        // --- Crear Oferta y agregarla al Cliente ---
        Oferta oferta = new Oferta(
            "Desarrollador Java", "Se busca dev Java senior",
            "TI", new Date(), new Date()
        );
        oferta.agregarRequisito(1, "3 años de experiencia en Java");
        oferta.agregarRequisito(2, "Conocimiento en Spring Boot");
        cliente.agregarOferta(oferta);

        // --- Crear Postulante ---
        Postulante postulante = new Postulante(
            "ana@gmail.com", "Ana", "García",
            "Av. Lima 123", new Date(), "pass456"
        );
        postulante.asignarGradoEstudio(grado1);
        postulante.postular(oferta);

        // --- Mostrar resultados ---
        System.out.println("=== CLIENTE ===");
        System.out.println(cliente);
        System.out.println("Ofertas: " + cliente.getOfertas().length);

        System.out.println("\n=== OFERTA ===");
        System.out.println(oferta);
        for (Requisito r : oferta.getRequisitos()) {
            System.out.println("  Req " + r.getOrden() + ": " + r.getDescripcion());
        }

        System.out.println("\n=== POSTULANTE ===");
        System.out.println(postulante);
        System.out.println("Grado: " + postulante.getGrado());
        System.out.println("Postulaciones: " + postulante.getPostulaciones().length);
        
        java.awt.EventQueue.invokeLater(() -> {
            new vista().setVisible(true); 
        });
        
        System.out.println("\nVentana iniciada correctamente.");
    }
    
}