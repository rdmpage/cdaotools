/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * AddAnnotation.java
 *
 * Created on Apr 28, 2010, 11:48:23 PM
 */

package cdaoexplorer.forms.dialogs;

import cdaoexplorer.model.annotation.AnnotationSourceCollection;
import cdaoexplorer.model.annotation.URLList;
import java.net.MalformedURLException;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.Iterator;
import java.util.Set;
import java.util.TreeSet;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.DefaultComboBoxModel;
import org.mindswap.pellet.owlapi.Reasoner;
import org.semanticweb.owl.apibinding.OWLManager;
import org.semanticweb.owl.model.OWLDataProperty;
import org.semanticweb.owl.model.OWLOntology;
import org.semanticweb.owl.model.OWLOntologyCreationException;
import org.semanticweb.owl.model.OWLOntologyManager;

/**
 *
 * @author bchisham
 */
public class AddAnnotation extends javax.swing.JDialog {
    private static OWLOntologyManager ontologyManager;
    private static AnnotationSourceCollection annotationSource;
    private static Set< String > shortNames = new TreeSet();
    private static DefaultComboBoxModel slistModel = new DefaultComboBoxModel();;
    private static boolean selectionMade = false;
    
    public static int  getOntologiesLoaded(){ return shortNames.size(); }

    /** Creates new form AddAnnotation */
    public AddAnnotation(java.awt.Frame parent, boolean modal) {
        super(parent, modal);
        
            initComponents();
            this.annotationComboBox.setModel(slistModel);
            //this.annotationComboBox.setSelectedIndex(0);
           // setAvailableProperties();
            AddAnnotation.selectionMade = false;
        

    }

    public static void  loadAnnotations() throws MalformedURLException{
         if (null == AddAnnotation.annotationSource ){
                AddAnnotation.ontologyManager = OWLManager.createOWLOntologyManager();
                AddAnnotation.annotationSource = new AnnotationSourceCollection();
            }

            AddAnnotation.addNewAnnotationType("cdao", new URL("http://www.evolutionaryontology.org/cdao/1.0/cdao.owl"));
            AddAnnotation.addNewAnnotationType( "study", new URL("http://www.cs.nmsu.edu/~bchisham/study.owl"));
    }

    /**
     * Get the annotation term.
     * @return
     */
    public URL getProperty(){
        return (URL)this.annotationTermComboBox.getSelectedItem();
    }
    /**
     * Get the value of the annotation.
     * @return
     */
    public String getValue(){
        return this.annotationValueTextField.getText();
    }


    public void setAvailableProperties(){
        String src = (String) annotationComboBox.getSelectedItem();
        Set< URI > pset  = AddAnnotation.annotationSource.getTerms(src);
        annotationTermComboBox.setModel( new URLList( pset ) );
        repaint();
    }


    public static void addNewAnnotationType( String shortName, URL source ){
        try {
            //OWLDataFactory dataFactory = this.ontologyManager.getOWLDataFactory();
            //PelletReasonerFactory reasonerFactory = new PelletReasonerFactory();
            //OWLReasonerFactory reasonerFactory = null;

            

            if ( ! shortNames.contains( shortName ) ){
                System.err.println("Adding: xmlns:" + shortName + "=\"" + source.toString() + "\"" );

                OWLOntology ontology = ontologyManager.loadOntologyFromPhysicalURI( source.toURI() );

                Reasoner reasoner = new Reasoner( ontologyManager );


                reasoner.setOntology( ontology );


                Set< OWLDataProperty > props = reasoner.getDataProperties();
                TreeSet< URI > uri_props = new TreeSet();
                Iterator< OWLDataProperty > pit = props.iterator();
                while (  pit.hasNext() ){
                    uri_props.add( pit.next().getURI() );
                }
                AddAnnotation.annotationSource.addSource(shortName, uri_props);
                slistModel.addElement( shortName );
                shortNames.add(shortName);


                
            }
            
            //this.setAvailableProperties();
        } catch (OWLOntologyCreationException ex) {
            Logger.getLogger(AddAnnotation.class.getName()).log(Level.SEVERE, null, ex);
        } catch (URISyntaxException ex) {
            Logger.getLogger(AddAnnotation.class.getName()).log(Level.SEVERE, null, ex);
        } 

    }

