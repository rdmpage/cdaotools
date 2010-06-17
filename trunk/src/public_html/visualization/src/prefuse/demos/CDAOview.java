package prefuse.demos;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.geom.Point2D;
import java.awt.geom.Rectangle2D;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileFilter;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.MalformedURLException;
import java.net.URL;

import javax.swing.AbstractAction;
import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.KeyStroke;
import javax.swing.ListSelectionModel;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.plaf.basic.BasicButtonListener;

import prefuse.Display;
import prefuse.Visualization;
import prefuse.action.ActionList;
import prefuse.action.RepaintAction;
import prefuse.action.assignment.ColorAction;
import prefuse.action.distortion.BifocalDistortion;
import prefuse.action.filter.GraphDistanceFilter;
import prefuse.action.layout.graph.BalloonTreeLayout;
import prefuse.action.layout.graph.ForceDirectedLayout;
import prefuse.action.layout.graph.FruchtermanReingoldLayout;
import prefuse.action.layout.graph.NodeLinkTreeLayout;
import prefuse.action.layout.graph.RadialTreeLayout;
import prefuse.action.layout.graph.SquarifiedTreeMapLayout;
import prefuse.activity.Activity;
import prefuse.controls.AnchorUpdateControl;
import prefuse.controls.DragControl;
import prefuse.controls.FocusControl;
import prefuse.controls.NeighborHighlightControl;
import prefuse.controls.PanControl;
import prefuse.controls.ToolTipControl;
import prefuse.controls.WheelZoomControl;
import prefuse.controls.ZoomControl;
import prefuse.controls.ZoomToFitControl;
import prefuse.data.Graph;
import prefuse.data.Table;
import prefuse.data.Tuple;
import prefuse.data.event.TupleSetListener;
import prefuse.data.io.DataIOException;
import prefuse.data.io.GraphMLReader;
import prefuse.data.tuple.TupleSet;
import prefuse.render.DefaultRendererFactory;
import prefuse.render.LabelRenderer;
import prefuse.util.ColorLib;
import prefuse.util.GraphLib;
import prefuse.util.GraphicsLib;
import prefuse.util.display.DisplayLib;
import prefuse.util.display.ItemBoundsListener;
import prefuse.util.ui.UILib;
import prefuse.visual.VisualGraph;
import prefuse.visual.VisualItem;

/**
 * @author <a href="http://jheer.org">jeffrey heer</a>
 */
public class CDAOview extends JPanel {

	/** String name for visual graph*/
    private static final String graph = "graph";
    /** String name for nodes of visual graph*/
    private static final String nodes = "graph.nodes";
    /** String name for edges of visual graph*/
    private static final String edges = "graph.edges";
    /** Base URL where files for visual trees will be located*/
    private static final String BASE_URI = "http://www.cs.nmsu.edu/~cdaostore/cgi-bin/phylows/tree";
    /** Default tree to load*/
    private static final String DEFAULT_GRAPH_URL= BASE_URI + "/" + "Tree3099?format=graphml";
    /** this visualization */
    private Visualization m_vis;
    
    /** This CDAOviews graph*/
    private Graph m_graph;
    /** This graph's ForceDirected layout, can be switched with nltl*/
    protected static ForceDirectedLayout fdl;
    /** This graph's NodeLink Layout, can be switched with fdl*/
    protected static NodeLinkTreeLayout nltl;
    /** This the they 'fisheye' lens, can be turned on/off */
    protected static BifocalDistortion feye;
    /** The display, has access to the actions/controls used*/
    protected static Display display;
    /** This is the control that looks over feye*/
    protected static AnchorUpdateControl fishUpdate;
    
