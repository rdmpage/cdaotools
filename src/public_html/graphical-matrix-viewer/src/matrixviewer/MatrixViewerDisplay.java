
/*
 * MatrixViewerDisplay.java
 *
 * Created on Apr 16, 2010, 12:59:30 PM
 */

package matrixviewer;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.regex.Pattern;
import javax.swing.JFileChooser;
import matrixviewer.model.Matrix;
import matrixviewer.model.MatrixFactory;
import matrixviewer.model.Range;
import matrixviewer.view.MatrixView;
import org.jdesktop.application.Action;

/**
 *
 * @author bchisham
 */
public class MatrixViewerDisplay extends javax.swing.JFrame {
    private static String ftype= "csv";
    Matrix matrix;

    private void doPostInit(){
        this.extractRangeButton.setText("Extract");
        this.rowRangeLabel.setText( "Row Ranage:" );
        this.colRangeLabel.setText( "Col Range: " );
        this.highlightRangeBtn.setText( "Highlight" );
        this.fileMenu.setText( "File" );
        this.saveMenuItem.setText( "Save" );
        this.Open.setText( "Open" );
        this.openUrlMenuItem.setText( "Open URL" );
        this.closeMenuItem.setText( "Close" );
    }

    /** Creates new form MatrixViewerDisplay */
    public MatrixViewerDisplay() {
        initComponents();
        doPostInit();
        

    }

    public MatrixViewerDisplay(String urlOrFile){
        initComponents();
        doPostInit();
        try {
            URL url = new URL(urlOrFile);
            openFile(url);
        } catch (MalformedURLException ex) {
            File file = new File( urlOrFile );
            openFile(file);
        }


    }

    public MatrixViewerDisplay( File file ){
        this.initComponents();
        this.doPostInit();
        this.setTitle( "Matrix Viewer - " +file.getName() );
        this.openFile(file);
    }

    private MatrixViewerDisplay(String title, Matrix sub_model) {
        initComponents();
        this.doPostInit();
        
        this.colRangeTxtField.setText( "0-" + sub_model.getcolumncount() );
        this.rowRangeTxtField.setText( "0-" + sub_model.getrowcount() );
        this.setTitle(title);
        this.matrix = sub_model;
        ((MatrixView)this.matrix_display_panel).setMatrix(sub_model);
         KeyDisplay kd = new KeyDisplay( matrix );
         kd.setTitle( "Color Key = " + this.getTitle() );
         kd.setVisible( true );
    }

    @Action
    private void openFile(){
        this.openFileChooser.setDialogType( JFileChooser.OPEN_DIALOG );
        this.openFileChooser.setVisible( true );
        if ( this.openFileChooser.getSelectedFile() != null ){
            this.openFile( this.openFileChooser.getSelectedFile() );
        }
    }

    @Action
    private void saveFile(){
        try {
            this.openFileChooser.setDialogType(JFileChooser.SAVE_DIALOG);
            this.openFileChooser.setVisible(true);
            this.matrix.write(new FileOutputStream(this.openFileChooser.getSelectedFile()));
        } catch (FileNotFoundException ex) {
            Logger.getLogger(MatrixViewerDisplay.class.getName()).log(Level.SEVERE, null, ex);
        }
        
    }

