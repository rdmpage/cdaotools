// Author: Neil W. Weber  neilweber@yahoo.com

import java.awt.*;
import java.awt.event.*;
import java.io.File;
import java.io.IOException;
import java.util.*;

import javax.swing.*;
import javax.swing.event.*;
import javax.swing.filechooser.FileFilter;
import javax.swing.table.*;
import javax.swing.undo.*;


public class ZoomTable
{
    public static void main(String[] args)
    {
        Object data[][] = {
            {"AMZN", "Amazon", 41.28, "BUY"},
            {"EBAY", "eBay", 41.57, "BUY"},
            {"GOOG", "Google", 388.33, "SELL"},
            {"MSFT", "Microsoft", 26.56, "SELL"},
            {"NOK", "Nokia Corp", 17.13, "BUY"},
            {"ORCL", "Oracle Corp.", 12.52, "BUY"},
            {"SUNW", "Sun Microsystems", 3.86, "BUY"},
            {"TWX",  "Time Warner", 17.66, "SELL"},
            {"VOD",  "Vodafone Group", 26.02, "SELL"},
            {"YHOO", "Yahoo!", 37.69, "BUY"}
        };
        String columns[] = {"Symbol", "Name", "Price", "Guidance"};

        JvUndoableTableModel tableModel = new JvUndoableTableModel(data, columns);
        JvTable table = new JvTable(tableModel);
        table.setCellSelectionEnabled(true); // required for Fill Down
        JScrollPane pane = new JScrollPane(table);

        JvUndoManager undoManager = new JvUndoManager();
        tableModel.addUndoableEditListener(undoManager);

        JMenu editMenu = new JMenu("Edit");
        editMenu.add(undoManager.getUndoAction());
        editMenu.add(undoManager.getRedoAction());
        editMenu.addSeparator();
        editMenu.add(new JvFillDownAction(table));

        JMenu viewMenu = new JMenu("View");
        viewMenu.add(new JvFontAction(table, undoManager, "Arial"));
        viewMenu.add(new JvFontAction(table, undoManager, "Century Gothic"));
        viewMenu.add(new JvFontAction(table, undoManager, "Courier New"));
        viewMenu.add(new JvFontAction(table, undoManager, "Garamond"));
        viewMenu.add(new JvFontAction(table, undoManager, "Sans Serif"));
        viewMenu.add(new JvFontAction(table, undoManager, "Times New Roman"));
        viewMenu.addSeparator();
        viewMenu.add(new JvZoomAction(table, undoManager, 200));
        viewMenu.add(new JvZoomAction(table, undoManager, 100));
        viewMenu.add(new JvZoomAction(table, undoManager, 75));
        viewMenu.add(new JvZoomAction(table, undoManager, 50));
        viewMenu.add(new JvZoomAction(table, undoManager, 25));

        JMenuBar menuBar = new JMenuBar();
        menuBar.add(editMenu);
        menuBar.add(viewMenu);

        JFrame frame = new JFrame("Zoomable JTable");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setJMenuBar(menuBar);
        frame.add(pane, BorderLayout.CENTER);
        frame.setSize(400, 250);
        frame.setLocation(200, 300);
        frame.setVisible(true);
    }
}


class JvTable extends JTable
{
    private Font originalFont;
    private int originalRowHeight;
    private float zoomFactor = 1.0f;


    public JvTable(TableModel dataModel)
    {
        super(dataModel);
    }


    @Override
    public void setFont(Font font)
    {
        originalFont = font;
        // When setFont() is first called, zoomFactor is 0.
        if (zoomFactor != 0.0 && zoomFactor != 1.0)
        {
            float scaledSize = originalFont.getSize2D() * zoomFactor;
            font = originalFont.deriveFont(scaledSize);
        }

        super.setFont(font);
    }


    @Override
    public void setRowHeight(int rowHeight)
    {
        originalRowHeight = rowHeight;
        // When setRowHeight() is first called, zoomFactor is 0.
        if (zoomFactor != 0.0 && zoomFactor != 1.0)
            rowHeight = (int) Math.ceil(originalRowHeight * zoomFactor);

        super.setRowHeight(rowHeight);

    }

