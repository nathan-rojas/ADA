/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/GUIForms/JFrame.java to edit this template
 */
package Vista;

import javax.swing.*;
import java.awt.*;

public class FrmPrincipal extends javax.swing.JFrame {
    public FrmPrincipal() {
        
        setTitle("Sistema de Reclutamiento TI");
        setSize(800, 600);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        JMenuBar menuBar = new JMenuBar();
        JMenu menuEmpresa = new JMenu("Empresa");
        JMenuItem itemRegEmpresa = new JMenuItem("Registrar");
        JMenuItem itemLoginEmpresa = new JMenuItem("Login");
        
        menuEmpresa.add(itemRegEmpresa);
        menuEmpresa.add(itemLoginEmpresa);

        JMenu menuCandidato = new JMenu("Candidato");
        JMenuItem itemRegCandidato = new JMenuItem("Registrar");
        JMenuItem itemLoginCandidato = new JMenuItem("Login");
        
        menuCandidato.add(itemRegCandidato);
        menuCandidato.add(itemLoginCandidato);
        menuBar.add(menuEmpresa);
        menuBar.add(menuCandidato);

        setJMenuBar(menuBar);

        itemLoginEmpresa.addActionListener(e -> abrirLogin("Empresa"));
        itemLoginCandidato.addActionListener(e -> abrirLogin("Candidato"));
 
        itemRegCandidato.addActionListener(e -> {
        VistaPrincipal ventanaRegistro = new VistaPrincipal(); 
        ventanaRegistro.setVisible(true);
        });
        
        itemRegEmpresa.addActionListener(e -> {
        VistaPrincipal ventanaEmpresa = new VistaPrincipal();
        ventanaEmpresa.setLocationRelativeTo(null); // Para que salga al centro
        ventanaEmpresa.setVisible(true);
    });
    }
    private void abrirLogin(String tipoInicial) {
        new FrmLogin(tipoInicial).setVisible(true);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new FrmPrincipal().setVisible(true));
    }

    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 400, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 300, Short.MAX_VALUE)
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents


    // Variables declaration - do not modify//GEN-BEGIN:variables
    // End of variables declaration//GEN-END:variables
}