    /**
     * 
     * @param g Graph to be loaded into CDAOview
     * @param label The key name to show as a label in the visualization
     */
    public CDAOview(Graph g, String label) {
    	super(new BorderLayout());
    	
        // create a new, empty visualization for our data
        m_vis = new Visualization();
        m_graph = g;
        
        // --------------------------------------------------------------------
        // set up the renderers
        
        LabelRenderer tr = new LabelRenderer();
        tr.setRoundedCorner(8, 8);
        m_vis.setRendererFactory(new DefaultRendererFactory(tr));

        // --------------------------------------------------------------------
        // register the data with a visualization
        
        // adds graph to visualization and sets renderer label field
        setGraph(g, label);
        
        // fix selected focus nodes
        TupleSet focusGroup = m_vis.getGroup(Visualization.FOCUS_ITEMS); 
        focusGroup.addTupleSetListener(new TupleSetListener() {
            public void tupleSetChanged(TupleSet ts, Tuple[] add, Tuple[] rem)
            {
                for ( int i=0; i<rem.length; ++i )
                    ((VisualItem)rem[i]).setFixed(false);
                for ( int i=0; i<add.length; ++i ) {
                    ((VisualItem)add[i]).setFixed(false);
                    ((VisualItem)add[i]).setFixed(true);
                }
                if ( ts.getTupleCount() == 0 ) {
                    ts.addTuple(rem[0]);
                    ((VisualItem)rem[0]).setFixed(false);
                }
                m_vis.run("draw");
            }
        });
        
        
        
        // --------------------------------------------------------------------
        // create actions to process the visual data

        int hops = 30;
        final GraphDistanceFilter filter = new GraphDistanceFilter(graph, hops);

        ColorAction fill = new ColorAction(nodes, 
                VisualItem.FILLCOLOR, ColorLib.rgb(200,200,255));
        fill.add(VisualItem.FIXED, ColorLib.rgb(255,100,100));
        fill.add(VisualItem.HIGHLIGHT, ColorLib.rgb(255,200,125));
        
        feye = new BifocalDistortion(0.05, 2.0);
        
        ActionList draw = new ActionList();
        draw.add(filter);
        //draw.add(feye);
        draw.add(fill);
        draw.add(new ColorAction(nodes, VisualItem.STROKECOLOR, 0));
        draw.add(new ColorAction(nodes, VisualItem.TEXTCOLOR, ColorLib.rgb(0,0,0)));
        draw.add(new ColorAction(edges, VisualItem.FILLCOLOR, ColorLib.gray(200)));
        draw.add(new ColorAction(edges, VisualItem.STROKECOLOR, ColorLib.gray(200)));
        
        ActionList animate = new ActionList(Activity.INFINITY);
        fdl = new ForceDirectedLayout(graph,false, false);
        animate.add(feye);//new BifocalDistortion(.1, 5.0));
        animate.add(fdl);
        animate.add(fill);
        animate.add(new RepaintAction());
        
        // finally, we register our ActionList with the Visualization.
        // we can later execute our Actions by invoking a method on our
        // Visualization, using the name we've chosen below.
        m_vis.putAction("draw", draw);
        m_vis.putAction("layout", animate);

        
        
        m_vis.runAfter("draw", "layout");
        
        
        // --------------------------------------------------------------------
        // set up a display to show the visualization
        
        display = new Display(m_vis);
        display.setSize(700,700);
        display.pan(350, 350);
        display.setForeground(Color.GRAY);
        display.setBackground(Color.WHITE);
        
        // main display controls
        fishUpdate = new AnchorUpdateControl(feye, "layout");
        display.addControlListener(fishUpdate);
        display.addControlListener(new FocusControl(1));
        display.addControlListener(new DragControl());
        display.addControlListener(new PanControl());
        display.addControlListener(new ZoomControl());
        display.addControlListener(new WheelZoomControl());
        display.addControlListener(new ZoomToFitControl());
        display.addControlListener(new ToolTipControl("IdLabel"));
        display.addControlListener(new NeighborHighlightControl());	
        
        display.setForeground(Color.GRAY);
        display.setBackground(Color.WHITE);
        
        // --------------------------------------------------------------------        
        // launch the visualization
        
        // create a panel for editing force values
        /*ForceSimulator fsim = ((ForceDirectedLayout)animate.get(0)).getForceSimulator();
        JForcePanel fpanel = new JForcePanel(fsim);
        
//        JPanel opanel = new JPanel();
//        opanel.setBorder(BorderFactory.createTitledBorder("Overview"));
//        opanel.setBackground(Color.WHITE);
//        opanel.add(overview);
        
        final JValueSlider slider = new JValueSlider("Distance", 0, hops, hops);
        slider.addChangeListener(new ChangeListener() {
            public void stateChanged(ChangeEvent e) {
                filter.setDistance(slider.getValue().intValue());
                m_vis.run("draw");
            }
        });
        slider.setBackground(Color.WHITE);
        slider.setPreferredSize(new Dimension(300,30));
        slider.setMaximumSize(new Dimension(300,30));
        
        Box cf = new Box(BoxLayout.Y_AXIS);
        cf.add(slider);
        cf.setBorder(BorderFactory.createTitledBorder("Connectivity Filter"));
        //fpanel.add(cf);
*/
        //fpanel.add(opanel);
        
        //fpanel.add(Box.createVerticalGlue());
        
        // create a new JSplitPane to present the interface
        //JSplitPane split = new JSplitPane();
        //split.setLeftComponent(display);  //Component(display);
        //split.setRightComponent(fpanel);
        //split.setOneTouchExpandable(true);
        //split.setContinuousLayout(false);
        //split.setDividerLocation(700);
        //split.setVisible(true);
      
        // now we run our action list
        m_vis.run("draw");
        add(display);          

    }
    
