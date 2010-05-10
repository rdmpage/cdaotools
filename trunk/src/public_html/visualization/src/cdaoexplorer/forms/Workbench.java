
/*
 * Workbench.java
 *
 * Created on Apr 28, 2010, 10:08:01 PM
 */

package cdaoexplorer.forms;

import cdaoexplorer.forms.dialogs.ErrorReportDialog;
import cdaoexplorer.forms.dialogs.FileDialog;
import java.io.File;
import java.net.MalformedURLException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JFrame;
import matrixviewer.CDAOMatrixChooser;
import org.jdesktop.application.Action;
import prefuse.demos.CDAOTreeChooser;



/**
 *
 * @author bchisham
 */
public class Workbench extends javax.swing.JFrame {
    private boolean treeVisible;
    private boolean matrixVisible;
    CDAOTreeChooser treeChooser;
    CDAOMatrixChooser matrixChooser;
    FileDialog fileChooser;
    ErrorReportDialog errorDialog;
    /** Creates new form Workbench */
    public Workbench() {
        initComponents();
        this.treeVisible = false;
        this.matrixVisible = false;
        treeChooser = new CDAOTreeChooser();
        treeChooser.setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE );
        matrixChooser = new CDAOMatrixChooser();
        matrixChooser.setDefaultCloseOperation( JFrame.HIDE_ON_CLOSE );
        fileChooser = new FileDialog( this, true );
        this.errorDialog = new ErrorReportDialog( this, false );
        this.errorDialog.setDefaultCloseOperation( JFrame.HIDE_ON_CLOSE );
    }
    @Action
    public void doOpenTree(){
        this.fileChooser.setVisible( true );
        File tree = this.fileChooser.getSelectedFile();
        if ( tree != null){
            try {
                this.treeChooser.doOpenFile(tree.toURL());
            } catch (MalformedURLException ex) {
                errorDialog.setRequestURI( tree.toString() );
                errorDialog.setExtraInfo(ex);
                errorDialog.setVisible( true );
                Logger.getLogger(Workbench.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }

    @Action
    public void doOpenMatrix(){
        this.fileChooser.setVisible( true );
        File matrixFile = this.fileChooser.getSelectedFile();
        if ( matrixFile != null){
            try {
                this.matrixChooser.doOpenFile(matrixFile.toURL());
            } catch (MalformedURLException ex) {
                errorDialog.setRequestURI( matrixFile.toString() );
                errorDialog.setExtraInfo(ex);
                errorDialog.setVisible( true );
                Logger.getLogger(Workbench.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }

    @Action
    public void handleSelectTreeAction(){
        this.treeVisible = !this.treeVisible;
        this.treeChooser.setVisible(treeVisible);
    }

    @Action
    public void handleSelectMatrixAction(){
        this.matrixVisible = !this.matrixVisible;
        this.matrixChooser.setVisible(matrixVisible);
        //this.matrixChooser.setDefaultCloseOperation( JFrame.DISPOSE_ON_CLOSE);
    }

    public void treeChooserClosing(){
        this.treeVisible = false;
    }

    public void matrixChooserClosing(){
        this.matrixVisible = false;
    }

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        iconLabel = new javax.swing.JLabel();
        titleLabel = new javax.swing.JLabel();
        selectTreeToggleButton = new javax.swing.JToggleButton();
        selectMatrixToggleButton = new javax.swing.JToggleButton();
        jMenuBar1 = new javax.swing.JMenuBar();
        fileMenu = new javax.swing.JMenu();
        openMatrixMenuItem = new javax.swing.JMenuItem();
        openTreeMenuItem = new javax.swing.JMenuItem();
        openWorkspaceMenuItem = new javax.swing.JMenuItem();
        saveWorkspaceMenuItem = new javax.swing.JMenuItem();
        exitMenuItem = new javax.swing.JMenuItem();
        helpMenu = new javax.swing.JMenu();
        aboutMenuItem = new javax.swing.JMenuItem();
        reportErrorMenuItem = new javax.swing.JMenuItem();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("CDAO Explorer");

        iconLabel.setIcon(new javax.swing.ImageIcon(getClass().getResource("/cdaoexplorer/forms/cdao-triplestore-logo.jpg"))); // NOI18N

        titleLabel.setFont(new java.awt.Font("Lucida Grande", 1, 24));
        titleLabel.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        titleLabel.setText("CDAO Explorer");
        titleLabel.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);

        selectTreeToggleButton.setText("Select Tree");
        selectTreeToggleButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                selectTreeToggleButtonActionPerformed(evt);
            }
        });

        selectMatrixToggleButton.setText("Select Matrix");
        selectMatrixToggleButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                selectMatrixToggleButtonActionPerformed(evt);
            }
        });

        fileMenu.setText("File");

        openMatrixMenuItem.setText("Open Matrix");
        openMatrixMenuItem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                openMatrixMenuItemActionPerformed(evt);
            }
        });
        fileMenu.add(openMatrixMenuItem);

        openTreeMenuItem.setText("Open Tree");
        openTreeMenuItem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                openTreeMenuItemActionPerformed(evt);
            }
        });
        fileMenu.add(openTreeMenuItem);

        openWorkspaceMenuItem.setText("Open Workspace");
        fileMenu.add(openWorkspaceMenuItem);

        saveWorkspaceMenuItem.setText("Save Workspace");
        fileMenu.add(saveWorkspaceMenuItem);

        exitMenuItem.setText("Exit");
        exitMenuItem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                exitMenuItemActionPerformed(evt);
            }
        });
        fileMenu.add(exitMenuItem);

        jMenuBar1.add(fileMenu);

        helpMenu.setText("Help");

        aboutMenuItem.setText("About");
        helpMenu.add(aboutMenuItem);

        reportErrorMenuItem.setText("Report Error");
        reportErrorMenuItem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                reportErrorMenuItemActionPerformed(evt);
            }
        });
        helpMenu.add(reportErrorMenuItem);

        jMenuBar1.add(helpMenu);

        setJMenuBar(jMenuBar1);

        org.jdesktop.layout.GroupLayout layout = new org.jdesktop.layout.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(layout.createSequentialGroup()
                .addContainerGap()
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(layout.createSequentialGroup()
                        .add(iconLabel)
                        .add(18, 18, 18)
                        .add(titleLabel, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 391, Short.MAX_VALUE))
                    .add(layout.createSequentialGroup()
                        .add(selectTreeToggleButton)
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(selectMatrixToggleButton)))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(layout.createSequentialGroup()
                .addContainerGap()
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(titleLabel)
                    .add(iconLabel))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED, 324, Short.MAX_VALUE)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(selectTreeToggleButton)
                    .add(selectMatrixToggleButton))
                .addContainerGap())
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void selectTreeToggleButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_selectTreeToggleButtonActionPerformed
        this.handleSelectTreeAction();
    }//GEN-LAST:event_selectTreeToggleButtonActionPerformed

    private void selectMatrixToggleButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_selectMatrixToggleButtonActionPerformed
        this.handleSelectMatrixAction();
    }//GEN-LAST:event_selectMatrixToggleButtonActionPerformed

    private void exitMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_exitMenuItemActionPerformed
        this.dispose();
        //System.exit( 0 );
    }//GEN-LAST:event_exitMenuItemActionPerformed

    private void openMatrixMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_openMatrixMenuItemActionPerformed
        this.doOpenMatrix();
    }//GEN-LAST:event_openMatrixMenuItemActionPerformed

    private void openTreeMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_openTreeMenuItemActionPerformed
        this.doOpenTree();
    }//GEN-LAST:event_openTreeMenuItemActionPerformed

    private void reportErrorMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_reportErrorMenuItemActionPerformed
        this.errorDialog.setVisible( true );
    }//GEN-LAST:event_reportErrorMenuItemActionPerformed

    /**
    * @param args the command line arguments
    */
    public static void main(String args[]) {
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new Workbench().setVisible(true);
            }
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JMenuItem aboutMenuItem;
    private javax.swing.JMenuItem exitMenuItem;
    private javax.swing.JMenu fileMenu;
    private javax.swing.JMenu helpMenu;
    private javax.swing.JLabel iconLabel;
    private javax.swing.JMenuBar jMenuBar1;
    private javax.swing.JMenuItem openMatrixMenuItem;
    private javax.swing.JMenuItem openTreeMenuItem;
    private javax.swing.JMenuItem openWorkspaceMenuItem;
    private javax.swing.JMenuItem reportErrorMenuItem;
    private javax.swing.JMenuItem saveWorkspaceMenuItem;
    private javax.swing.JToggleButton selectMatrixToggleButton;
    private javax.swing.JToggleButton selectTreeToggleButton;
    private javax.swing.JLabel titleLabel;
    // End of variables declaration//GEN-END:variables

}
