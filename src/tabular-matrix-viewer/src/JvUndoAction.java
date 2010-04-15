
import javax.swing.*;
import javax.swing.event.*;
import java.awt.event.*;
import javax.swing.undo.*;


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


