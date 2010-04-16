/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * MatrixViewerDisplay.java
 *
 * Created on Apr 16, 2010, 12:59:30 PM
 */

package matrixviewer;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;
import matrixviewer.model.Matrix;
import matrixviewer.model.MatrixFactory;
import matrixviewer.view.MatrixView;
import org.jdesktop.application.Action;

/**
 *
 * @author bchisham
 */
public class MatrixViewerDisplay extends javax.swing.JFrame {
    private static String ftype= "csv";
    Matrix matrix;
    /** Creates new form MatrixViewerDisplay */
    public MatrixViewerDisplay() {
        initComponents();

    }

    public MatrixViewerDisplay(String urlOrFile){
        initComponents();
        try {
            URL url = new URL(urlOrFile);
            openFile(url);
        } catch (MalformedURLException ex) {
            File file = new File( urlOrFile );
            openFile(file);
        }


    }

    @Action
    private void openFile(){

    }

    private void openFile(URL url){
        try {
            this.matrix = MatrixFactory.getMatrix( ftype );
            this.matrix.read(url.openStream());
            ((MatrixView)this.matrix_canvas).setMatrix(matrix);
            KeyDisplay kd = new KeyDisplay( matrix );
            kd.setVisible( true );
            
            //this.repaint();

        } catch (IOException ex) {
            Logger.getLogger(MatrixViewerDisplay.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    private void openFile(File file){
        try {
            this.matrix = MatrixFactory.getMatrix(ftype);
            this.matrix.read(new FileInputStream(file));
            ((MatrixView)this.matrix_canvas).setMatrix(matrix);
            //this.repaint();
        } catch (FileNotFoundException ex) {
            Logger.getLogger(MatrixViewerDisplay.class.getName()).log(Level.SEVERE, null, ex);
        }
    }



    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jLabel1 = new javax.swing.JLabel();
        matrixviewpane = new javax.swing.JScrollPane();
        matrix_canvas = new MatrixView( matrix );
        mainMenuBar = new javax.swing.JMenuBar();
        fileMenu = new javax.swing.JMenu();
        openMenuItem = new javax.swing.JMenuItem();
        closeMenuItem = new javax.swing.JMenuItem();

        setName("Form"); // NOI18N

        org.jdesktop.application.ResourceMap resourceMap = org.jdesktop.application.Application.getInstance(matrixviewer.MatrixViewerApp.class).getContext().getResourceMap(MatrixViewerDisplay.class);
        jLabel1.setFont(resourceMap.getFont("jLabel1.font")); // NOI18N
        jLabel1.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        jLabel1.setText(resourceMap.getString("jLabel1.text")); // NOI18N
        jLabel1.setName("jLabel1"); // NOI18N

        matrixviewpane.setName("matrixviewpane"); // NOI18N

        matrix_canvas.setName("matrix_canvas"); // NOI18N
        matrixviewpane.setViewportView(matrix_canvas);

        mainMenuBar.setName("mainMenuBar"); // NOI18N

        fileMenu.setText(resourceMap.getString("fileMenu.text")); // NOI18N
        fileMenu.setName("fileMenu"); // NOI18N

        openMenuItem.setText(resourceMap.getString("openMenuItem.text")); // NOI18N
        openMenuItem.setName("openMenuItem"); // NOI18N
        openMenuItem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                openMenuItemActionPerformed(evt);
            }
        });
        fileMenu.add(openMenuItem);

        closeMenuItem.setText(resourceMap.getString("closeMenuItem.text")); // NOI18N
        closeMenuItem.setName("closeMenuItem"); // NOI18N
        fileMenu.add(closeMenuItem);

        mainMenuBar.add(fileMenu);

        setJMenuBar(mainMenuBar);

        org.jdesktop.layout.GroupLayout layout = new org.jdesktop.layout.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(org.jdesktop.layout.GroupLayout.TRAILING, layout.createSequentialGroup()
                .addContainerGap()
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.TRAILING)
                    .add(org.jdesktop.layout.GroupLayout.LEADING, matrixviewpane, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 624, Short.MAX_VALUE)
                    .add(org.jdesktop.layout.GroupLayout.LEADING, jLabel1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 624, Short.MAX_VALUE))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(layout.createSequentialGroup()
                .addContainerGap()
                .add(jLabel1)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(matrixviewpane, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 406, Short.MAX_VALUE)
                .addContainerGap())
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void openMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_openMenuItemActionPerformed
        this.openFile();
    }//GEN-LAST:event_openMenuItemActionPerformed

    /**
    * @param args the command line arguments
    */
    public static void main(String args[]) {
        
        if (args.length <= 1){
            java.awt.EventQueue.invokeLater(new Runnable() {
                public void run() {
                    new MatrixViewerDisplay().setVisible(true);
                }
            });
        }
        else {
            final String urlarg = args[1];
            java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new MatrixViewerDisplay( urlarg ).setVisible(true);
            }
        });
        }
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JMenuItem closeMenuItem;
    private javax.swing.JMenu fileMenu;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JMenuBar mainMenuBar;
    private java.awt.Canvas matrix_canvas;
    private javax.swing.JScrollPane matrixviewpane;
    private javax.swing.JMenuItem openMenuItem;
    // End of variables declaration//GEN-END:variables

}
