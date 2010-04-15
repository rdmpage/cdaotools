
import java.util.*;

import java.awt.*;
import java.awt.event.*;

import javax.swing.*;
import javax.swing.event.*;
import javax.swing.undo.*;
import javax.swing.table.*;


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