    public float getZoom()
    {
        return zoomFactor;
    }


    public void setZoom(float zoomFactor)
    {
        if (this.zoomFactor == zoomFactor)
            return;

        if (originalFont == null)
            originalFont = getFont();
        if (originalRowHeight == 0)
            originalRowHeight = getRowHeight();

        float oldZoomFactor = this.zoomFactor;
        this.zoomFactor = zoomFactor;
        Font font = originalFont;
        if (zoomFactor != 1.0)
        {
            float scaledSize = originalFont.getSize2D() * zoomFactor;
            font = originalFont.deriveFont(scaledSize);
        }

        super.setFont(font);
        super.setRowHeight((int) Math.ceil(originalRowHeight * zoomFactor));
        getTableHeader().setFont(font);
        calcColumnWidths();
        firePropertyChange("zoom", oldZoomFactor, zoomFactor);
    }


    @Override
    public Component prepareRenderer(TableCellRenderer renderer, int row, int column)
    {
        Component comp = super.prepareRenderer(renderer, row, column);
        comp.setFont(this.getFont());
        return comp;
    }


    @Override
    public Component prepareEditor(TableCellEditor editor, int row, int column)
    {
        Component comp = super.prepareEditor(editor, row, column);
        comp.setFont(this.getFont());
        return comp;
    }
    /*
     * Adjust the column width to the biggest cell/header
     */
	public void calcColumnWidths()
	{
		JTableHeader header = this.getTableHeader();
		TableCellRenderer defaultHeaderRenderer = null;
		if (header != null)
		defaultHeaderRenderer = header.getDefaultRenderer();
	
		TableColumnModel columns = this.getColumnModel();
		TableModel data = this.getModel();
		int margin = columns.getColumnMargin(); 
		int rowCount = data.getRowCount();
		int totalWidth = 0;
	
		for (int i = columns.getColumnCount() - 1; i >= 0; --i)
		{
			TableColumn column = columns.getColumn(i);
			int columnIndex = column.getModelIndex();
			int width = -1;
	
			TableCellRenderer h = column.getHeaderRenderer();
	
			if (h == null)
				h = defaultHeaderRenderer;
	
			if (h != null)
			{
				Component c = h.getTableCellRendererComponent(this, column.getHeaderValue(), false, false, -1, i);
				width = c.getPreferredSize().width;
			}
	
			for (int row = rowCount - 1; row >= 0; --row)
			{
				TableCellRenderer r = this.getCellRenderer(row, i);
				Component c = r.getTableCellRendererComponent(this, data.getValueAt(row, columnIndex), false, false,
				row, i);
				width = Math.max(width, c.getPreferredSize().width);
			}
		
			if (width >= 0)
				column.setPreferredWidth(width + margin); 
			else
				totalWidth += column.getPreferredWidth();
		}
	}
}


class JvUndoableTableModel extends DefaultTableModel implements Cloneable
{
    public JvUndoableTableModel(Object[][] data, Object[] columnNames)
    {
        super(data, columnNames);
    }
    public JvUndoableTableModel(Vector data,Vector columnNames){
    	super(data,columnNames);
    }
    public JvUndoableTableModel(JvUndoableTableModel tm){
    	super(tm.getDataVector(),tm.getColumnNames());
    }
    public Vector getColumnNames(){
    	Vector columnNames=new Vector();
    	for(int i=0;i<this.getColumnCount();i++)
    		columnNames.add(this.getColumnName(i));
    	return columnNames;
    }
    //disable Cell editing
    public boolean isCellEditable(int rowIndex, int mColIndex) {
        return false;
    }
    public Vector getColumnIdentifiers() {
        return columnIdentifiers;
    }

    public Class getColumnClass(int column)
    {
        if (column >= 0 && column < getColumnCount())
            return getValueAt(0, column).getClass();

        return Object.class;
    }
    @Override
    public void setValueAt(Object value, int row, int column)
    {
        setValueAt(value, row, column, true);
    }