    private void openFile(URL url){
        try {
            this.matrix = MatrixFactory.getMatrix( ftype );
            this.matrix.read(url.openStream());
            this.setTitle( "Matrix Data - " + url.toString());
            this.rowRangeTxtField.setText("0-" + this.matrix.getrowcount());
            this.colRangeTxtField.setText("0-" + this.matrix.getcolumncount() );
            ((MatrixView)this.matrix_display_panel).setMatrix(matrix);
            //((RowLabelCanvas)this.row_labels_canvas).setModel(matrix);
            KeyDisplay kd = new KeyDisplay( matrix );
            kd.setVisible( true );
            kd.setTitle( "Color Key - " + url.toString() );
            //this.repaint();

        } catch (IOException ex) {
            Logger.getLogger(MatrixViewerDisplay.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    private void openFile(File file){
        try {
            this.matrix = MatrixFactory.getMatrix(ftype);
            this.matrix.read(new FileInputStream(file));
            this.setTitle( "Matrix Data - " + file.getName() );
            this.rowRangeTxtField.setText("0," + this.matrix.getrowcount());
            this.colRangeTxtField.setText("0," + this.matrix.getcolumncount() );
            ((MatrixView)this.matrix_display_panel).setMatrix(matrix);
            //((RowLabelCanvas)this.row_labels_canvas).setModel(matrix);
            KeyDisplay kd = new KeyDisplay( matrix );
            kd.setVisible( true );
            kd.setTitle( "Color Key - " + file.getName() );
            //this.repaint();
        } catch (FileNotFoundException ex) {
            Logger.getLogger(MatrixViewerDisplay.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    private void matrixResized(){
        //this.matrixScrollPane.setSize( this.matrix_display_panel.getWidth() ,  this.matrix_display_panel.getHeight() );
       // this.matrixScrollPane.setPreferredSize( this.matrix_display_panel.getPreferredSize() );
        this.matrixScrollPane.invalidate();
        //this.invalidate();
        //this.repaint();
    }


    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        openFileChooser = new javax.swing.JFileChooser();
        titleLabel = new javax.swing.JLabel();
        rowRangeLabel = new javax.swing.JLabel();
        colRangeLabel = new javax.swing.JLabel();
        rowRangeTxtField = new javax.swing.JTextField();
        colRangeTxtField = new javax.swing.JTextField();
        extractRangeButton = new javax.swing.JButton();
        highlightRangeBtn = new javax.swing.JButton();
        matrixScrollPane = new javax.swing.JScrollPane();
        matrix_display_panel = new MatrixView( );
        jScrollPane1 = new javax.swing.JScrollPane();
        mainMenu = new javax.swing.JMenuBar();
        fileMenu = new javax.swing.JMenu();
        Open = new javax.swing.JMenuItem();
        openUrlMenuItem = new javax.swing.JMenuItem();
        saveMenuItem = new javax.swing.JMenuItem();
        closeMenuItem = new javax.swing.JMenuItem();

        openFileChooser.setName("openFileChooser"); // NOI18N

        setName("Form"); // NOI18N

        org.jdesktop.application.ResourceMap resourceMap = org.jdesktop.application.Application.getInstance(matrixviewer.MatrixViewerApp.class).getContext().getResourceMap(MatrixViewerDisplay.class);
        titleLabel.setFont(resourceMap.getFont("titleLabel.font")); // NOI18N
        titleLabel.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        titleLabel.setText(resourceMap.getString("titleLabel.text")); // NOI18N
        titleLabel.setName("titleLabel"); // NOI18N

        rowRangeLabel.setLabelFor(rowRangeTxtField);
        rowRangeLabel.setText(resourceMap.getString("rowRangeLabel.text")); // NOI18N
        rowRangeLabel.setFocusable(false);
        rowRangeLabel.setName("rowRangeLabel"); // NOI18N

        colRangeLabel.setLabelFor(colRangeTxtField);
        colRangeLabel.setText(resourceMap.getString("colRangeLabel.text")); // NOI18N
        colRangeLabel.setFocusable(false);
        colRangeLabel.setName("colRangeLabel"); // NOI18N

        rowRangeTxtField.setText(resourceMap.getString("rowRangeTxtField.text")); // NOI18N
        rowRangeTxtField.setName("rowRangeTxtField"); // NOI18N

        colRangeTxtField.setText(resourceMap.getString("colRangeTxtField.text")); // NOI18N
        colRangeTxtField.setName("colRangeTxtField"); // NOI18N

        extractRangeButton.setText(resourceMap.getString("extractRangeButton.text")); // NOI18N
        extractRangeButton.setName("extractRangeButton"); // NOI18N
        extractRangeButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                extractRangeButtonActionPerformed(evt);
            }
        });

        highlightRangeBtn.setText(resourceMap.getString("highlightRangeBtn.text")); // NOI18N
        highlightRangeBtn.setName("highlightRangeBtn"); // NOI18N

        matrixScrollPane.setAutoscrolls(true);
        matrixScrollPane.setName("matrixScrollPane"); // NOI18N
        matrixScrollPane.setNextFocusableComponent(rowRangeTxtField);
        matrixScrollPane.addComponentListener(new java.awt.event.ComponentAdapter() {
            public void componentResized(java.awt.event.ComponentEvent evt) {
                matrixScrollPaneComponentResized(evt);
            }
        });

        matrix_display_panel.setName("matrix_display_panel"); // NOI18N
        matrix_display_panel.addComponentListener(new java.awt.event.ComponentAdapter() {
            public void componentResized(java.awt.event.ComponentEvent evt) {
                matrix_display_panelComponentResized(evt);
            }
        });

        org.jdesktop.layout.GroupLayout matrix_display_panelLayout = new org.jdesktop.layout.GroupLayout(matrix_display_panel);
        matrix_display_panel.setLayout(matrix_display_panelLayout);
        matrix_display_panelLayout.setHorizontalGroup(
            matrix_display_panelLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(0, 740, Short.MAX_VALUE)
        );
        matrix_display_panelLayout.setVerticalGroup(
            matrix_display_panelLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(0, 455, Short.MAX_VALUE)
        );

        matrixScrollPane.setViewportView(matrix_display_panel);

        jScrollPane1.setName("jScrollPane1"); // NOI18N

        mainMenu.setName("mainMenu"); // NOI18N

        fileMenu.setText(resourceMap.getString("fileMenu.text")); // NOI18N
        fileMenu.setName("fileMenu"); // NOI18N

        Open.setText(resourceMap.getString("Open.text")); // NOI18N
        Open.setName("Open"); // NOI18N
        Open.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                OpenActionPerformed(evt);
            }
        });
        fileMenu.add(Open);

