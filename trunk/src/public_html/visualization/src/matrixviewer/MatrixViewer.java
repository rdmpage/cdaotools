/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * MatrixViewer.java
 *
 * Created on Apr 17, 2010, 2:07:31 PM
 */

package matrixviewer;

import cdaoexplorer.forms.dialogs.FileDialog;
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
import javax.swing.JFrame;
import matrixviewer.model.Matrix;
import matrixviewer.model.MatrixFactory;
import matrixviewer.model.Range;
import matrixviewer.view.MatrixView;
import org.jdesktop.application.Action;
import org.jdesktop.application.FrameView;
import org.jdesktop.application.SingleFrameApplication;

/**
 *
 * @author bchisham
 */
public class MatrixViewer extends JFrame {
    private Matrix matrix;
    private static String ftype = "csv";
    private Range rowRange;
    private Range colRange;
    private KeyDisplay kd;
    //private OpenURLDialog openUrlDialog;
    /** Creates new form MatrixViewer */
    public MatrixViewer() {
        //super(app);
        initComponents();
        this.scaleButton.setEnabled( false );
        this.restoreScaleButton.setEnabled( false );
        //this.setDefaultCloseOperation( JFrame.DISPOSE_ON_CLOSE );
        //this.unloadAction();
    }

    public MatrixViewer( String fileorurl){
        //super(app);
        initComponents();
        this.openFile( fileorurl );
        this.setDefaultCloseOperation( JFrame.DISPOSE_ON_CLOSE );
    }

    public MatrixViewer( Matrix matrix){
        //super(app);
        initComponents();
        this.matrix = matrix;
        this.setDefaultCloseOperation( JFrame.DISPOSE_ON_CLOSE );
        //this.setMatrix(matrix, "No Title");
    }

    public void postInit(){
        //this.extractButton.setText( "Extract" );
        //this.highlightButton.setText( "Highlight" );
    }

    public void setMatrix(Matrix matrix, String title){
        this.matrix = matrix;
        //System.err.println("Set matrix");
        if (matrix != null){
            //matrix.write( System.err );
            this.setTitle( "Matrix Data - " + title);
            this.rowRangeTextField.setText("0-" + this.matrix.getrowcount());
            this.colRangeTextField.setText("0-" + this.matrix.getcolumncount() );
            this.rowStatusTextField.setText( ""+this.matrix.getrowcount() );
            this.columnStatusTextField.setText( ""+this.matrix.getcolumncount() );
            ((MatrixView)this.matrix_view_panel).setMatrix(matrix);
            
            this.highlightAction();
            this.kd = new KeyDisplay( matrix );
            kd.setTitle( "Color Key - " + title );
            kd.setVisible( true );
        }
        else {
            this.kd.setVisible( false );
            this.kd.dispose();
            this.rowRangeTextField.setText("0-N");
            this.colRangeTextField.setText("0-N");
            this.rowStatusTextField.setText("");
            this.columnStatusTextField.setText("");
            ((MatrixView)this.matrix_view_panel).setMatrix(matrix);

        }
        this.scaleButton.setEnabled( true );
        this.restoreScaleButton.setEnabled( true );
            //this.repaint();
    }

    public void openFile(String fileorurl){
        try {
            this.openFile( new URL(fileorurl) );
        } catch ( MalformedURLException ex ){
            this.openFile( new File(fileorurl) );
        }
    }

