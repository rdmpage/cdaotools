/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * AnnotationListPanel.java
 *
 * Created on May 10, 2010, 7:47:34 PM
 */

package cdaoexplorer.forms.panels;

import cdaoexplorer.forms.dialogs.AddAnnotation;
import cdaoexplorer.forms.dialogs.OpenURLDialog;
import cdaoexplorer.model.annotation.DataProperty;
import cdaoexplorer.model.annotation.PropertyListModel;
import cdaoexplorer.model.annotation.Triple;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.DefaultComboBoxModel;
import org.jdesktop.application.Action;

/**
 *
 * @author bchisham
 */
public class AnnotationListPanel extends javax.swing.JPanel {
    private AddAnnotation addAnnotation;
    private PropertyListModel model;
    private DefaultComboBoxModel subjects;
    private OpenURLDialog urlDialog;
    //private URL subject;
    /** Creates new form AnnotationListPanel */
    public AnnotationListPanel() {
        initComponents();
        this.addAnnotation = new AddAnnotation( null, true);
        this.model = new PropertyListModel();
        this.subjects = new DefaultComboBoxModel();
        this.subjectComboBox.setModel(subjects);
        this.urlDialog = new OpenURLDialog( null, true );
        try {
            this.subjects.addElement( new URL("http://www.cs.nmsu.edu/~cdaostore") );
        } catch (MalformedURLException ex) {
            Logger.getLogger(AnnotationListPanel.class.getName()).log(Level.SEVERE, null, ex);
        }

    }

    public void addSubject( URL subject ){
        //this.subject = subject;
        this.subjects.addElement(subject);
    }

    public URL getSubject(){
        return (URL) this.subjectComboBox.getSelectedItem();
    }

    @Action
    public void handleAddAnnotation(){
        this.addAnnotation.setVisible(true);
        URL prop = this.addAnnotation.getProperty();
        String value = this.addAnnotation.getValue();
        this.model.addAnnotation( new DataProperty( (URL)this.subjectComboBox.getSelectedItem(), prop, value ) );

        this.propertyList.setModel( this.model.getProperties() );
        this.valueList.setModel( this.model.getValues() );
    }




    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jScrollPane2 = new javax.swing.JScrollPane();
        propertyList = new javax.swing.JList();
        propertyLabel = new javax.swing.JLabel();
        valueLabel = new javax.swing.JLabel();
        annotationsLabel = new javax.swing.JLabel();
        addButton = new javax.swing.JButton();
        editButton = new javax.swing.JButton();
        removeButton = new javax.swing.JButton();
        jScrollPane3 = new javax.swing.JScrollPane();
        valueList = new javax.swing.JList();
        jLabel1 = new javax.swing.JLabel();
        subjectComboBox = new javax.swing.JComboBox();
        addSubjectButton = new javax.swing.JButton();

        jScrollPane2.setName("jScrollPane2"); // NOI18N

        propertyList.setName("propertyList"); // NOI18N
        jScrollPane2.setViewportView(propertyList);

        propertyLabel.setText("Property");
        propertyLabel.setName("propertyLabel"); // NOI18N

        valueLabel.setText("Value");
        valueLabel.setName("valueLabel"); // NOI18N

        annotationsLabel.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        annotationsLabel.setText("Annotations");
        annotationsLabel.setName("annotationsLabel"); // NOI18N

        addButton.setText("Add Annotation");
        addButton.setName("addButton"); // NOI18N
        addButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                addButtonActionPerformed(evt);
            }
        });

        editButton.setText("Edit");
        editButton.setName("editButton"); // NOI18N

        removeButton.setText("Remove");
        removeButton.setName("removeButton"); // NOI18N

        jScrollPane3.setName("jScrollPane3"); // NOI18N

        valueList.setSelectionMode(javax.swing.ListSelectionModel.SINGLE_SELECTION);
        valueList.setName("valueList"); // NOI18N
        jScrollPane3.setViewportView(valueList);

        jLabel1.setText("Subject");
        jLabel1.setName("jLabel1"); // NOI18N

        subjectComboBox.setName("subjectComboBox"); // NOI18N

        addSubjectButton.setText("Add Subject");
        addSubjectButton.setName("addSubjectButton"); // NOI18N
        addSubjectButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                addSubjectButtonActionPerformed(evt);
            }
        });

        org.jdesktop.layout.GroupLayout layout = new org.jdesktop.layout.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(layout.createSequentialGroup()
                .addContainerGap()
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.TRAILING)
                    .add(annotationsLabel, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 561, Short.MAX_VALUE)
                    .add(org.jdesktop.layout.GroupLayout.LEADING, layout.createSequentialGroup()
                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.TRAILING, false)
                            .add(org.jdesktop.layout.GroupLayout.LEADING, layout.createSequentialGroup()
                                .add(jLabel1)
                                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                                .add(subjectComboBox, 0, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                            .add(org.jdesktop.layout.GroupLayout.LEADING, propertyLabel)
                            .add(org.jdesktop.layout.GroupLayout.LEADING, jScrollPane2, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 353, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                            .add(org.jdesktop.layout.GroupLayout.LEADING, layout.createSequentialGroup()
                                .add(13, 13, 13)
                                .add(addSubjectButton)
                                .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                                .add(addButton)
                                .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                                .add(editButton)))
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                            .add(valueLabel)
                            .add(jScrollPane3, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 198, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                            .add(removeButton))))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(org.jdesktop.layout.GroupLayout.TRAILING, layout.createSequentialGroup()
                .add(27, 27, 27)
                .add(annotationsLabel)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(jLabel1)
                    .add(subjectComboBox, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(propertyLabel)
                    .add(valueLabel))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(jScrollPane2, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 158, Short.MAX_VALUE)
                    .add(jScrollPane3, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 158, Short.MAX_VALUE))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(addSubjectButton)
                    .add(addButton)
                    .add(editButton)
                    .add(removeButton))
                .addContainerGap())
        );
    }// </editor-fold>//GEN-END:initComponents

    private void addButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_addButtonActionPerformed
        this.handleAddAnnotation();
    }//GEN-LAST:event_addButtonActionPerformed

    private void addSubjectButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_addSubjectButtonActionPerformed
        this.urlDialog.setVisible(true);
        try {
            this.addSubject(this.urlDialog.getURL());
        } catch (MalformedURLException ex) {
            Logger.getLogger(AnnotationListPanel.class.getName()).log(Level.SEVERE, null, ex);
        }

    }//GEN-LAST:event_addSubjectButtonActionPerformed


    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton addButton;
    private javax.swing.JButton addSubjectButton;
    private javax.swing.JLabel annotationsLabel;
    private javax.swing.JButton editButton;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JScrollPane jScrollPane3;
    private javax.swing.JLabel propertyLabel;
    private javax.swing.JList propertyList;
    private javax.swing.JButton removeButton;
    private javax.swing.JComboBox subjectComboBox;
    private javax.swing.JLabel valueLabel;
    private javax.swing.JList valueList;
    // End of variables declaration//GEN-END:variables

}