/*
 * MatrixViewerView.java
 */

package matrixviewer;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.jdesktop.application.Action;
import org.jdesktop.application.ResourceMap;
import org.jdesktop.application.SingleFrameApplication;
import org.jdesktop.application.FrameView;
import org.jdesktop.application.TaskMonitor;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.regex.Pattern;
import javax.swing.Timer;
import javax.swing.Icon;
import javax.swing.JDialog;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import matrixviewer.model.Matrix;
import matrixviewer.model.MatrixFactory;
import matrixviewer.view.MatrixView;

/**
 * The application's main frame.
 */
public class MatrixViewerView extends FrameView {
    
    public MatrixViewerView(SingleFrameApplication app) {
        super(app);
        this.fileDialog = new JFileChooser();
        this.fileTypePattern = Pattern.compile("\\.([a-zA-Z]+)$");
        initComponents();
        // status bar initialization - message timeout, idle icon and busy animation, etc
        ResourceMap resourceMap = getResourceMap();
        int messageTimeout = resourceMap.getInteger("StatusBar.messageTimeout");
        messageTimer = new Timer(messageTimeout, new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                statusMessageLabel.setText("");
            }
        });
        messageTimer.setRepeats(false);
        int busyAnimationRate = resourceMap.getInteger("StatusBar.busyAnimationRate");
        for (int i = 0; i < busyIcons.length; i++) {
            busyIcons[i] = resourceMap.getIcon("StatusBar.busyIcons[" + i + "]");
        }
        busyIconTimer = new Timer(busyAnimationRate, new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                busyIconIndex = (busyIconIndex + 1) % busyIcons.length;
                statusAnimationLabel.setIcon(busyIcons[busyIconIndex]);
            }
        });
        idleIcon = resourceMap.getIcon("StatusBar.idleIcon");
        statusAnimationLabel.setIcon(idleIcon);
        //progressBar.setVisible(false);

        // connecting action tasks to status bar via TaskMonitor
        TaskMonitor taskMonitor = new TaskMonitor(getApplication().getContext());
        taskMonitor.addPropertyChangeListener(new java.beans.PropertyChangeListener() {
            public void propertyChange(java.beans.PropertyChangeEvent evt) {
                String propertyName = evt.getPropertyName();
                if ("started".equals(propertyName)) {
                    if (!busyIconTimer.isRunning()) {
                        statusAnimationLabel.setIcon(busyIcons[0]);
                        busyIconIndex = 0;
                        busyIconTimer.start();
                    }
                    //progressBar.setVisible(true);
                    //progressBar.setIndeterminate(true);
                } else if ("done".equals(propertyName)) {
                    busyIconTimer.stop();
                    statusAnimationLabel.setIcon(idleIcon);
                    //progressBar.setVisible(false);
                    //progressBar.setValue(0);
                } else if ("message".equals(propertyName)) {
                    String text = (String)(evt.getNewValue());
                    statusMessageLabel.setText((text == null) ? "" : text);
                    messageTimer.restart();
                } else if ("progress".equals(propertyName)) {
                    int value = (Integer)(evt.getNewValue());
                    //progressBar.setVisible(true);
                    //progressBar.setIndeterminate(false);
                    //progressBar.setValue(value);
                }
            }
        });
        MatrixViewerApp.getApplication().getMainFrame().setTitle( "Matrix Viewer" );
    }

    @Action
    public void showAboutBox() {
        if (aboutBox == null) {
            JFrame mainFrame = MatrixViewerApp.getApplication().getMainFrame();
            aboutBox = new MatrixViewerAboutBox(mainFrame);
            aboutBox.setLocationRelativeTo(mainFrame);
        }
        MatrixViewerApp.getApplication().show(aboutBox);
    }

    public void setMatrix(Matrix matrix){
        this.matrix = matrix;
    }

    public Matrix getMatrix(){
        return this.matrix;
    }
    @Action
    public void openFile(){
        System.err.println("Open file handler.");
        this.fileDialog.showOpenDialog( MatrixViewerApp.getApplication().getMainFrame() );
        File userSelection = this.fileDialog.getSelectedFile();
        System.err.println("Selected File: " + userSelection.getAbsolutePath());
        try {
            InputStream is = new FileInputStream(userSelection);
            assert( is != null );
            String ftype = "csv";
            if ( this.fileTypePattern.matcher( userSelection.getName() ).matches() ){
                ftype = this.fileTypePattern.matcher(  userSelection.getName() ).group( );
            }
            this.matrix = MatrixFactory.getMatrix( ftype );
            this.matrix.read(is);
            ((MatrixView)this.matrix_canvas).setMatrix( this.matrix );
            this.matrixStatsLabel.setText( "File: " + userSelection.getName() + 
                                           " Rows: " + this.matrix.getrowcount() +
                                           " Cols: " + this.matrix.getrowcount() );
            //this.matrix_canvas.repaint();

            is.close();
            this.matrix.write( System.err );
        } catch (FileNotFoundException ex) {
            Logger.getLogger(MatrixViewerView.class.getName()).log(Level.SEVERE, null, ex);
        } catch ( IOException ex ){
            Logger.getLogger(MatrixViewerView.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    @Action
    public void saveFile(){
        OutputStream os = null;
        try {
            this.fileDialog.showSaveDialog( MatrixViewerApp.getApplication().getMainFrame() );
            File userSelection = this.fileDialog.getSelectedFile();
            os = new FileOutputStream(userSelection);
            this.matrix.write(os);
        } catch (FileNotFoundException ex) {
            Logger.getLogger(MatrixViewerView.class.getName()).log(Level.SEVERE, null, ex);
        } finally {
            try {
                os.close();
            } catch (IOException ex) {
                Logger.getLogger(MatrixViewerView.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }

    @Action
    public void handleResize(){
        this.matrix_canvas.setSize( this.getFrame().getWidth()  , this.getFrame().getHeight()  );
        this.matrix_canvas.repaint();

    }

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {
        bindingGroup = new org.jdesktop.beansbinding.BindingGroup();

        mainPanel = new javax.swing.JPanel();
        jScrollPane1 = new javax.swing.JScrollPane();
        jPanel1 = new javax.swing.JPanel();
        matrix_canvas = new MatrixView( matrix );
        menuBar = new javax.swing.JMenuBar();
        javax.swing.JMenu fileMenu = new javax.swing.JMenu();
        openFileMenuItem = new javax.swing.JMenuItem();
        saveFileMenuItem = new javax.swing.JMenuItem();
        javax.swing.JMenuItem exitMenuItem = new javax.swing.JMenuItem();
        javax.swing.JMenu helpMenu = new javax.swing.JMenu();
        javax.swing.JMenuItem aboutMenuItem = new javax.swing.JMenuItem();
        statusPanel = new javax.swing.JPanel();
        javax.swing.JSeparator statusPanelSeparator = new javax.swing.JSeparator();
        statusMessageLabel = new javax.swing.JLabel();
        statusAnimationLabel = new javax.swing.JLabel();
        matrixStatsLabel = new javax.swing.JLabel();

        mainPanel.setName("mainPanel"); // NOI18N

        org.jdesktop.beansbinding.Binding binding = org.jdesktop.beansbinding.Bindings.createAutoBinding(org.jdesktop.beansbinding.AutoBinding.UpdateStrategy.READ_WRITE, matrix_canvas, org.jdesktop.beansbinding.ELProperty.create("${visible}"), mainPanel, org.jdesktop.beansbinding.BeanProperty.create("autoscrolls"));
        bindingGroup.addBinding(binding);

        jScrollPane1.setName("jScrollPane1"); // NOI18N

        jPanel1.setName("jPanel1"); // NOI18N
        jPanel1.setPreferredSize(new java.awt.Dimension(0, 0));

        matrix_canvas.setName("matrix_canvas"); // NOI18N

        org.jdesktop.layout.GroupLayout jPanel1Layout = new org.jdesktop.layout.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(matrix_canvas, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 636, Short.MAX_VALUE)
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(matrix_canvas, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 257, Short.MAX_VALUE)
        );

        matrix_canvas.getAccessibleContext().setAccessibleParent(jScrollPane1);

        jScrollPane1.setViewportView(jPanel1);

        org.jdesktop.layout.GroupLayout mainPanelLayout = new org.jdesktop.layout.GroupLayout(mainPanel);
        mainPanel.setLayout(mainPanelLayout);
        mainPanelLayout.setHorizontalGroup(
            mainPanelLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(org.jdesktop.layout.GroupLayout.TRAILING, jScrollPane1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 640, Short.MAX_VALUE)
        );
        mainPanelLayout.setVerticalGroup(
            mainPanelLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(jScrollPane1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 261, Short.MAX_VALUE)
        );

        menuBar.setName("menuBar"); // NOI18N

        org.jdesktop.application.ResourceMap resourceMap = org.jdesktop.application.Application.getInstance(matrixviewer.MatrixViewerApp.class).getContext().getResourceMap(MatrixViewerView.class);
        fileMenu.setText(resourceMap.getString("fileMenu.text")); // NOI18N
        fileMenu.setName("fileMenu"); // NOI18N

        javax.swing.ActionMap actionMap = org.jdesktop.application.Application.getInstance(matrixviewer.MatrixViewerApp.class).getContext().getActionMap(MatrixViewerView.class, this);
        openFileMenuItem.setAction(actionMap.get("openFile")); // NOI18N
        openFileMenuItem.setText(resourceMap.getString("openMenuItem.text")); // NOI18N
        openFileMenuItem.setToolTipText(resourceMap.getString("openMenuItem.toolTipText")); // NOI18N
        openFileMenuItem.setName("openMenuItem"); // NOI18N
        openFileMenuItem.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                openFileMouseClicked(evt);
            }
        });
        fileMenu.add(openFileMenuItem);

        saveFileMenuItem.setAction(actionMap.get("saveFile")); // NOI18N
        saveFileMenuItem.setText(resourceMap.getString("saveMenuItem.text")); // NOI18N
        saveFileMenuItem.setName("saveMenuItem"); // NOI18N
        saveFileMenuItem.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                saveFileMouseClicked(evt);
            }
        });
        fileMenu.add(saveFileMenuItem);

        exitMenuItem.setAction(actionMap.get("quit")); // NOI18N
        exitMenuItem.setName("exitMenuItem"); // NOI18N
        fileMenu.add(exitMenuItem);

        menuBar.add(fileMenu);

        helpMenu.setText(resourceMap.getString("helpMenu.text")); // NOI18N
        helpMenu.setName("helpMenu"); // NOI18N

        aboutMenuItem.setAction(actionMap.get("showAboutBox")); // NOI18N
        aboutMenuItem.setName("aboutMenuItem"); // NOI18N
        helpMenu.add(aboutMenuItem);

        menuBar.add(helpMenu);

        statusPanel.setName("statusPanel"); // NOI18N

        statusPanelSeparator.setName("statusPanelSeparator"); // NOI18N

        statusMessageLabel.setName("statusMessageLabel"); // NOI18N

        statusAnimationLabel.setHorizontalAlignment(javax.swing.SwingConstants.LEFT);
        statusAnimationLabel.setName("statusAnimationLabel"); // NOI18N

        matrixStatsLabel.setText(resourceMap.getString("matrixDataLabel.text")); // NOI18N
        matrixStatsLabel.setName("matrixDataLabel"); // NOI18N

        org.jdesktop.layout.GroupLayout statusPanelLayout = new org.jdesktop.layout.GroupLayout(statusPanel);
        statusPanel.setLayout(statusPanelLayout);
        statusPanelLayout.setHorizontalGroup(
            statusPanelLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(statusPanelLayout.createSequentialGroup()
                .addContainerGap()
                .add(matrixStatsLabel)
                .add(18, 18, 18)
                .add(statusPanelLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(statusPanelSeparator, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 491, Short.MAX_VALUE)
                    .add(statusPanelLayout.createSequentialGroup()
                        .add(statusMessageLabel)
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED, 471, Short.MAX_VALUE)
                        .add(statusAnimationLabel)
                        .addContainerGap())))
        );
        statusPanelLayout.setVerticalGroup(
            statusPanelLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(statusPanelLayout.createSequentialGroup()
                .add(statusPanelSeparator, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 2, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .add(statusPanelLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(statusMessageLabel)
                    .add(statusAnimationLabel)
                    .add(matrixStatsLabel))
                .add(23, 23, 23))
        );

        matrixStatsLabel.getAccessibleContext().setAccessibleName(resourceMap.getString("matrixDataLabel.AccessibleContext.accessibleName")); // NOI18N

        setComponent(mainPanel);
        setMenuBar(menuBar);
        setStatusBar(statusPanel);

        bindingGroup.bind();
    }// </editor-fold>//GEN-END:initComponents

    private void saveFileMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_saveFileMouseClicked
        this.saveFile();
    }//GEN-LAST:event_saveFileMouseClicked

    private void openFileMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_openFileMouseClicked
        this.openFile();
    }//GEN-LAST:event_openFileMouseClicked

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JPanel jPanel1;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JPanel mainPanel;
    private javax.swing.JLabel matrixStatsLabel;
    private java.awt.Canvas matrix_canvas;
    private javax.swing.JMenuBar menuBar;
    private javax.swing.JMenuItem openFileMenuItem;
    private javax.swing.JMenuItem saveFileMenuItem;
    private javax.swing.JLabel statusAnimationLabel;
    private javax.swing.JLabel statusMessageLabel;
    private javax.swing.JPanel statusPanel;
    private org.jdesktop.beansbinding.BindingGroup bindingGroup;
    // End of variables declaration//GEN-END:variables

    private final Timer messageTimer;
    private final Timer busyIconTimer;
    private final Icon idleIcon;
    private final Icon[] busyIcons = new Icon[15];
    private int busyIconIndex = 0;

    private JDialog aboutBox;

    private Matrix matrix;
    private JFileChooser fileDialog;
    Pattern fileTypePattern;
}