        openUrlMenuItem.setText(resourceMap.getString("openUrlMenuItem.text")); // NOI18N
        openUrlMenuItem.setName("openUrlMenuItem"); // NOI18N
        fileMenu.add(openUrlMenuItem);

        saveMenuItem.setText(resourceMap.getString("saveMenuItem.text")); // NOI18N
        saveMenuItem.setName("saveMenuItem"); // NOI18N
        saveMenuItem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                saveMenuItemActionPerformed(evt);
            }
        });
        fileMenu.add(saveMenuItem);

        closeMenuItem.setText(resourceMap.getString("closeMenuItem.text")); // NOI18N
        closeMenuItem.setName("closeMenuItem"); // NOI18N
        fileMenu.add(closeMenuItem);

        mainMenu.add(fileMenu);

        setJMenuBar(mainMenu);

        org.jdesktop.layout.GroupLayout layout = new org.jdesktop.layout.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(layout.createSequentialGroup()
                .addContainerGap()
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(titleLabel, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 945, Short.MAX_VALUE)
                    .add(layout.createSequentialGroup()
                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                            .add(rowRangeLabel)
                            .add(extractRangeButton, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 80, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                            .add(colRangeLabel))
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING, false)
                            .add(highlightRangeBtn, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 94, Short.MAX_VALUE)
                            .add(colRangeTxtField)
                            .add(rowRangeTxtField))
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(matrixScrollPane, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 752, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(layout.createSequentialGroup()
                .add(20, 20, 20)
                .add(titleLabel)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(layout.createSequentialGroup()
                        .add(32, 32, 32)
                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                            .add(rowRangeLabel)
                            .add(rowRangeTxtField, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                            .add(colRangeLabel)
                            .add(colRangeTxtField, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                            .add(extractRangeButton)
                            .add(highlightRangeBtn)))
                    .add(layout.createSequentialGroup()
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(matrixScrollPane, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 447, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap())
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void extractRangeButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_extractRangeButtonActionPerformed
        this.extractSubmatrixAction();
    }//GEN-LAST:event_extractRangeButtonActionPerformed

    private void matrixScrollPaneComponentResized(java.awt.event.ComponentEvent evt) {//GEN-FIRST:event_matrixScrollPaneComponentResized
        //this.matrixResized();
    }//GEN-LAST:event_matrixScrollPaneComponentResized

    private void matrix_display_panelComponentResized(java.awt.event.ComponentEvent evt) {//GEN-FIRST:event_matrix_display_panelComponentResized
       this.matrixResized();
    }//GEN-LAST:event_matrix_display_panelComponentResized

    private void OpenActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_OpenActionPerformed
        this.openFile();
    }//GEN-LAST:event_OpenActionPerformed

    private void saveMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_saveMenuItemActionPerformed
        this.saveFile();
    }//GEN-LAST:event_saveMenuItemActionPerformed

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

    @Action
    public void extractSubmatrixAction() {
        System.err.println("Extracting Submatrix");
        Pattern matcher = Pattern.compile("-");
        String[] split_row_txt = matcher.split( this.rowRangeTxtField.getText() );
        String[] split_col_txt = matcher.split( this.colRangeTxtField.getText() );

        assert( split_row_txt.length > 1 );
        assert( split_row_txt.length > 1 );


        Range row_range = new Range( Integer.parseInt( split_row_txt[0].trim() ) , Integer.parseInt( split_row_txt[1].trim() ) );
        Range col_range = new Range( Integer.parseInt( split_col_txt[0].trim() ) , Integer.parseInt( split_col_txt[1].trim() ) );

        Matrix sub_model = this.matrix.extractRange(row_range, col_range);
        MatrixViewerDisplay mvd = new MatrixViewerDisplay( this.getTitle() + ":" + row_range + "x" + col_range , sub_model );
        mvd.setVisible(true);
    }




    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JMenuItem Open;
    private javax.swing.JMenuItem closeMenuItem;
    private javax.swing.JLabel colRangeLabel;
    private javax.swing.JTextField colRangeTxtField;
    private javax.swing.JButton extractRangeButton;
    private javax.swing.JMenu fileMenu;
    private javax.swing.JButton highlightRangeBtn;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JMenuBar mainMenu;
    private javax.swing.JScrollPane matrixScrollPane;
    private javax.swing.JPanel matrix_display_panel;
    private javax.swing.JFileChooser openFileChooser;
    private javax.swing.JMenuItem openUrlMenuItem;
    private javax.swing.JLabel rowRangeLabel;
    private javax.swing.JTextField rowRangeTxtField;
    private javax.swing.JMenuItem saveMenuItem;
    private javax.swing.JLabel titleLabel;
    // End of variables declaration//GEN-END:variables

}
