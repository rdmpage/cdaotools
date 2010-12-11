/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * ImagePanel.java
 *
 * Created on Nov 5, 2010, 9:49:32 PM
 */

package phylipalrequin;

import java.awt.Image;
import javax.swing.ImageIcon;

/**
 *
 * @author bchisham
 */
public class ImagePanel extends javax.swing.JPanel {

    private Image bgImage;
    private final String IMAGE_LOCATION = "resources/importer_background.png";

    /** Creates new form ImagePanel */
    public ImagePanel() {
        initComponents();
        bgImage = new ImageIcon(IMAGE_LOCATION).getImage();
    }

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        setMinimumSize(new java.awt.Dimension(800, 474));
        setName("Form"); // NOI18N
        setPreferredSize(new java.awt.Dimension(800, 474));

        org.jdesktop.layout.GroupLayout layout = new org.jdesktop.layout.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(0, 800, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(0, 474, Short.MAX_VALUE)
        );
    }// </editor-fold>//GEN-END:initComponents


    // Variables declaration - do not modify//GEN-BEGIN:variables
    // End of variables declaration//GEN-END:variables

}