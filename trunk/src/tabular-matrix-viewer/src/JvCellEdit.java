
import javax.swing.undo.*;

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


