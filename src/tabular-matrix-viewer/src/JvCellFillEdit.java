
import javax.swing.*;
import javax.swing.undo.*;

import java.util.*;

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