    public void openFile(URL url){
        try {
            this.matrix = MatrixFactory.getMatrix( ftype );
            this.matrix.read(url.openStream());
            
            this.setMatrix(matrix, url.toString());
            

        } catch (IOException ex) {
            Logger.getLogger(MatrixViewerDisplay.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public void openFile(File file){
        try {
            this.matrix = MatrixFactory.getMatrix(ftype);
            this.matrix.read(new FileInputStream(file));
            this.setMatrix(matrix, file.getName());

            //this.repaint();
        } catch (FileNotFoundException ex) {
            Logger.getLogger(MatrixViewerDisplay.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

     @Action
    private void openFile(){
        FileDialog openChooser = new FileDialog(this,true);
        openChooser.setDialogType( JFileChooser.OPEN_DIALOG );
        openChooser.setModal( true );
        openChooser.setVisible( true );
        if ( openChooser.getSelectedFile() != null ){
            this.openFile( openChooser.getSelectedFile() );
        }
    }

     @Action
     private void openURL(){
         OpenURLDialog urlDialog = new OpenURLDialog();
         urlDialog.setModal( true );
         urlDialog.setVisible( true );
        try {
            this.openFile(urlDialog.getURL());
        } catch (MalformedURLException ex) {
            Logger.getLogger(MatrixViewer.class.getName()).log(Level.SEVERE, null, ex);
        }

     }

    @Action
    private void saveFile(){
        try {
            FileDialog saveChooser = new FileDialog(this,true);
            saveChooser.setDialogType( JFileChooser.SAVE_DIALOG );
            saveChooser.setModal( true );
            saveChooser.setVisible(true);
            this.matrix.write(new FileOutputStream(saveChooser.getSelectedFile()));
        } catch (FileNotFoundException ex) {
            Logger.getLogger(MatrixViewerDisplay.class.getName()).log(Level.SEVERE, null, ex);
        }

    }
     @Action
    public void extractSubmatrixAction() {
        //System.err.println("Extracting Submatrix");
        this.setRanges();

        Matrix sub_model = this.matrix.extractRange(rowRange, colRange);
        MatrixViewer mvd = new MatrixViewer(  );
        sub_model.setXOffset( colRange.getMinValue() );
        sub_model.setYOffset( rowRange.getMinValue() );
        mvd.setMatrix(sub_model, this.getTitle() + ":" + rowRange + "x" + colRange);
        mvd.setDefaultCloseOperation( JFrame.DISPOSE_ON_CLOSE );
        mvd.setVisible(true);
        this.matrix_view_panel.invalidate();
        this.highlightAction();
    }

   @Action
   public void highlightAction(){
       this.setRanges();
       ((MatrixView)this.matrix_view_panel).setHighlightedRange(rowRange, colRange);
       this.matrix_view_panel.invalidate();
       this.repaint();
   }

   @Action
   public void startSelection(){

   }

   @Action 
   public void expandSelection(){
       MatrixView mv = (MatrixView)this.matrix_view_panel;
        //mv.mouseDragged(evt);
      Range cols = mv.getSelectedCols();
      Range rows = mv.getSelectedRows();
      this.colRange = mv.getSelectedCols();
      

      this.rowRange = mv.getSelectedRows();
      

      this.colRangeTextField.setText( this.colRange.toString());
      this.rowRangeTextField.setText( this.rowRange.toString() );

      //mv.setHighlightedRange(rowRange, colRange);
      //this.highlightAction();
      
   }


   private void setRanges(){
       Pattern matcher = Pattern.compile("-");
        String[] split_row_txt = matcher.split( this.rowRangeTextField.getText() );
        String[] split_col_txt = matcher.split( this.colRangeTextField.getText() );

        assert( split_row_txt.length > 1 );
        assert( split_row_txt.length > 1 );


        this.rowRange = new Range( Integer.parseInt( split_row_txt[0].trim() ) , Integer.parseInt( split_row_txt[1].trim() ) );
        this.colRange = new Range( Integer.parseInt( split_col_txt[0].trim() ) , Integer.parseInt( split_col_txt[1].trim() ) );

   }

   @Action
   public void clearAction(){
        this.colRangeTextField.setText("0-"+this.columnStatusTextField.getText());
        this.rowRangeTextField.setText("0-"+this.rowStatusTextField.getText());
        this.highlightAction();
        //this.matrix_view_panel.invalidate();
        //this.repaint();
   }

   @Action
   public void unloadAction(){
        this.setMatrix(null, "No Matrix Loaded");
        this.scaleButton.setEnabled( false );
        this.scaleButton.setEnabled( false );
   }

   @Action
   public void scaleMatrix(){
       final int scaleFactors[] = { 8,4,2,2,4,8,16 };
       final int FIRST_DOWN_INDEX = 3;
       int selectedIndex = (Integer)this.scaleFactorComboBox.getSelectedIndex();
       MatrixView mv = (MatrixView)this.matrix_view_panel;
       if ( selectedIndex >= FIRST_DOWN_INDEX ){
           //scale down

           mv.scaleDown( scaleFactors[ selectedIndex ] );
       }
       else {
           //scale up
           mv.scaleUp( scaleFactors[ selectedIndex ] );
       }
       
       this.jScrollPane1.invalidate();
       this.jScrollPane1.repaint();

   }
   @Action
   public void restoreScale(){
       ((MatrixView)this.matrix_view_panel).resetScale();
       //this.matrix_view_panel.invalidate();
       //this.matrix_view_panel.repaint();
   }

   public void handleScrolling( java.awt.event.ComponentEvent evt ){
        MatrixView  mv = (MatrixView) this.matrix_view_panel;
        int x_displacement = this.jScrollPane1.getHorizontalScrollBar().getValue() * this.jScrollPane1.getHorizontalScrollBar().getUnitIncrement() ;
        int y_displacement = this.jScrollPane1.getVerticalScrollBar().getValue() * this.jScrollPane1.getVerticalScrollBar().getUnitIncrement();
        mv.setVisible_top( y_displacement );
        mv.setVisible_left( x_displacement );
        this.matrix_view_panel.invalidate();
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
        jLabel3 = new javax.swing.JLabel();
        subMatrixFormPanel = new javax.swing.JPanel();
        extractButton = new javax.swing.JButton();
        rowRangeTextField = new javax.swing.JTextField();
        highlightButton = new javax.swing.JButton();
        colRangeLabel = new javax.swing.JLabel();
        colRangeTextField = new javax.swing.JTextField();
        rowRangeLabel = new javax.swing.JLabel();
        clearButton = new javax.swing.JButton();
        unloadMatixButton = new javax.swing.JButton();
        jScrollPane1 = new javax.swing.JScrollPane();
        matrix_view_panel = new MatrixView();
        statusPanel = new javax.swing.JPanel();
        rowStatusLabel = new javax.swing.JLabel();
        columnStatusLabel = new javax.swing.JLabel();
        rowStatusTextField = new javax.swing.JTextField();
        columnStatusTextField = new javax.swing.JTextField();
        jPanel1 = new javax.swing.JPanel();
        scaleFactorLabel = new javax.swing.JLabel();
        scaleFactorComboBox = new javax.swing.JComboBox();
        scaleButton = new javax.swing.JButton();
        restoreScaleButton = new javax.swing.JButton();
        mainMenuBar = new javax.swing.JMenuBar();
        fileMenu = new javax.swing.JMenu();
        openMenuItem = new javax.swing.JMenuItem();
        openUrlMenuItem = new javax.swing.JMenuItem();
        saveMenuItem = new javax.swing.JMenuItem();
        closeMenuItem = new javax.swing.JMenuItem();

        openFileChooser.setName("openFileChooser"); // NOI18N

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        org.jdesktop.application.ResourceMap resourceMap = org.jdesktop.application.Application.getInstance().getContext().getResourceMap(MatrixViewer.class);
        setTitle(resourceMap.getString("Form.title")); // NOI18N
        setName("Form"); // NOI18N
        addWindowListener(new java.awt.event.WindowAdapter() {
            public void windowClosing(java.awt.event.WindowEvent evt) {
                formWindowClosing(evt);
            }
        });

        jLabel3.setFont(resourceMap.getFont("jLabel3.font")); // NOI18N
        jLabel3.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        jLabel3.setText(resourceMap.getString("jLabel3.text")); // NOI18N
        jLabel3.setName("jLabel3"); // NOI18N

        subMatrixFormPanel.setName("subMatrixFormPanel"); // NOI18N

        extractButton.setText(resourceMap.getString("extractButton.text")); // NOI18N
        extractButton.setName("extractButton"); // NOI18N
        extractButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                extractButtonActionPerformed(evt);
            }
        });

        rowRangeTextField.setText(resourceMap.getString("rowRangeTextField.text")); // NOI18N
        rowRangeTextField.setName("rowRangeTextField"); // NOI18N

        highlightButton.setText(resourceMap.getString("highlightButton.text")); // NOI18N
        highlightButton.setName("highlightButton"); // NOI18N
        highlightButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                highlightButtonActionPerformed(evt);
            }
        });

        colRangeLabel.setText(resourceMap.getString("colRangeLabel.text")); // NOI18N
        colRangeLabel.setName("colRangeLabel"); // NOI18N

        colRangeTextField.setText(resourceMap.getString("colRangeTextField.text")); // NOI18N
        colRangeTextField.setName("colRangeTextField"); // NOI18N

        rowRangeLabel.setText(resourceMap.getString("rowRangeLabel.text")); // NOI18N
        rowRangeLabel.setName("rowRangeLabel"); // NOI18N

        clearButton.setText(resourceMap.getString("clearButton.text")); // NOI18N
        clearButton.setName("clearButton"); // NOI18N
        clearButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                clearButtonActionPerformed(evt);
            }
        });

        unloadMatixButton.setText(resourceMap.getString("unloadMatixButton.text")); // NOI18N
        unloadMatixButton.setName("unloadMatixButton"); // NOI18N
        unloadMatixButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                unloadMatixButtonActionPerformed(evt);
            }
        });

        org.jdesktop.layout.GroupLayout subMatrixFormPanelLayout = new org.jdesktop.layout.GroupLayout(subMatrixFormPanel);
        subMatrixFormPanel.setLayout(subMatrixFormPanelLayout);
        subMatrixFormPanelLayout.setHorizontalGroup(
            subMatrixFormPanelLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(org.jdesktop.layout.GroupLayout.TRAILING, subMatrixFormPanelLayout.createSequentialGroup()
                .addContainerGap()
                .add(subMatrixFormPanelLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.TRAILING)
                    .add(org.jdesktop.layout.GroupLayout.LEADING, subMatrixFormPanelLayout.createSequentialGroup()
                        .add(subMatrixFormPanelLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                            .add(colRangeLabel)
                            .add(rowRangeLabel))
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(subMatrixFormPanelLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                            .add(rowRangeTextField, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 181, Short.MAX_VALUE)
                            .add(colRangeTextField, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 181, Short.MAX_VALUE)))
                    .add(org.jdesktop.layout.GroupLayout.LEADING, subMatrixFormPanelLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.TRAILING, false)
                        .add(subMatrixFormPanelLayout.createSequentialGroup()
                            .add(unloadMatixButton)
                            .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                            .add(clearButton))
                        .add(subMatrixFormPanelLayout.createSequentialGroup()
                            .add(extractButton)
                            .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                            .add(highlightButton))))
                .addContainerGap())
        );
        subMatrixFormPanelLayout.setVerticalGroup(
            subMatrixFormPanelLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(org.jdesktop.layout.GroupLayout.TRAILING, subMatrixFormPanelLayout.createSequentialGroup()
                .addContainerGap(20, Short.MAX_VALUE)
                .add(subMatrixFormPanelLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(rowRangeLabel)
                    .add(rowRangeTextField, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 30, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(subMatrixFormPanelLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(colRangeLabel)
                    .add(colRangeTextField, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                .add(subMatrixFormPanelLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(extractButton)
                    .add(highlightButton))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                .add(subMatrixFormPanelLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(clearButton)
                    .add(unloadMatixButton))
                .add(110, 110, 110))
        );

        jScrollPane1.setName("jScrollPane1"); // NOI18N
        jScrollPane1.addComponentListener(new java.awt.event.ComponentAdapter() {
            public void componentMoved(java.awt.event.ComponentEvent evt) {
                scrollComponentMoved(evt);
            }
        });

        matrix_view_panel.setName("matrix_view_panel"); // NOI18N
        matrix_view_panel.addMouseMotionListener(new java.awt.event.MouseMotionAdapter() {
            public void mouseMoved(java.awt.event.MouseEvent evt) {
                matrix_view_panelMouseMoved(evt);
            }
            public void mouseDragged(java.awt.event.MouseEvent evt) {
                matrix_view_panelMouseDragged(evt);
            }
        });
        matrix_view_panel.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                matrix_view_panelMouseClicked(evt);
            }
        });

        org.jdesktop.layout.GroupLayout matrix_view_panelLayout = new org.jdesktop.layout.GroupLayout(matrix_view_panel);
        matrix_view_panel.setLayout(matrix_view_panelLayout);
        matrix_view_panelLayout.setHorizontalGroup(
            matrix_view_panelLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(0, 678, Short.MAX_VALUE)
        );
        matrix_view_panelLayout.setVerticalGroup(
            matrix_view_panelLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(0, 474, Short.MAX_VALUE)
        );

        jScrollPane1.setViewportView(matrix_view_panel);

        statusPanel.setBorder(javax.swing.BorderFactory.createBevelBorder(javax.swing.border.BevelBorder.RAISED));
        statusPanel.setName("statusPanel"); // NOI18N

        rowStatusLabel.setText(resourceMap.getString("rowStatusLabel.text")); // NOI18N
        rowStatusLabel.setName("rowStatusLabel"); // NOI18N

        columnStatusLabel.setText(resourceMap.getString("columnStatusLabel.text")); // NOI18N
        columnStatusLabel.setName("columnStatusLabel"); // NOI18N

        rowStatusTextField.setEditable(false);
        rowStatusTextField.setText(resourceMap.getString("rowStatusTextField.text")); // NOI18N
        rowStatusTextField.setBorder(null);
        rowStatusTextField.setFocusable(false);
        rowStatusTextField.setName("rowStatusTextField"); // NOI18N
        rowStatusTextField.setRequestFocusEnabled(false);

        columnStatusTextField.setEditable(false);
        columnStatusTextField.setText(resourceMap.getString("columnStatusTextField.text")); // NOI18N
        columnStatusTextField.setBorder(null);
        columnStatusTextField.setFocusable(false);
        columnStatusTextField.setName("columnStatusTextField"); // NOI18N
        columnStatusTextField.setRequestFocusEnabled(false);

        org.jdesktop.layout.GroupLayout statusPanelLayout = new org.jdesktop.layout.GroupLayout(statusPanel);
        statusPanel.setLayout(statusPanelLayout);
        statusPanelLayout.setHorizontalGroup(
            statusPanelLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(statusPanelLayout.createSequentialGroup()
                .addContainerGap()
                .add(statusPanelLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(columnStatusLabel)
                    .add(rowStatusLabel))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(statusPanelLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(rowStatusTextField, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 144, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                    .add(columnStatusTextField, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 144, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .addContainerGap())
        );
        statusPanelLayout.setVerticalGroup(
            statusPanelLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(statusPanelLayout.createSequentialGroup()
                .add(31, 31, 31)
                .add(statusPanelLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(rowStatusLabel)
                    .add(rowStatusTextField, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(statusPanelLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(columnStatusLabel)
                    .add(columnStatusTextField, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .addContainerGap(org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        jPanel1.setName("jPanel1"); // NOI18N

        scaleFactorLabel.setText(resourceMap.getString("scaleFactorLabel.text")); // NOI18N
        scaleFactorLabel.setName("scaleFactorLabel"); // NOI18N

        scaleFactorComboBox.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "800", "400", "200", "50", "25", "13", "7" }));
        scaleFactorComboBox.setSelectedIndex(3);
        scaleFactorComboBox.setName("scaleFactorComboBox"); // NOI18N

        scaleButton.setText(resourceMap.getString("scaleButton.text")); // NOI18N
        scaleButton.setName("scaleButton"); // NOI18N
        scaleButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                scaleButtonActionPerformed(evt);
            }
        });

        restoreScaleButton.setText(resourceMap.getString("restoreScaleButton.text")); // NOI18N
        restoreScaleButton.setName("restoreScaleButton"); // NOI18N
        restoreScaleButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                restoreScaleButtonActionPerformed(evt);
            }
        });

        org.jdesktop.layout.GroupLayout jPanel1Layout = new org.jdesktop.layout.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(jPanel1Layout.createSequentialGroup()
                .addContainerGap()
                .add(jPanel1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(jPanel1Layout.createSequentialGroup()
                        .add(scaleFactorLabel)
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                        .add(scaleFactorComboBox, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                    .add(jPanel1Layout.createSequentialGroup()
                        .add(scaleButton)
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                        .add(restoreScaleButton)))
                .addContainerGap(109, Short.MAX_VALUE))
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(jPanel1Layout.createSequentialGroup()
                .add(23, 23, 23)
                .add(jPanel1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(scaleFactorLabel)
                    .add(scaleFactorComboBox, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(jPanel1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(scaleButton)
                    .add(restoreScaleButton))
                .addContainerGap(115, Short.MAX_VALUE))
        );

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

        openUrlMenuItem.setText(resourceMap.getString("openUrlMenuItem.text")); // NOI18N
        openUrlMenuItem.setName("openUrlMenuItem"); // NOI18N
        openUrlMenuItem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                openUrlMenuItemActionPerformed(evt);
            }
        });
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
        closeMenuItem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                closeMenuItemActionPerformed(evt);
            }
        });
        fileMenu.add(closeMenuItem);

        mainMenuBar.add(fileMenu);

        setJMenuBar(mainMenuBar);

        org.jdesktop.layout.GroupLayout layout = new org.jdesktop.layout.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(layout.createSequentialGroup()
                .addContainerGap()
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(jLabel3, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 946, Short.MAX_VALUE)
                    .add(layout.createSequentialGroup()
                        .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                            .add(jPanel1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                            .add(org.jdesktop.layout.GroupLayout.TRAILING, statusPanel, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 299, Short.MAX_VALUE)
                            .add(org.jdesktop.layout.GroupLayout.TRAILING, subMatrixFormPanel, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                        .add(18, 18, 18)
                        .add(jScrollPane1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 629, Short.MAX_VALUE)))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(layout.createSequentialGroup()
                .addContainerGap()
                .add(jLabel3)
                .add(18, 18, 18)
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(layout.createSequentialGroup()
                        .add(subMatrixFormPanel, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 162, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(jPanel1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED, 28, Short.MAX_VALUE)
                        .add(statusPanel, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                    .add(jScrollPane1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 482, Short.MAX_VALUE))
                .addContainerGap())
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void openMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_openMenuItemActionPerformed
        this.openFile();
    }//GEN-LAST:event_openMenuItemActionPerformed

    private void openUrlMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_openUrlMenuItemActionPerformed
        this.openURL();
    }//GEN-LAST:event_openUrlMenuItemActionPerformed

    private void saveMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_saveMenuItemActionPerformed
        this.saveFile();
    }//GEN-LAST:event_saveMenuItemActionPerformed

    private void closeMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_closeMenuItemActionPerformed
        this.setVisible( false );
        this.dispose();
    }//GEN-LAST:event_closeMenuItemActionPerformed

    private void extractButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_extractButtonActionPerformed
        this.extractSubmatrixAction();
    }//GEN-LAST:event_extractButtonActionPerformed

    private void highlightButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_highlightButtonActionPerformed
        this.setRanges();
        this.highlightAction();
    }//GEN-LAST:event_highlightButtonActionPerformed

    private void formWindowClosing(java.awt.event.WindowEvent evt) {//GEN-FIRST:event_formWindowClosing
        if (this.kd != null){
          this.kd.setVisible(false);
        }
    }//GEN-LAST:event_formWindowClosing

    private void clearButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_clearButtonActionPerformed
        this.clearAction();
    }//GEN-LAST:event_clearButtonActionPerformed

    private void unloadMatixButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_unloadMatixButtonActionPerformed
        this.unloadAction();
    }//GEN-LAST:event_unloadMatixButtonActionPerformed

    private void matrix_view_panelMouseMoved(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_matrix_view_panelMouseMoved
        
    }//GEN-LAST:event_matrix_view_panelMouseMoved

    private void matrix_view_panelMouseDragged(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_matrix_view_panelMouseDragged
        this.expandSelection();
    }//GEN-LAST:event_matrix_view_panelMouseDragged

    private void matrix_view_panelMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_matrix_view_panelMouseClicked
        MatrixView mv = (MatrixView)this.matrix_view_panel;
        if (mv.isSelectionActive()){
            mv.resetSelection();
            this.clearAction();
        } else {
        
            this.expandSelection();
        }
        
        //this.expandSelection();
    }//GEN-LAST:event_matrix_view_panelMouseClicked

    private void scaleButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_scaleButtonActionPerformed
        this.scaleMatrix();
    }//GEN-LAST:event_scaleButtonActionPerformed

    private void restoreScaleButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_restoreScaleButtonActionPerformed
        this.restoreScale();
    }//GEN-LAST:event_restoreScaleButtonActionPerformed

    private void scrollComponentMoved(java.awt.event.ComponentEvent evt) {//GEN-FIRST:event_scrollComponentMoved
        this.handleScrolling(evt);
    }//GEN-LAST:event_scrollComponentMoved


    /**
    * @param args the command line arguments
    */
    public static void main(String args[]) {
        if (args.length <= 1){
            java.awt.EventQueue.invokeLater(new Runnable() {
                public void run() {
                    new MatrixViewer().setVisible(true);
                }
            });
        }
        else {
            final String urlarg = args[1];
            java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new MatrixViewer( urlarg ).setVisible(true);
            }
        });
        }
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton clearButton;
    private javax.swing.JMenuItem closeMenuItem;
    private javax.swing.JLabel colRangeLabel;
    private javax.swing.JTextField colRangeTextField;
    private javax.swing.JLabel columnStatusLabel;
    private javax.swing.JTextField columnStatusTextField;
    private javax.swing.JButton extractButton;
    private javax.swing.JMenu fileMenu;
    private javax.swing.JButton highlightButton;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JMenuBar mainMenuBar;
    private javax.swing.JPanel matrix_view_panel;
    private javax.swing.JFileChooser openFileChooser;
    private javax.swing.JMenuItem openMenuItem;
    private javax.swing.JMenuItem openUrlMenuItem;
    private javax.swing.JButton restoreScaleButton;
    private javax.swing.JLabel rowRangeLabel;
    private javax.swing.JTextField rowRangeTextField;
    private javax.swing.JLabel rowStatusLabel;
    private javax.swing.JTextField rowStatusTextField;
    private javax.swing.JMenuItem saveMenuItem;
    private javax.swing.JButton scaleButton;
    private javax.swing.JComboBox scaleFactorComboBox;
    private javax.swing.JLabel scaleFactorLabel;
    private javax.swing.JPanel statusPanel;
    private javax.swing.JPanel subMatrixFormPanel;
    private javax.swing.JButton unloadMatixButton;
    // End of variables declaration//GEN-END:variables

}
