
import javax.swing.*;
import javax.swing.event.*;
import java.awt.event.*;

import java.io.*;

class JvSaveAsAction extends AbstractAction{
	private JvUndoableTableModel tableModel;
	private JFileChooser fc;
	private File outputfile;
	public JvSaveAsAction(JvUndoableTableModel tableModel,JFileChooser fc){
		super();
		this.tableModel=tableModel;
		this.fc=fc;
		outputfile=null;
	}
	public void actionPerformed(ActionEvent e) {
        if (fc.showSaveDialog(null) == JFileChooser.APPROVE_OPTION)
        {
            File f = fc.getSelectedFile();

            // check if file exists before overwriting
            // (Note: Dialog only pops up if file exists)
            if (!f.exists() || okToReplace(f))
            	outputfile=f;
        }
		try {
			MatrixVisualization.CSVWriter(tableModel, outputfile);
		} catch (IOException e1) {
			System.out.println("Writing error");
			e1.printStackTrace();
			System.exit(1);
		}
	}
	   private boolean okToReplace(File f)
	   {
	      final Object[] options = { "Yes", "No", "Cancel" };
	      return JOptionPane.showOptionDialog(null,
	                  "The file '" + f.getName() +
	                  "' already exists.  " +
	                  "Replace existing file?",
	                  "Warning",
	                  JOptionPane.YES_NO_CANCEL_OPTION,
	                  JOptionPane.WARNING_MESSAGE,
	                  null,
	                  options,
	                  options[2]) == JOptionPane.YES_OPTION;
	   }
}

