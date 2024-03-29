/*
 * Copyright (c) 1995 - 2008 Sun Microsystems, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   - Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   - Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *   - Neither the name of Sun Microsystems nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */ 

package matrix.view;



/*
 * TableSelectionDemo.java requires no other files.
 */

import java.net.URISyntaxException;
import javax.swing.*;
import javax.swing.table.TableColumn;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.Dimension;
import java.awt.Toolkit;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.net.URI;
import java.net.URL;
import java.util.*;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class MatrixVisualization extends JPanel 
                                implements ActionListener { 
	public static String filelocation = "http://www.cs.nmsu.edu/~bchisham/cgi-bin/phylows/matrix/M450?format=csv";
	private static String cutoffString;//The String removed in CSV reader
	private static JFrame frame;
    private JvTable table;
    private JvUndoableTableModel tableModel;
    private JScrollPane scrollPane;
    private JPanel buttonPanel;
    private JButton hide;
    private JButton extract;
    private JCheckBox rowCheck;
    private JCheckBox columnCheck;
    private JLabel label;
    private ButtonGroup buttonGroup;
    private JTextArea output;

    private void initComponents(){
        table = new JvTable(tableModel);
        //System.out.println(table.getColumnCount());
        table.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
        Dimension dimension = Toolkit.getDefaultToolkit().getScreenSize();
        table.setPreferredScrollableViewportSize(new Dimension(dimension.height, dimension.width-100));
        table.calcColumnWidths();
        //table.setFillsViewportHeight(true);
        table.getTableHeader().setReorderingAllowed(false);//disable reodering column
        table.getSelectionModel().addListSelectionListener(new RowListener());
        table.getColumnModel().getSelectionModel().
            addListSelectionListener(new ColumnListener());
        scrollPane=new JScrollPane(table);
        scrollPane.setAlignmentX(LEFT_ALIGNMENT);
        add(scrollPane);
        label=new JLabel("Selection Options");
        label.setAlignmentX(LEFT_ALIGNMENT);
        add(label);
        rowCheck = addCheckBox("Row Selection");
        rowCheck.setSelected(true);
        columnCheck = addCheckBox("Column Selection");
        buttonPanel=new JPanel();
        buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.X_AXIS));
        buttonPanel.setAlignmentX(LEFT_ALIGNMENT);
        hide=addButton("Hide");
        extract=addButton("Extract");
        add(buttonPanel);

    }
    /**
     * Open the specified matrix file initially.
     * @param path
     */
    public MatrixVisualization(String path){
        super();
        setLayout( new BoxLayout(this, BoxLayout.Y_AXIS));
        try {
        	tableModel=CSVReader(path);
	} catch (IOException e) {
			e.printStackTrace();
            System.err.println("Error loading table. Exiting...");
            System.exit(1);
	}


        initComponents();

    }
    /**
     * Display the matrix at the specified url.
     * @param url
     */
    public MatrixVisualization(URL url){
        super();
        setLayout( new BoxLayout(this, BoxLayout.Y_AXIS));
        try {
        	tableModel=CSVReader(url.openStream());
	} catch (IOException e) {
			e.printStackTrace();
            System.err.println("Error loading table. Exiting...");
            System.exit(1);
	}


        initComponents();
    }
    /**
     * Display the default matrix.
     */
    public MatrixVisualization(){
        super();
        setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
        //Read in file

        try {
        	tableModel=CSVReader(filelocation);
	} catch (IOException e) {
			e.printStackTrace();
            System.err.println("Error loading table. Exiting...");
            System.exit(1);
	}
	initComponents();
    }
    
    /**
     * Display the matrix with selected row or column
     */
    public MatrixVisualization(JvUndoableTableModel tm){
        super();
        setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
        tableModel=tm;
        initComponents();
    }
    /**
     * Read a matrix from the specified input stream
     * @param is
     * @return
     * @throws IOException
     */
    private static JvUndoableTableModel CSVReader( InputStream is )throws IOException {
        JvUndoableTableModel tableModel=null;
    	Vector< Vector <String> > data = new Vector();
    	//File file = new File(filepath);
    	boolean flag=false;
    	Scanner bufRdr  =  new Scanner( is );
    	String line = null;
    	//read each line of text file
        int colCount = 0;
    	while(bufRdr.hasNextLine()){
                line = bufRdr.nextLine();
    		Vector<String> vline= new Vector<String>();
    		StringTokenizer st = new StringTokenizer(line,",");
    		String s;
    		int index;
                Pattern separator = Pattern.compile(",");
                String[] cells = separator.split(line);
                if ( colCount <= 1){ colCount = cells.length; }
                //System.err.println("Column Count: " + colCount );
                for (int i = 0; i < cells.length; ++i){
                    //System.err.println( "Adding cell: " + cells[ i ] );
                    vline.add( cells[ i ] );
                }
                
//    		while (st.hasMoreTokens())	{
//    				//get next token and store it in the array
//    					s=st.nextToken();
//    					index=s.lastIndexOf('#');
//    					if(!flag){
//						cutoffString=s.substring(0,index+1);
//						flag=true;
//					}
//    					s=s.substring(index+1);
//    					vline.add(s);
//    				}
    		data.add(vline);
        }

        //System.err.println("Column Count: " + colCount);

    	//close the file
    	bufRdr.close();
    	//Create a dummy columns names
        Vector<String> columnNames=new Vector<String>();
        columnNames.add("");
        for (long i=1; i < colCount;i++){
        	columnNames.add("trait"+i);
        }
        tableModel=new JvUndoableTableModel(data, columnNames);
    	return tableModel;
    }
    /**
     * Read a matrix from a file
     * @param file
     * @return
     * @throws IOException
     */
    private static JvUndoableTableModel CSVReader( File file ) throws IOException {
        return CSVReader( new FileInputStream(file) );
    }
    /**
     * Read a matrix from a file or url.
     * @param filepath
     * @return
     * @throws IOException
     */
    private static JvUndoableTableModel CSVReader(String filepath) throws IOException{
        try {
            URI url = new URI(filepath);
            return CSVReader( url.toURL().openStream() );
        } catch (URISyntaxException ex) {
            File file = new File(filepath);
            return CSVReader(file);
        }

    }
    /**
     * Write the displayed matrix to the specified stream.
     * @param tableModel
     * @param file
     * @throws IOException
     */
    static void CSVWriter(JvUndoableTableModel tableModel,File file) throws IOException{
    	Vector data= tableModel.getDataVector();
    	BufferedWriter bufWrt = new BufferedWriter(new FileWriter(file));
    	for(Iterator i=data.iterator();i.hasNext();){
    		Vector<String> line= (Vector<String>)i.next();
    		for(Iterator j=line.iterator();j.hasNext();){
    				  bufWrt.write(cutoffString+j.next()+",");
    		}
    		bufWrt.write("\n");
    	}    		
    	bufWrt.close();
    }
    private JButton addButton(String text){
        JButton button =new JButton(text);
        button.addActionListener(this);
        button.setActionCommand(text);
        buttonPanel.add(button);
        return button;
    }
    private JCheckBox addCheckBox(String text) {
        JCheckBox checkBox = new JCheckBox(text);
        checkBox.addActionListener(this);
        checkBox.setAlignmentX(LEFT_ALIGNMENT);
        add(checkBox);
        return checkBox;
    }
    private JRadioButton addRadio(String text) {
        JRadioButton b = new JRadioButton(text);
        b.addActionListener(this);
        buttonGroup.add(b);
        add(b);
        return b;
    }

    public void actionPerformed(ActionEvent event) {
        String command = event.getActionCommand();
        if( command.equals( "Row Selection" ) ||command.equals( "Column Selection" ) ){
	        if ( command.equals( "Row Selection" ) ) {
	            table.setRowSelectionAllowed(rowCheck.isSelected());
	                table.setColumnSelectionAllowed(!rowCheck.isSelected());
	        } else if ( command.equals( "Column Selection" )  ) {
	            table.setColumnSelectionAllowed(columnCheck.isSelected());
	                table.setRowSelectionAllowed(!columnCheck.isSelected());
	        }
	        table.setSelectionMode(
	                ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
	        rowCheck.setSelected(table.getRowSelectionAllowed());
	        columnCheck.setSelected(table.getColumnSelectionAllowed());
        }
        else{
        	if( command.equals( "Hide" ) ){
            	int [] indices;
            	if(rowCheck.isSelected()){
            		indices=table.getSelectedRows();
            		for(int i=0;i<indices.length;i++) tableModel.removeRow(table.getSelectedRow());
            	}
            	else if(columnCheck.isSelected()){
            		indices=table.getSelectedColumns();
            		for(int i=0;i<indices.length;i++){
            			removeColumnAndData(indices[i]-i);
            		}
            		table.calcColumnWidths();
            	}
        	}
        	else if( command.equals( "Extract" ) ){
            	Vector newdata=new Vector();
            	Vector olddata= tableModel.getDataVector();
            	int [] indices;
            	if(rowCheck.isSelected()){
            		indices=table.getSelectedRows();
            		for(int i=0;i<indices.length;i++){
            			newdata.add(olddata.elementAt(indices[i]));
            		}
            	}
            	else if(columnCheck.isSelected()){
            		indices=table.getSelectedColumns();
            		for(int j=0;j<olddata.size();j++){
            			Vector<String> vline= new Vector<String>();
            			vline.add(((Vector<String>)olddata.get(j)).get(0));//
                		for(int i=0;i<indices.length;i++){
                			vline.add(((Vector<String>)olddata.get(j)).get(indices[i]));
                		}
                		newdata.add(vline);
            		}
            	}
                Vector<String> columnNames=new Vector<String>();
                columnNames.add("");
                for (long i=1;i<((Vector)newdata.firstElement()).size();i++)
                	columnNames.add(""+i);
                JvUndoableTableModel tm=new JvUndoableTableModel(newdata, columnNames);
                createAndShowGUI(tm);
        	}
        }
    }
    public void removeColumnAndData(int vColIndex) {
        TableColumn col = table.getColumnModel().getColumn(vColIndex);
        int columnModelIndex = col.getModelIndex();
        Vector data = tableModel.getDataVector();
        Vector colIds = tableModel.getColumnIdentifiers();

        // Remove the column from the table
        table.removeColumn(col);

        // Remove the column header from the table model
        colIds.removeElementAt(columnModelIndex);

        // Remove the column data
        for (int r=0; r<data.size(); r++) {
            Vector row = (Vector)data.get(r);
            row.removeElementAt(columnModelIndex);
        }
        tableModel.setDataVector(data, colIds);

        // Correct the model indices in the TableColumn objects
        // by decrementing those indices that follow the deleted column
        Enumeration<TableColumn> en = table.getColumnModel().getColumns();
        for (; en.hasMoreElements(); ) {
            TableColumn c = (TableColumn)en.nextElement();
            if (c.getModelIndex() >= columnModelIndex) {
                c.setModelIndex(c.getModelIndex()-1);
            }
        }
        tableModel.fireTableStructureChanged();
    }

    private void outputSelection() {
        output.append(String.format("Lead: %d, %d. ",
                    table.getSelectionModel().getLeadSelectionIndex(),
                    table.getColumnModel().getSelectionModel().
                        getLeadSelectionIndex()));
        output.append("Rows:");
        for (int c : table.getSelectedRows()) {
            output.append(String.format(" %d", c));
        }
        output.append(". Columns:");
        for (int c : table.getSelectedColumns()) {
            output.append(String.format(" %d", c));
        }
        output.append(".\n");
    }

    private class RowListener implements ListSelectionListener {
        public void valueChanged(ListSelectionEvent event) {
            if (event.getValueIsAdjusting()) {
                return;
            }
            //output.append("ROW SELECTION EVENT. ");
            //outputSelection();
        }
    }

    private class ColumnListener implements ListSelectionListener {
        public void valueChanged(ListSelectionEvent event) {
            if (event.getValueIsAdjusting()) {
                return;
            }
            //output.append("COLUMN SELECTION EVENT. ");
            //outputSelection();
        }
    }


    class Menu extends JMenuBar{
    	 Menu(JvTable table, JvUndoableTableModel tableModel){
            //Add zoom option
            JvUndoManager undoManager = new JvUndoManager();
            tableModel.addUndoableEditListener(undoManager);
            JFileChooser fc = new JFileChooser(new File("."));
            final String[] EXTENSIONS = {".csv"}; 
            fc.addChoosableFileFilter(new MyFileFilter(EXTENSIONS));
            JMenu fileMenu = new JMenu("File");
            JMenuItem save = new JMenuItem("Save");
            save.addActionListener(new JvSaveAction(tableModel,fc));
            JMenuItem saveAs = new JMenuItem("Save As");
            saveAs.addActionListener(new JvSaveAsAction(tableModel,fc));
            JMenuItem exit= new JMenuItem("Exit");
            exit.addActionListener(new JvExitAction());
            fileMenu.add(save);
            fileMenu.add(saveAs);
            fileMenu.addSeparator();
            fileMenu.add(exit);
            JMenu editMenu = new JMenu("Edit");
            editMenu.add(undoManager.getUndoAction());
            editMenu.add(undoManager.getRedoAction());
            

            JMenu viewMenu = new JMenu("View");
            viewMenu.addSeparator();
            viewMenu.add(new JvZoomAction(table, undoManager, 200));
            viewMenu.add(new JvZoomAction(table, undoManager, 100));
            viewMenu.add(new JvZoomAction(table, undoManager, 75));
            viewMenu.add(new JvZoomAction(table, undoManager, 50));
            viewMenu.add(new JvZoomAction(table, undoManager, 25));
            viewMenu.add(new JvZoomAction(table, undoManager, 20));
            
            add(fileMenu);
            add(editMenu);
            add(viewMenu);
            
    	}
    }
    /**
     * Create the GUI and show it.  For thread safety,
     * this method should be invoked from the
     * event-dispatching thread.
     */
    private static void createAndShowGUI() {
        //Disable boldface controls.
        UIManager.put("swing.boldMetal", Boolean.FALSE); 

        //Create and set up the window.
        frame = new JFrame("Matrix");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        //Create and set up the content pane.
        MatrixVisualization newContentPane = new MatrixVisualization();
        newContentPane.setOpaque(true); //content panes must be opaque
        //Create menu 
        Menu menuBar = newContentPane.new Menu(newContentPane.table,newContentPane.tableModel);
        frame.setContentPane(newContentPane);
        frame.setJMenuBar(menuBar);
        frame.setSize(1200, 700);
        //Display the window.
        frame.pack();
        frame.setVisible(true);
    }

    private static void createAndShowGUI(String path){
        //Disable boldface controls.
        UIManager.put("swing.boldMetal", Boolean.FALSE);

        //Create and set up the window.
        frame = new JFrame("Matrix");
        frame.setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);
        //Create and set up the content pane.
        MatrixVisualization newContentPane = new MatrixVisualization( path );
        newContentPane.setOpaque(true); //content panes must be opaque
        //Create menu
        Menu menuBar = newContentPane.new Menu(newContentPane.table,newContentPane.tableModel);
        frame.setContentPane(newContentPane);
        frame.setJMenuBar(menuBar);
        frame.setSize(1200, 700);
        //Display the window.
        frame.pack();
        frame.setVisible(true);
    }
    private static void createAndShowGUI(JvUndoableTableModel tm) {
        //Disable boldface controls.
        UIManager.put("swing.boldMetal", Boolean.FALSE); 

        //Create and set up the window.
        frame = new JFrame("Matrix");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        //Create and set up the content pane.
        MatrixVisualization newContentPane = new MatrixVisualization(tm);
        newContentPane.setOpaque(true); //content panes must be opaque
        //Create menu 
        Menu menuBar = newContentPane.new
        Menu(newContentPane.table,newContentPane.tableModel);
        frame.setContentPane(newContentPane);
        frame.setJMenuBar(menuBar);
        frame.setSize(1200, 700);
        //Display the window.
        frame.pack();
        frame.setVisible(true);
    }


    public static void main(String[] args) {
        //Schedule a job for the event-dispatching thread:
        //creating and showing this application's GUI.

        if (args.length < 1){

           javax.swing.SwingUtilities.invokeLater(new Runnable() {
               public void run() {
                   createAndShowGUI();
               }
            });

        }
        else {
            final String path = args[1];
            javax.swing.SwingUtilities.invokeLater(new Runnable() {
               public void run() {
                   createAndShowGUI( path );
               }
            });
        }
    }
}
