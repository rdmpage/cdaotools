/*
 * CDAOTreeChooser.java
 *
 * Created on Apr 12, 2010, 6:09:26 PM
 */

package prefuse.demos;

import java.net.MalformedURLException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JFrame;

/**
 *
 * @author bchisham
 */
public class CDAOTreeChooser extends javax.swing.JFrame {
    private TreeNameListModel model;
    AboutBox aboutBox;
    FileDialog fileDialog;
    /** Creates new form CDAOTreeChooser */
    public CDAOTreeChooser() {
        model = new TreeNameListModel();
        model.updateModel();
        aboutBox = new AboutBox();
        fileDialog = new FileDialog(this, true);
        initComponents();
        
    }
    private void nextPage(){
        this.model.nextPage( this.treeIDTxt.getText() );
        this.repaint();
    }
    private void prevPage(){
        this.model.prevPage( this.treeIDTxt.getText() );
        this.repaint();
    }

    private void viewTree(){
        String[] params = new String[ 2 ];
        String treeURI = "http://www.cs.nmsu.edu/~bchisham/cgi-bin/phylows/tree/" +
                this.model.getElementAt( this.treeList.getMinSelectionIndex() ) + "?format=graphml" ;

        System.err.println("Displaying URI: " + treeURI );

        params[ 0 ] = treeURI;
        params[ 1 ] = "IdLabel";
        //params[ 2 ] = treeURI;


        JFrame viewer = CDAOview.demo(treeURI, "IdLabel");
        viewer.setDefaultCloseOperation( JFrame.DISPOSE_ON_CLOSE);
        //CDAOview.
        //CDAOview.main(params);

    }

    private void matchKey(){
        this.model.matchKey( this.treeIDTxt.getText() );
        this.treeList.setModel(model);
        this.repaint();
    }

    public void doOpenFile(){
        
            this.fileDialog.setVisible(true);
            System.err.println("Displaying file: " + this.fileDialog.getSelectedFile().getAbsolutePath());
            JFrame viewer = CDAOview.demo(this.fileDialog.getSelectedFile(), "IdLabel");
            viewer.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        
    }

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jScrollPane1 = new javax.swing.JScrollPane();
        treeList = new javax.swing.JList();
        prevButton = new javax.swing.JButton();
        viewBtn = new javax.swing.JButton();
        nextBtn = new javax.swing.JButton();
        treeIDTxt = new javax.swing.JTextField();
        jLabel1 = new javax.swing.JLabel();
        jMenuBar1 = new javax.swing.JMenuBar();
        mainMenu = new javax.swing.JMenu();
        openFileMenuItem = new javax.swing.JMenuItem();
        Exit = new javax.swing.JMenuItem();
        jMenu2 = new javax.swing.JMenu();
        helpMenu = new javax.swing.JMenu();
        aboutMenuItem = new javax.swing.JMenuItem();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

        treeList.setModel(this.model);
        treeList.setSelectionMode(javax.swing.ListSelectionModel.SINGLE_SELECTION);
        treeList.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                treeSelected(evt);
            }
        });
        jScrollPane1.setViewportView(treeList);

        prevButton.setLabel("< Prev");
        prevButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                prevButtonActionPerformed(evt);
            }
        });

        viewBtn.setText("View");
        viewBtn.setEnabled(false);
        viewBtn.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                viewBtnActionPerformed(evt);
            }
        });

        nextBtn.setLabel("Next >");
        nextBtn.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                nextBtnActionPerformed(evt);
            }
        });

        treeIDTxt.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                treeIDTxtActionPerformed(evt);
            }
        });

        jLabel1.setText("Tree ID:");

        mainMenu.setText("File");

        openFileMenuItem.setText("Open File");
        openFileMenuItem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                openFileMenuItemActionPerformed(evt);
            }
        });
        mainMenu.add(openFileMenuItem);

        Exit.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_X, java.awt.event.InputEvent.CTRL_MASK));
        Exit.setText("Exit");
        Exit.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                exitApplication(evt);
            }
        });
        mainMenu.add(Exit);

        jMenuBar1.add(mainMenu);

        jMenu2.setText("Edit");
        jMenuBar1.add(jMenu2);

        helpMenu.setText("Help");

        aboutMenuItem.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_A, java.awt.event.InputEvent.CTRL_MASK));
        aboutMenuItem.setText("About");
        aboutMenuItem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                showAbout(evt);
            }
        });
        helpMenu.add(aboutMenuItem);

        jMenuBar1.add(helpMenu);

        setJMenuBar(jMenuBar1);

        org.jdesktop.layout.GroupLayout layout = new org.jdesktop.layout.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(org.jdesktop.layout.GroupLayout.TRAILING, layout.createSequentialGroup()
                .addContainerGap()
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.TRAILING)
                    .add(jScrollPane1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 468, Short.MAX_VALUE)
                    .add(org.jdesktop.layout.GroupLayout.LEADING, layout.createSequentialGroup()
                        .add(prevButton)
                        .add(120, 120, 120)
                        .add(viewBtn)
                        .add(101, 101, 101)
                        .add(nextBtn))
                    .add(org.jdesktop.layout.GroupLayout.LEADING, layout.createSequentialGroup()
                        .add(jLabel1)
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(treeIDTxt, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 409, Short.MAX_VALUE)))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(org.jdesktop.layout.GroupLayout.TRAILING, layout.createSequentialGroup()
                .add(28, 28, 28)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(jLabel1)
                    .add(treeIDTxt, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(jScrollPane1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 372, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED, 35, Short.MAX_VALUE)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(nextBtn)
                    .add(prevButton)
                    .add(viewBtn)))
        );

        prevButton.getAccessibleContext().setAccessibleName("Prev");
        nextBtn.getAccessibleContext().setAccessibleName("Next");

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void nextBtnActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_nextBtnActionPerformed
    this.nextPage();

    }//GEN-LAST:event_nextBtnActionPerformed

    private void prevButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_prevButtonActionPerformed
        this.prevPage();
    }//GEN-LAST:event_prevButtonActionPerformed

    private void viewBtnActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_viewBtnActionPerformed
        this.viewTree();
    }//GEN-LAST:event_viewBtnActionPerformed

    private void treeIDTxtActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_treeIDTxtActionPerformed
        this.matchKey();
    }//GEN-LAST:event_treeIDTxtActionPerformed

    private void treeSelected(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_treeSelected
        this.viewBtn.setEnabled( true );
    }//GEN-LAST:event_treeSelected

    private void exitApplication(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_exitApplication
        System.exit( 0 );
    }//GEN-LAST:event_exitApplication

    private void showAbout(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_showAbout
        this.aboutBox.setVisible( true );
    }//GEN-LAST:event_showAbout

    private void openFileMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_openFileMenuItemActionPerformed
        this.doOpenFile();
    }//GEN-LAST:event_openFileMenuItemActionPerformed

    /**
    * @param args the command line arguments
    */
    public static void main(String args[]) {
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new CDAOTreeChooser().setVisible(true);
            }
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JMenuItem Exit;
    private javax.swing.JMenuItem aboutMenuItem;
    private javax.swing.JMenu helpMenu;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JMenu jMenu2;
    private javax.swing.JMenuBar jMenuBar1;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JMenu mainMenu;
    private javax.swing.JButton nextBtn;
    private javax.swing.JMenuItem openFileMenuItem;
    private javax.swing.JButton prevButton;
    private javax.swing.JTextField treeIDTxt;
    private javax.swing.JList treeList;
    private javax.swing.JButton viewBtn;
    // End of variables declaration//GEN-END:variables

}
