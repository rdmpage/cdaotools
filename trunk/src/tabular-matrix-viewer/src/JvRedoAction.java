
import javax.swing.*;
import javax.swing.event.*;
import java.awt.event.*;

import javax.swing.undo.*;

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

