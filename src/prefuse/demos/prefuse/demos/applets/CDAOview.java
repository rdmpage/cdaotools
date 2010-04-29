package prefuse.demos.applets;

import java.awt.Color;
import java.awt.Dimension;
import java.util.Iterator;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JComponent;
import javax.swing.JSplitPane;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import prefuse.Display;
import prefuse.Visualization;
import prefuse.action.ActionList;
import prefuse.action.RepaintAction;
import prefuse.action.assignment.ColorAction;
import prefuse.action.distortion.BifocalDistortion;
import prefuse.action.distortion.Distortion;
import prefuse.action.distortion.FisheyeDistortion;
import prefuse.action.filter.FisheyeTreeFilter;
import prefuse.action.filter.GraphDistanceFilter;
import prefuse.action.layout.graph.ForceDirectedLayout;
import prefuse.activity.Activity;
import prefuse.controls.AnchorUpdateControl;
import prefuse.controls.DragControl;
import prefuse.controls.FocusControl;
import prefuse.controls.NeighborHighlightControl;
import prefuse.controls.PanControl;
import prefuse.controls.WheelZoomControl;
import prefuse.controls.ZoomControl;
import prefuse.controls.ZoomToFitControl;
import prefuse.data.Graph;
import prefuse.data.Tuple;
import prefuse.data.event.TupleSetListener;
import prefuse.data.io.GraphMLReader;
import prefuse.data.tuple.TupleSet;
import prefuse.render.DefaultRendererFactory;
import prefuse.render.LabelRenderer;
import prefuse.util.ColorLib;
import prefuse.util.GraphLib;
import prefuse.util.PrefuseLib;
import prefuse.util.force.ForceSimulator;
import prefuse.util.ui.JForcePanel;
import prefuse.util.ui.JPrefuseApplet;
import prefuse.util.ui.JValueSlider;
import prefuse.util.ui.UILib;
import prefuse.visual.EdgeItem;
import prefuse.visual.NodeItem;
import prefuse.visual.VisualGraph;
import prefuse.visual.VisualItem;

/**
 * @author <a href="http://jheer.org">jeffrey heer</a>
 */
public class CDAOview extends JPrefuseApplet {

    private static final String graph = "graph";
    private static final String nodes = "graph.nodes";
    private static final String edges = "graph.edges";
    
    public void init() {
        UILib.setPlatformLookAndFeel();
        JComponent graphview = demo("/Tree3099.graphml", "IdLabel");
        this.getContentPane().add(graphview);
    }

    public static JComponent demo(String datafile, String label) {
        Graph g = null;
        if ( datafile == null ) {
            g = GraphLib.getGrid(15,15);
        } else {
            try {
                g = new GraphMLReader().readGraph(datafile);
            } catch ( Exception e ) {
                e.printStackTrace();
                System.exit(1);
            }
        }
        return demo(g, label);
    }
    