    /**
     * This returns the graph of the current CDAOview
     * @return m_graph
     */
    public Graph getGraph()
    {
    	return m_graph;
    }
    
    public void setGraph(Graph g, String label) {
        // update labeling
        DefaultRendererFactory drf = (DefaultRendererFactory)
                                                m_vis.getRendererFactory();
        ((LabelRenderer)drf.getDefaultRenderer()).setTextField(label);
        
        // update graph
        m_vis.removeGroup(graph);
        VisualGraph vg = m_vis.addGraph(graph, g);
        m_vis.setValue(edges, null, VisualItem.INTERACTIVE, Boolean.FALSE);
        VisualItem f = (VisualItem)vg.getNode(0);
        m_vis.getGroup(Visualization.FOCUS_ITEMS).setTuple(f);
        f.setFixed(false);
    }
    
    // ------------------------------------------------------------------------
    // Main and demo methods
    
    public static void main(String[] args) {
        UILib.setPlatformLookAndFeel();
        
        // create cdaoview
        
	System.err.print("Called with: ");
	for (int i = 0; i < args.length; ++i ){
            System.err.print( args[i] + " " );
	}
        System.err.println();
        String datafile = DEFAULT_GRAPH_URL;
        
        String label = "IdLabel";
        if ( args.length > 1 ) {
            datafile = args[0];
            label = args[1];
        }
        
        JFrame frame = demo(datafile, label);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }
    
    public static JFrame demo() {
        return demo((String)null, "label");
    }
    
    public static JFrame demo(String datafile, String label) {
        Graph g = null;
        if ( datafile == null ) {
            g = GraphLib.getGrid(15,15);
            label = "label";
        } else {
            try {
                g = new GraphMLReader().readGraph( new URL( datafile ));
            } catch ( Exception e ) {
                e.printStackTrace();
                System.exit(1);
            }
        }
        return demo(g, label);
    }
    
    public static JFrame demo( URL datafile, String label  ){
        Graph g = null;
        if ( datafile == null ) {
            g = GraphLib.getGrid(15,15);
            label = "label";
        } else {
            try {
                g = new GraphMLReader().readGraph(  datafile );
            } catch ( Exception e ) {
                e.printStackTrace();
                System.exit(1);
            }
        }
        return demo(g, label);
    }

    public static JFrame demo( File datafile, String label ){
        Graph g = null;
        if ( datafile == null ) {
            g = GraphLib.getGrid(15,15);
            label = "label";
        } else {
            try {
                g = new GraphMLReader().readGraph(  datafile );
            } catch ( Exception e ) {
                e.printStackTrace();
                System.exit(1);
            }
        }
        return demo(g, label);
    }