    public void setValueAt(Object value, int row, int column, boolean undoable)
    {
        UndoableEditListener[] listeners = getListeners(UndoableEditListener.class);
        if (undoable == false || listeners == null)
        {
            super.setValueAt(value, row, column);
            return;
        }


        Object oldValue = getValueAt(row, column);
        super.setValueAt(value, row, column);
        JvCellEdit cellEdit = new JvCellEdit(this, oldValue, value, row, column);
        fireUndoableEdit(listeners, cellEdit);
    }


    public void addUndoableEditListener(UndoableEditListener listener)
    {
        listenerList.add(UndoableEditListener.class, listener);
    }


    public void fireUndoableEdit(UndoableEdit edit)
    {
        UndoableEditListener[] listeners = getListeners(UndoableEditListener.class);
        fireUndoableEdit(listeners, edit);
    }


    protected void fireUndoableEdit(UndoableEditListener[] listeners, UndoableEdit edit)
    {
        UndoableEditEvent editEvent = new UndoableEditEvent(this, edit);
        for (UndoableEditListener listener : listeners)
            listener.undoableEditHappened(editEvent);
    }
    //Hide/show column
    
}


class JvCellEdit extends AbstractUndoableEdit
{
    protected JvUndoableTableModel tableModel;
    protected Object oldValue;
    protected Object newValue;
    protected int row;
    protected int column;


    public JvCellEdit(JvUndoableTableModel tableModel, Object oldValue, Object newValue, int row, int column)
    {
        this.tableModel = tableModel;
        this.oldValue = oldValue;
        this.newValue = newValue;
        this.row = row;
        this.column = column;
    }


    @Override
    public String getPresentationName()
    {
        return "Cell Edit";
    }


    @Override
    public void undo() throws CannotUndoException
    {
        super.undo();

        tableModel.setValueAt(oldValue, row, column, false);
    }


    @Override
    public void redo() throws CannotUndoException
    {
        super.redo();

        tableModel.setValueAt(newValue, row, column, false);
    }
}


class JvUndoManager extends UndoManager
{
    protected Action undoAction;
    protected Action redoAction;


    public JvUndoManager()
    {
        this.undoAction = new JvUndoAction(this);
        this.redoAction = new JvRedoAction(this);

        synchronizeActions();           // to set initial names
    }


    public Action getUndoAction()
    {
        return undoAction;
    }


    public Action getRedoAction()
    {
        return redoAction;
    }


    @Override
    public boolean addEdit(UndoableEdit anEdit)
    {
        try
        {
            return super.addEdit(anEdit);
        }
        finally
        {
            synchronizeActions();
        }
    }


    @Override
    protected void undoTo(UndoableEdit edit) throws CannotUndoException
    {
        try
        {
            super.undoTo(edit);
        }
        finally
        {
            synchronizeActions();
        }
    }


    @Override
    protected void redoTo(UndoableEdit edit) throws CannotRedoException
    {
        try
        {
            super.redoTo(edit);
        }
        finally
        {
            synchronizeActions();
        }
    }


    protected void synchronizeActions()
    {
        undoAction.setEnabled(canUndo());
        undoAction.putValue(Action.NAME, getUndoPresentationName());

        redoAction.setEnabled(canRedo());
        redoAction.putValue(Action.NAME, getRedoPresentationName());
    }
}

