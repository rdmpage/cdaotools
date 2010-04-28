package matrix.view;

import javax.swing.undo.*;
import javax.swing.event.*;
import java.awt.event.*;

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