    public static JFrame demo(Graph g, String label) {
        final CDAOview view = new CDAOview(g, label);
        
        // set up menu
        JMenu dataMenu = new JMenu("Layout");
        dataMenu.setMnemonic(KeyEvent.VK_L);
        dataMenu.add(new ForceLayoutAction(view));
        dataMenu.add(new NodeLinkTreeLayoutAction(view));
        
        JMenu effectMenu = new JMenu("Effects");
        effectMenu.setMnemonic(KeyEvent.VK_E);
        FisheyeZoomAction fza = new FisheyeZoomAction(view);
        effectMenu.add(fza);
        
        JMenu saveMenu = new JMenu("Save");
        saveMenu.setMnemonic(KeyEvent.VK_S);
        saveMenu.add(new SaveImageAction(view));
        
        JMenuBar menubar = new JMenuBar();
        menubar.add(dataMenu);
        menubar.add(effectMenu);
        menubar.add(saveMenu);
        
        // launch window
        JFrame frame = new JFrame("Tree Viewer | powered by prefuse");
        frame.setJMenuBar(menubar);
        frame.setContentPane(view);
        
        frame.pack();
        frame.setVisible(true);
        view.setVisible(true);
        view.setGraph(g, label);
        view.m_vis.runAfter("draw", "layout");
        view.m_vis.run("layout");
        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowActivated(WindowEvent e) {
                view.m_vis.run("layout");
            }
            @Override
            public void windowDeactivated(WindowEvent e) {
                view.m_vis.cancel("layout");
            }
        });
        return frame;
    }
    
    
    // ------------------------------------------------------------------------
    
    /**
     * Swing menu action that loads a graph into the graph viewer.
     */
    public abstract static class GraphMenuAction extends AbstractAction {
        private CDAOview m_view;
        public GraphMenuAction(String name, String accel, CDAOview view) {
            m_view = view;
            this.putValue(AbstractAction.NAME, name);
            this.putValue(AbstractAction.ACCELERATOR_KEY,
                          KeyStroke.getKeyStroke(accel));
        }
        public void actionPerformed(ActionEvent e) {
            m_view.setGraph(getGraph(), "label");
        }
        protected abstract Graph getGraph();
    }
    /**
     * This menu option saves the graph as an image.
     * @author Ben Wright
     *
     */
    public static class SaveImageAction extends AbstractAction {
    	private CDAOview m_view;
    	private JFileChooser jfc;
    	public SaveImageAction(CDAOview view){
    		m_view = view;
    		jfc = new JFileChooser();
                //jfc.setDialogType( JFileChooser.SAVE_DIALOG );
    		jfc.setFileFilter((javax.swing.filechooser.FileFilter) new ImgFileFilter());
    		this.putValue(AbstractAction.NAME, "Save Graph As...");
    	}
    	public void actionPerformed(ActionEvent e)
    	{   
    		int ret_val = jfc.showSaveDialog(m_view);
    		File f;
    	    FileOutputStream fos = null;
    	    if(ret_val == JFileChooser.APPROVE_OPTION)
    	    {
    	    	f = jfc.getSelectedFile();
    	    	//String format = jfc.getChoosableFileFilters();
    	    	try {
    	    		fos = new FileOutputStream(f);
    	    	} catch (FileNotFoundException e1) {
    	    		// TODO Auto-generated catch block
    	    		e1.printStackTrace();
    	    	}
    	    	String name = f.getName().toLowerCase();
    	    	if(name.endsWith("jpg") || name.endsWith("jpeg"))
    	    	  display.saveImage(fos, "JPG", 2.0);
    	    	else if(name.endsWith("png"))
    	    		display.saveImage(fos, "PNG", 2.0);
    	    	else
    	    		System.out.println(name + " was selected to be saved as, but is not jpg, jpeg, or png.");
    	    }
    	    try {
				fos.close();
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
    	}
    	private class ImgFileFilter extends javax.swing.filechooser.FileFilter
    	{

			@Override
			public boolean accept(File f) {
				
				if(f.isDirectory()) return true;
				String name = f.getName().toLowerCase();
				return name.endsWith("jpg") || name.endsWith("jpeg") || name.endsWith("png");
			}

			@Override
			public String getDescription() {
				// TODO Auto-generated method stub
				return "jpg, jpeg, and png";
			}
    	}
    }
    
    /**
     * Swing menu action that loads a graph into a nice tree structure from left to right.
     * This layout is switched around with the Force Layout.
     * @author Ben Wright
     */
    public static class NodeLinkTreeLayoutAction extends AbstractAction {
    	private CDAOview m_view;
    	public NodeLinkTreeLayoutAction(CDAOview view){
    		m_view = view;
    		this.putValue(AbstractAction.NAME, "Node Link Tree Layout");
    	}
    	public void actionPerformed(ActionEvent e)
    	{
    	  ActionList l = (ActionList) m_view.m_vis.getAction("layout");
    	  fdl.cancel();
    	  l.remove(fdl);
    	  nltl = new NodeLinkTreeLayout(graph);
    	  l.add(nltl);
    	  m_view.m_vis.putAction("layout", l);
    	  m_view.m_vis.run("draw");
    	}
    }
    /**
     * Swing menu action that loads a graph into a the visualization that allows it to "wander" and be moved around as if on springs.
     * This layout is switched around with the Node Link Tree Layout
     * @author Ben Wright
     *
     */
    public static class ForceLayoutAction extends AbstractAction {
        private CDAOview m_view;
        public ForceLayoutAction(CDAOview view) {
            m_view = view;
            this.putValue(AbstractAction.ACCELERATOR_KEY,
                          KeyStroke.getKeyStroke("ctrl f"));
            this.putValue(AbstractAction.NAME, "Force Layout");
        }
        public void actionPerformed(ActionEvent e) {
         	  ActionList l = (ActionList) m_view.m_vis.getAction("layout");
         	  nltl.cancel();
         	  l.remove(nltl);
        	  l.add(fdl);
        	  fdl.reset();
        	  fdl.run();
        	  m_view.m_vis.putAction("layout", l);
        	  m_view.m_vis.run("draw");
        }
        public static String getLabel(Component c, Graph g) {
            // get the column names
            Table t = g.getNodeTable();
            int  cc = t.getColumnCount();
            String[] names = new String[cc];
            for ( int i=0; i<cc; ++i )
                names[i] = t.getColumnName(i);
            
            // where to store the result
            final String[] label = new String[1];

            // -- build the dialog -----
            // we need to get the enclosing frame first
            while ( c != null && !(c instanceof JFrame) ) {
                c = c.getParent();
            }
            final JDialog dialog = new JDialog(
                    (JFrame)c, "Choose Label Field", true);
            
            // create the ok/cancel buttons
            final JButton ok = new JButton("OK");
            ok.setEnabled(false);
            ok.addActionListener(new ActionListener() {
               public void actionPerformed(ActionEvent e) {
                   dialog.setVisible(false);
               }
            });
            JButton cancel = new JButton("Cancel");
            cancel.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    label[0] = null;
                    dialog.setVisible(false);
                }
            });
            
            // build the selection list
            final JList list = new JList(names);
            list.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
            list.getSelectionModel().addListSelectionListener(
            new ListSelectionListener() {
                public void valueChanged(ListSelectionEvent e) {
                    int sel = list.getSelectedIndex(); 
                    if ( sel >= 0 ) {
                        ok.setEnabled(true);
                        label[0] = (String)list.getModel().getElementAt(sel);
                    } else {
                        ok.setEnabled(false);
                        label[0] = null;
                    }
                }
            });
            JScrollPane scrollList = new JScrollPane(list);
            
            JLabel title = new JLabel("Choose a field to use for node labels:");
            
            // layout the buttons
            Box bbox = new Box(BoxLayout.X_AXIS);
            bbox.add(Box.createHorizontalStrut(5));
            bbox.add(Box.createHorizontalGlue());
            bbox.add(ok);
            bbox.add(Box.createHorizontalStrut(5));
            bbox.add(cancel);
            bbox.add(Box.createHorizontalStrut(5));
            
            // put everything into a panel
            JPanel panel = new JPanel(new BorderLayout());
            panel.add(title, BorderLayout.NORTH);
            panel.add(scrollList, BorderLayout.CENTER);
            panel.add(bbox, BorderLayout.SOUTH);
            panel.setBorder(BorderFactory.createEmptyBorder(5,2,2,2));
            
            // show the dialog
            dialog.setContentPane(panel);
            dialog.pack();
            dialog.setLocationRelativeTo(c);
            dialog.setVisible(true);
            dialog.dispose();
            
            // return the label field selection
            return label[0];
        }
    }
    protected static boolean fishFlag = true;
    /**
     * Swing menu action that turns on/off the effect of a 'fish eye' lens.
     * @author Ben Wright
     *
     */
    public static class FisheyeZoomAction extends AbstractAction {
    	private CDAOview m_view;
    	public FisheyeZoomAction(CDAOview view){
    		m_view = view;
    		this.putValue(AbstractAction.NAME, "Toggle Fisheye Zooming");
    		this.putValue(MNEMONIC_KEY, KeyEvent.VK_F);
    	
    	}
    	public void actionPerformed(ActionEvent e)
    	{
    	  if(fishFlag) //flag on
    	  {
    		  feye.setEnabled(false);
    		  fishUpdate.setEnabled(false);
    		  fishFlag = false;
    	  }
    	  else //flag off
    	  {
    		feye.setEnabled(true);
    		fishUpdate.setEnabled(true);
    		fishFlag = true;
    	  }
    	}
    }
    
    public static class FitOverviewListener implements ItemBoundsListener {
        private Rectangle2D m_bounds = new Rectangle2D.Double();
        private Rectangle2D m_temp = new Rectangle2D.Double();
        private double m_d = 15;
        public void itemBoundsChanged(Display d) {
            d.getItemBounds(m_temp);
            GraphicsLib.expand(m_temp, 25/d.getScale());
            
            double dd = m_d/d.getScale();
            double xd = Math.abs(m_temp.getMinX()-m_bounds.getMinX());
            double yd = Math.abs(m_temp.getMinY()-m_bounds.getMinY());
            double wd = Math.abs(m_temp.getWidth()-m_bounds.getWidth());
            double hd = Math.abs(m_temp.getHeight()-m_bounds.getHeight());
            if ( xd>dd || yd>dd || wd>dd || hd>dd ) {
                m_bounds.setFrame(m_temp);
                DisplayLib.fitViewToBounds(d, m_bounds, 0);
            }
        }
    }
    
} // end of class GraphView