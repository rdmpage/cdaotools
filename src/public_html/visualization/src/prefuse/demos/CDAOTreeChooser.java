/*
 * CDAOTreeChooser.java
 *
 * Created on Apr 12, 2010, 6:09:26 PM
 */

package prefuse.demos;

import cdaoexplorer.forms.dialogs.CDAODataSetChooser;
import cdaoexplorer.forms.dialogs.ErrorReportDialog;
import cdaoexplorer.forms.dialogs.FileDialog;
import cdaoexplorer.guimodels.TreeNameListModel;
import java.io.File;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JFrame;

/**
 *
 * @author bchisham
 */
public class CDAOTreeChooser extends CDAODataSetChooser {
    private TreeNameListModel model;
    private AboutBox aboutBox;
    private FileDialog fileDialog;
    private ErrorReportDialog errorDialog;
    /** Creates new form CDAOTreeChooser */
    public CDAOTreeChooser() {
        //super();
        model = new TreeNameListModel();
        this.setModel( model );
        model.setListingType( TreeNameListModel.ListingType.TREE );
        model.updateModel();

        

        aboutBox = new AboutBox();
        fileDialog = new FileDialog(this, true);
        errorDialog = new ErrorReportDialog( this, false );
        initComponents();
        this.setTitle("CDAO Tree Chooser");
        this.setKeyLabel("Tree ID:");
        
    }

    public void view(){
        String[] params = new String[ 2 ];
        String treeURI = "http://www.cs.nmsu.edu/~cdaostore/cgi-bin/phylows/tree/" +
                this.getSelectedItem() + "?format=graphml" ;

        System.err.println("Displaying URI: " + treeURI );

        params[ 0 ] = treeURI;
        params[ 1 ] = "IdLabel";
        //params[ 2 ] = treeURI;


        JFrame viewer = CDAOview.demo(treeURI, "IdLabel");
        viewer.setDefaultCloseOperation( JFrame.DISPOSE_ON_CLOSE);
        //CDAOview.
        //CDAOview.main(params);

    }

    
    public void doOpenFile(){

            this.fileDialog.setVisible(true);
            System.err.println("Displaying file: " + this.fileDialog.getSelectedFile().getAbsolutePath());
            File treeFile = this.fileDialog.getSelectedFile();
            
            if ( treeFile != null ){
                try {
                    this.doOpenFile(treeFile.toURL());
                } catch (MalformedURLException ex) {
                    Logger.getLogger(CDAOTreeChooser.class.getName()).log(Level.SEVERE, null, ex);
                }
            }

    }

    @Override
    public void doOpenFile(URL file) {
        JFrame viewer = CDAOview.demo(file, "IdLabel");
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

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

        org.jdesktop.layout.GroupLayout layout = new org.jdesktop.layout.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(0, 512, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(0, 511, Short.MAX_VALUE)
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

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
    // End of variables declaration//GEN-END:variables

    

}
