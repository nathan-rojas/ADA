package Modelo;

import Vista.FrmPrincipal;
import javax.swing.UIManager;

public class main {

    public static void main(String[] args) {
 
        try {
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        } catch (Exception e) {
        }

        java.awt.EventQueue.invokeLater(() -> {
            FrmPrincipal principal = new FrmPrincipal();
            
            principal.setLocationRelativeTo(null);

            principal.setVisible(true);
        });
    }
}