    public void addNewAnnotationType(){
        try {
            AddAnnotationSource aas = new AddAnnotationSource(null, true);
            aas.setVisible(true);
            String shortName = aas.getSymbolicName();
            URL terms = aas.getSourceURL();
            this.addNewAnnotationType(shortName, terms);
        } catch (MalformedURLException ex) {
            Logger.getLogger(AddAnnotation.class.getName()).log(Level.SEVERE, null, ex);
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
        jLabel2 = new javax.swing.JLabel();
        annotationComboBox = new javax.swing.JComboBox();
        jLabel3 = new javax.swing.JLabel();
        jLabel4 = new javax.swing.JLabel();
        annotationValueTextField = new javax.swing.JTextField();
        annotationTermComboBox = new javax.swing.JComboBox();
        saveButton = new javax.swing.JButton();
        closeButton = new javax.swing.JButton();
        newAnnotationTypeButton = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        org.jdesktop.application.ResourceMap resourceMap = org.jdesktop.application.Application.getInstance().getContext().getResourceMap(AddAnnotation.class);
        setTitle(resourceMap.getString("Form.title")); // NOI18N
        setName("Form"); // NOI18N

        jLabel1.setFont(resourceMap.getFont("jLabel1.font")); // NOI18N
        jLabel1.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        jLabel1.setText(resourceMap.getString("jLabel1.text")); // NOI18N
        jLabel1.setName("jLabel1"); // NOI18N

        jLabel2.setText(resourceMap.getString("jLabel2.text")); // NOI18N
        jLabel2.setName("jLabel2"); // NOI18N

        annotationComboBox.setName("annotationComboBox"); // NOI18N
        annotationComboBox.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                annotationComboBoxActionPerformed(evt);
            }
        });
        annotationComboBox.addFocusListener(new java.awt.event.FocusAdapter() {
            public void focusLost(java.awt.event.FocusEvent evt) {
                annotationComboBoxFocusLost(evt);
            }
        });

        jLabel3.setText(resourceMap.getString("jLabel3.text")); // NOI18N
        jLabel3.setName("jLabel3"); // NOI18N

        jLabel4.setText(resourceMap.getString("jLabel4.text")); // NOI18N
        jLabel4.setName("jLabel4"); // NOI18N

        annotationValueTextField.setText(resourceMap.getString("annotationValueTextField.text")); // NOI18N
        annotationValueTextField.setName("annotationValueTextField"); // NOI18N

        annotationTermComboBox.setName("annotationTermComboBox"); // NOI18N

        saveButton.setText(resourceMap.getString("saveButton.text")); // NOI18N
        saveButton.setName("saveButton"); // NOI18N
        saveButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                saveButtonActionPerformed(evt);
            }
        });

        closeButton.setText(resourceMap.getString("closeButton.text")); // NOI18N
        closeButton.setName("closeButton"); // NOI18N
        closeButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                closeButtonActionPerformed(evt);
            }
        });

        newAnnotationTypeButton.setText(resourceMap.getString("newAnnotationTypeButton.text")); // NOI18N
        newAnnotationTypeButton.setName("newAnnotationTypeButton"); // NOI18N
        newAnnotationTypeButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                newAnnotationTypeButtonActionPerformed(evt);
            }
        });

        org.jdesktop.layout.GroupLayout layout = new org.jdesktop.layout.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(layout.createSequentialGroup()
                .addContainerGap()
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(jLabel1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 399, Short.MAX_VALUE)
                    .add(layout.createSequentialGroup()
                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                            .add(jLabel2)
                            .add(jLabel3)
                            .add(jLabel4))
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                            .add(annotationValueTextField, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 281, Short.MAX_VALUE)
                            .add(annotationTermComboBox, 0, 281, Short.MAX_VALUE)
                            .add(org.jdesktop.layout.GroupLayout.TRAILING, annotationComboBox, 0, 281, Short.MAX_VALUE)))
                    .add(layout.createSequentialGroup()
                        .add(saveButton)
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                        .add(closeButton)
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(newAnnotationTypeButton)))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(layout.createSequentialGroup()
                .addContainerGap()
                .add(jLabel1)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(jLabel2)
                    .add(annotationComboBox, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(jLabel3)
                    .add(annotationTermComboBox, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(annotationValueTextField, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                    .add(jLabel4))
                .add(18, 18, 18)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(saveButton)
                    .add(closeButton)
                    .add(newAnnotationTypeButton))
                .addContainerGap(17, Short.MAX_VALUE))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void newAnnotationTypeButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_newAnnotationTypeButtonActionPerformed
        this.addNewAnnotationType();
    }//GEN-LAST:event_newAnnotationTypeButtonActionPerformed

    private void annotationComboBoxFocusLost(java.awt.event.FocusEvent evt) {//GEN-FIRST:event_annotationComboBoxFocusLost
        //this.setAvailableProperties();
    }//GEN-LAST:event_annotationComboBoxFocusLost

    private void annotationComboBoxActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_annotationComboBoxActionPerformed
        this.setAvailableProperties();
    }//GEN-LAST:event_annotationComboBoxActionPerformed

    private void saveButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_saveButtonActionPerformed
        this.setVisible( false );
        this.selectionMade = true;
    }//GEN-LAST:event_saveButtonActionPerformed

    private void closeButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_closeButtonActionPerformed
        this.setVisible( false );

    }//GEN-LAST:event_closeButtonActionPerformed

    /**
    * @param args the command line arguments
    */
    public static void main(String args[]) {
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                AddAnnotation dialog = new AddAnnotation(new javax.swing.JFrame(), true);
                dialog.addWindowListener(new java.awt.event.WindowAdapter() {
                    public void windowClosing(java.awt.event.WindowEvent e) {
                        System.exit(0);
                    }
                });
                dialog.setVisible(true);
            }
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JComboBox annotationComboBox;
    private javax.swing.JComboBox annotationTermComboBox;
    private javax.swing.JTextField annotationValueTextField;
    private javax.swing.JButton closeButton;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JButton newAnnotationTypeButton;
    private javax.swing.JButton saveButton;
    // End of variables declaration//GEN-END:variables

}