    public static JComponent demo(Graph g, String label) {

        // create a new, empty visualization for our data
        final Visualization vis = new Visualization();
        VisualGraph vg = vis.addGraph(graph, g);
        //vis.setValue(edges, null, VisualItem.INTERACTIVE, Boolean.FALSE);
        
        System.out.println("Visualization.FOCUS_ITEMS="+ Visualization.FOCUS_ITEMS);
        
        /*for(int i =0; i < vg.getNodeCount(); i++)
        {
        	System.out.print("Node["+i+"]: ");
        	for(int j=0; j < vg.getNode(i).getColumnCount(); j++)
        	{
        		System.out.print(" colName["+j+"]:"+ vg.getNode(i).getColumnName(j)+"|");
        	}
        	System.out.println();
        }*/
        
        
        
        TupleSet focusGroup = vis.getGroup(Visualization.FOCUS_ITEMS); 
        focusGroup.addTupleSetListener(new TupleSetListener() {
            public void tupleSetChanged(TupleSet ts, Tuple[] add, Tuple[] rem)
            {
                for ( int i=0; i<rem.length; ++i )
                    ((VisualItem)rem[i]).setFixed(false);
                for ( int i=0; i<add.length; ++i ) {
                    ((VisualItem)add[i]).setFixed(false);
                    //((VisualItem)add[i]).setFixed(true);
                    
                    if(add[i] instanceof EdgeItem)
                    {
                    	System.out.println("EDGES DONE IN tupleSetChanged");
                    }
                    else if(add[i] instanceof NodeItem)
                    {
                    	System.out.println("NODES DONE IN tupleSetChanged");
                    }
                    else
                    {
                    	System.out.println("I DONT KNOW WHATS DONE IN tupleSetChanged");	
                    }
                    
                }
                
                vis.run("draw");
            }
        });
        
        // set up the renderers
        LabelRenderer tr = new LabelRenderer(label);
        tr.setRoundedCorner(8, 8);
        //CDAOEdgeRenderer er = new CDAOEdgeRenderer(label);
        //er.setRoundedCorner(8, 8);
        vis.setRendererFactory(new DefaultRendererFactory(tr));
        
        
       
        
        // -- set up the actions ----------------------------------------------
        
        int maxhops = 99, hops = 50;
        final GraphDistanceFilter filter = new GraphDistanceFilter(graph, hops);

        ActionList draw = new ActionList();
        //draw.add(filter);
        draw.add(new ColorAction(nodes, VisualItem.FILLCOLOR, ColorLib.rgb(200,200,255)));
        draw.add(new ColorAction(nodes, VisualItem.STROKECOLOR, 0));
        draw.add(new ColorAction(nodes, VisualItem.TEXTCOLOR, ColorLib.rgb(0,0,0)));
        draw.add(new ColorAction(edges, VisualItem.FILLCOLOR, ColorLib.rgb(200,200,255)));
        draw.add(new ColorAction(edges, VisualItem.STROKECOLOR, ColorLib.rgb(150, 150, 205)));
        draw.add(new ColorAction(edges, VisualItem.TEXTCOLOR, ColorLib.rgb(0,0,0)));
        
        ColorAction nfill = new ColorAction(nodes, 
                VisualItem.FILLCOLOR, ColorLib.rgb(200,200,255));
        nfill.add("_fixed", ColorLib.rgb(255,100,100));
        nfill.add("_highlight", ColorLib.rgb(255,200,125));
        
        ColorAction efill = new ColorAction(edges, 
                VisualItem.FILLCOLOR, ColorLib.rgb(255,200,200));
        efill.add("_fixed", ColorLib.rgb(100,100,255));
        efill.add("_highlight", ColorLib.rgb(125,200,255));
        
        ForceDirectedLayout fdl = new ForceDirectedLayout(graph,false,false);
        ForceSimulator fsim = fdl.getForceSimulator();
        fsim.getForces()[0].setParameter(0, -1.2f);
        
        //draw.add(fdl);
        //draw.add(new RepaintAction());
        
        //ADDING FISHEYE DISTORTION (taken from FisheyeMenu demo)
        //ActionList distort = new ActionList();
        
        Distortion feye = new BifocalDistortion(0.05,5.0);
        
        //FisheyeTreeFilter feye = new FisheyeTreeFilter(graph, 5);
        
        ActionList animate = new ActionList(Activity.INFINITY);
        animate.add(fdl);
        animate.add(efill);
        animate.add(nfill);
        //animate.add(feye);
        animate.add(new RepaintAction()); 
        
        // finally, we register our ActionList with the Visualization.
        // we can later execute our Actions by invoking a method on our
        // Visualization, using the name we've chosen below.
        vis.putAction("draw", draw);
        vis.putAction("layout", animate);
        vis.runAfter("draw", "layout");
        
 
        
        
        // --------------------------------------------------------------------
        // STEP 4: set up a display to show the visualization
        
        Display display = new Display(vis);
        display.setSize(500,500);
        display.setForeground(Color.GRAY);
        display.setBackground(Color.WHITE);
        
        // main display controls
        display.addControlListener(new FocusControl(1));
        display.addControlListener(new DragControl());
        display.addControlListener(new PanControl());
        display.addControlListener(new ZoomControl());
        display.addControlListener(new WheelZoomControl());
        display.addControlListener(new ZoomToFitControl());
        display.addControlListener(new NeighborHighlightControl());
        display.addControlListener(new AnchorUpdateControl(feye, "layout"));
        
        //display.addControlListener(new );
        
        display.setForeground(Color.GRAY);
        display.setBackground(Color.WHITE);
        
        // --------------------------------------------------------------------        
        // STEP 5: launching the visualization
        
        // create a panel for editing force values
        //final JForcePanel fpanel = new JForcePanel(fsim);
        
        final JValueSlider slider = new JValueSlider("Distance", 0, maxhops, hops);
        slider.addChangeListener(new ChangeListener() {
            public void stateChanged(ChangeEvent e) {
                filter.setDistance(slider.getValue().intValue());
                vis.run("draw");
            }
        });
        slider.setBackground(Color.WHITE);
        slider.setPreferredSize(new Dimension(300,30));
        slider.setMaximumSize(new Dimension(300,30));
        
        Box cf = new Box(BoxLayout.Y_AXIS);
        cf.add(slider);
        cf.setBorder(BorderFactory.createTitledBorder("Connectivity Filter"));
        //fpanel.add(cf);
        
        //fpanel.add(Box.createVerticalGlue());
        
        // create a new JSplitPane to present the interface
        JSplitPane split = new JSplitPane();
        split.setLeftComponent(display);
        //split.setRightComponent(fpanel);
        split.setOneTouchExpandable(true);
        split.setContinuousLayout(false);
        split.setDividerLocation(530);
        split.setDividerLocation(800);
        
        
        // position and fix the default focus node
        /*for(int i = 0; i < vg.getNodeCount(); i++)
        {
        	VisualItem focus = (VisualItem)vg.getNode(i);
        	focusGroup.addTuple(focus);
        }
        for(int i = 0; i < vg.getEdgeCount(); i++)
        {
        	VisualItem focus = (VisualItem)vg.getEdge(i);
        	focusGroup.addTuple(focus);
        }*/
        
        VisualItem focus = (VisualItem)vg.getNode(0);
        PrefuseLib.setX(focus, null, 400);
        PrefuseLib.setY(focus, null, 250);
        focusGroup.setTuple(focus);
	    
        // now we run our action list and return
        return display;
    }
    
} // end of class GraphView