class JvSaveAction extends AbstractAction{
	private JvUndoableTableModel tableModel;
	private JFileChooser fc;
	private File outputfile;
	public JvSaveAction(JvUndoableTableModel tableModel,JFileChooser fc){
		super();
		this.tableModel=tableModel;
		this.fc=fc;
		outputfile=null;
	}
	public void actionPerformed(ActionEvent e) {
		if(outputfile==null){
	         if (fc.showSaveDialog(null) == JFileChooser.APPROVE_OPTION)
	         {
	            File f = fc.getSelectedFile();

	            // check if file exists before overwriting
	            // (Note: Dialog only pops up if file exists)
	            if (!f.exists() || okToReplace(f))
	            	outputfile=f;
	         }
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
class JvExitAction extends AbstractAction{
	public void actionPerformed(ActionEvent e){
		System.exit(0);
	}
}
class JvUndoAction extends AbstractAction
{
    protected final UndoManager manager;


    public JvUndoAction(UndoManager manager)
    {
        this.manager = manager;
    }


    public void actionPerformed(ActionEvent e)
    {
        try
        {
            manager.undo();
        }
        catch (CannotUndoException ex)
        {
            ex.printStackTrace();
        }
    }
}


class JvRedoAction extends AbstractAction
{
    protected final UndoManager manager;


    public JvRedoAction(UndoManager manager)
    {
        this.manager = manager;
    }


    public void actionPerformed(ActionEvent e)
    {
        try
        {
            manager.redo();
        }
        catch (CannotRedoException ex)
        {
            ex.printStackTrace();
        }
    }
}


interface JvCellFill
{
    public void doFill(JvUndoableTableModel model);
    public void undoFill(JvUndoableTableModel model);
}


class JvColumnFill implements JvCellFill
{
    protected int columnIndex;
    protected int[] rowIndices;
    protected Object[] values;


    public JvColumnFill(JvUndoableTableModel model, int columnIndex, int[] rowIndices)
    {
        this.columnIndex = columnIndex;
        this.rowIndices = rowIndices;

        values = new Object[rowIndices.length];
        for (int i = 0; i < rowIndices.length; i++)
            values[i] = model.getValueAt(rowIndices[i], columnIndex);
    }


    public void doFill(JvUndoableTableModel model)
    {
        for (int i = 1; i < rowIndices.length; i++)
        {
            if (model.isCellEditable(rowIndices[i], columnIndex))
                model.setValueAt(values[0], rowIndices[i], columnIndex, false);
        }
    }


    public void undoFill(JvUndoableTableModel model)
    {
        for (int i = 1; i < rowIndices.length; i++)
        {
            if (model.isCellEditable(rowIndices[i], columnIndex))
                model.setValueAt(values[i], rowIndices[i], columnIndex, false);
        }
    }
}


class JvCellFillEdit extends AbstractUndoableEdit
{
    protected JvUndoableTableModel tableModel;
    protected ArrayList<JvCellFill> fills = new ArrayList<JvCellFill>();


    public JvCellFillEdit(JvUndoableTableModel tableModel)
    {
        this.tableModel = tableModel;
    }


    public void addFill(JvCellFill fill)
    {
        fills.add(fill);
    }


    @Override
    public String getPresentationName()
    {
        return "Cell Fill";
    }


    public void fill()
    {
        for (JvCellFill fill : fills)
            fill.doFill(tableModel);
    }


    @Override
    public void undo() throws CannotUndoException
    {
        super.undo();

        for (JvCellFill fill : fills)
            fill.undoFill(tableModel);
    }


    @Override
    public void redo() throws CannotUndoException
    {
        super.redo();

        fill();
    }
}


class JvFillDownAction extends AbstractAction
{
    protected JTable table;


    public JvFillDownAction(JTable table)
    {
        super("Fill Down");
        this.table = table;

        // As far as I can tell, in Excel the "Fill Down" action is always
        // enabled.
        setEnabled(true);

        KeyStroke accel = KeyStroke.getKeyStroke('D', InputEvent.CTRL_MASK);
        putValue(ACCELERATOR_KEY, accel);

        table.getInputMap().put(accel, "fillDown");
        table.getActionMap().put("fillDown", this);
    }


    public void actionPerformed(ActionEvent event)
    {
        // Cell selection in JTable looks like a hack.  Basically it looks like
        // the union of column and row selection.
        int[] rowIndices = table.getSelectedRows();
        if (rowIndices.length == 0)
            return;

        JvUndoableTableModel model = (JvUndoableTableModel) table.getModel();
        JvCellFillEdit fillEdit = new JvCellFillEdit(model);

        for (int columnIndex : table.getSelectedColumns())
        {
            columnIndex = table.convertColumnIndexToModel(columnIndex);
            JvColumnFill fill = new JvColumnFill(model, columnIndex, rowIndices);
            fillEdit.addFill(fill);
        }

        fillEdit.fill();
        model.fireUndoableEdit(fillEdit);
    }
}


class JvTableZoomEdit extends AbstractUndoableEdit
{
    protected JvTable table;
    protected float oldValue;
    protected float newValue;


    public JvTableZoomEdit(JvTable table, float oldValue, float newValue)
    {
        this.table = table;
        this.oldValue = oldValue;
        this.newValue = newValue;
    }


    @Override
    public String getPresentationName()
    {
        return "Zoom";
    }


    public void doit()
    {
        table.setZoom(newValue);
    }


    @Override
    public void undo() throws CannotUndoException
    {
        super.undo();

        table.setZoom(oldValue);
    }


    @Override
    public void redo() throws CannotUndoException
    {
        super.redo();

        table.setZoom(newValue);
    }
}


class JvZoomAction extends AbstractAction
{
    protected JvTable table;
    protected UndoManager undoManager;
    protected float scaleFactor;


    public JvZoomAction(JvTable table, UndoManager undoManager, int percent)
    {
        super("Zoom " + percent + "%");
        this.table = table;
        this.undoManager = undoManager;
        scaleFactor = percent / 100.0f;
    }


    public void actionPerformed(ActionEvent event)
    {
        float oldScaleFactor = table.getZoom();
        if (oldScaleFactor == scaleFactor)
            return;

        JvTableZoomEdit edit = new JvTableZoomEdit(table, oldScaleFactor, scaleFactor);
        edit.doit();
        UndoableEditEvent editEvent = new UndoableEditEvent(table, edit);
        undoManager.undoableEditHappened(editEvent);
    }
}


class JvFontEdit extends AbstractUndoableEdit
{
    protected JComponent component;
    protected Font oldValue;
    protected Font newValue;


    public JvFontEdit(JComponent component, Font oldValue, Font newValue)
    {
        this.component = component;
        this.oldValue = oldValue;
        this.newValue = newValue;
    }


    @Override
    public String getPresentationName()
    {
        return "Font";
    }


    @Override
    public String getUndoPresentationName()
    {
        return super.getUndoPresentationName() + ' ' + getFontSpec(oldValue);
    }


    @Override
    public String getRedoPresentationName()
    {
        return super.getRedoPresentationName() + ' ' + getFontSpec(newValue);
    }


    protected String getFontSpec(Font font)
    {
        return font.getFontName();
    }


    public void doit()
    {
        setFont(newValue);
    }


    @Override
    public void undo() throws CannotUndoException
    {
        super.undo();

        setFont(oldValue);
    }


    @Override
    public void redo() throws CannotUndoException
    {
        super.redo();

        setFont(newValue);
    }


    protected void setFont(Font font)
    {
        component.setFont(font);
        if (component instanceof JTable)
            ((JTable) component).getTableHeader().setFont(font);
    }
}


class JvFontAction extends AbstractAction
{
    protected JComponent component;
    protected UndoManager undoManager;
    protected String fontSpec;


    public JvFontAction(JComponent component, UndoManager undoManager, String fontSpec)
    {
        super("Font " + fontSpec);
        this.component = component;
        this.undoManager = undoManager;
        this.fontSpec = fontSpec.replace(" ", "-");
    }


    public void actionPerformed(ActionEvent event)
    {
        Font oldFont = component.getFont();
        Font font = Font.decode(fontSpec);

        JvFontEdit edit = new JvFontEdit(component, oldFont, font);
        edit.doit();
        UndoableEditEvent editEvent = new UndoableEditEvent(component, edit);
        undoManager.undoableEditHappened(editEvent);
    }
}
class MyFileFilter extends FileFilter
{
   private String[] s;

   MyFileFilter(String[] sArg)
   {
      s = sArg;
   }

   // determine which files to display in the chooser
   public boolean accept(File fArg)
   {
      // if it's a directory, show it
      if (fArg.isDirectory())
      return true;

      // if the filename contains the extension, show it
      for (int i = 0; i < s.length; ++i)
         if (fArg.getName().toLowerCase().indexOf(s[i].toLowerCase()) > 0)
            return true;

      // filter out everything else
      return false;
   }

   public String getDescription()
   {
      String tmp = "";
      for (int i = 0; i < s.length; ++i)
      tmp += "*" + s[i] + " ";

      return tmp;
   }
}
