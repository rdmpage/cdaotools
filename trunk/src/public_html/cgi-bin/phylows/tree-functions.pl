%Brandon Chisham
%March 25, 2010
%Script including methods for doing count oriented queries.
%example tree

:- use_module(library( lists ) ).
%:- include('gzconsult').
%gzconsult( File ):- gzopen( File, read, In ), load_files( File, [stream(In), format( source )]).
%:- gzconsult( 'prolog-trees.pl.gz').

:- include('Tree3099').

%node( 'foo-tree', 'a' ).
%node( 'foo-tree', 'b' ).
%node( 'foo-tree', 'c' ).
%node( 'foo-tree', 'd' ).
%node( 'foo-tree', 'e' ).
%node( 'foo-tree', 'f' ).
%edge( 'foo-tree', 'e1', 'directional' , 'a', 'b' ).
%edge( 'foo-tree', 'e2', 'directional' , 'a', 'c' ).
%edge( 'foo-tree', 'e3', 'directional' ,'c', 'd' ).
%edge( 'foo-tree', 'e4', 'directional', 'c', 'e' ).
%edge( 'foo-tree', 'e5', 'directional', 'd', 'f' ).

%Tree Structure.

leaf( Tree, Node ):- node( Tree, Node ), not(edge(Tree, _, _, Node, _)).
root( Tree, Node ):- node( Tree, Node ), not(edge(Tree,_,_, _,Node )).
internal_node( Tree, Node ):- node( Tree, Node ), not(leaf( Tree, Node)).

%set the root depth.
depth( Tree, Node, 0 ):- root( Tree, Node ).
%define rule for recursibly computing the depth.
depth( Tree, Node, Depth ):- node( Tree, Node ),
                       	     node( Tree, Node2 ),
		             edge( Tree, _, _ ,Node2, Node ),
		             depth( Tree, Node2, Depth2 ),
			     succ( Depth2, Depth ).

max_depth( Tree, Node, 0 ):- root( Tree, Node ).

%max_depth( Node, Depth ):- root( Node ), leaf( Node1 ), leaf( Node2 ), depth( Node1, Depth ), depth( Node2, Depth2 ), Depth > Depth2.

max_depth( Tree, Node, Depth ):- root( Tree, Anst ),
                           nearest_common_ancestor_of( Tree, Anst, [Node, Node2] ),
                           leaf( Tree, Node ),
                           leaf( Tree, Node2 ),
	 		   depth( Tree, Node, Depth ),
			   depth( Tree, Node2, Depth2 ),
			   Depth > Depth2.
%count( Node, 1 ):- root( Node ).
%Count the number of nodes in a tree
node_count( Tree, Count ):- root( Tree, RootNode ), node_count( Tree, RootNode, Count ).
node_count( Tree, StartNode, Count ):- setof( Node, descendent_of(Tree, Node, StartNode), Nodes ), length( Nodes, Count ).
%Count the leaves in a tree.
leaf_count( Tree, Count ):- leaf_count( Tree, _, Count ).
leaf_count( Tree, _, Count ):- setof( LNode, leaf( Tree, LNode ), Nodes ), length( Nodes, Count ).
%Count the internal nodes in a tree.
internal_count( Tree, Count ):- internal_count( Tree, _, Count ).
internal_count( Tree, _, Count ):- setof( INode, internal_node(Tree,INode), Nodes ), length( Nodes, Count ).
%Finds descendents of a particular node.
clade( Tree, Node, Member ):- ancestor_of( Tree, Node, Member ).
clade( Tree, Node, Node):- node( Tree, Node ).
clade( Tree, Node ):- setof( Member, clade( Tree, Node, Member ), Members ), write( Members ).

%pathlenth finds the length of a path from Node Start to Node End in Tree
% if the Node starts at the Root, just find the depth of Node End...
pathlength( _, Node, Node, 0).
pathlength( Tree, Start, End, Len ):- root(Tree, Start ), depth( Tree, End, Len ).
pathlength( Tree, Start, End, Len ):- root(Tree, End   ), depth( Tree, Start, 0 ).

pathlength( Tree, Start, End, Len ):- ancestor_of( Tree, Start, End ),
	                              depth( Tree, Start, Slen ),
				      depth( Tree, End, Elen ),
				      plus( Len, Slen, Elen ).

pathlength( Tree, Start, End, Len ):- nearest_common_ancestor_of( Tree, Nca, [Start, End]),
	                              depth( Tree, Nca, Nlen ),
				      depth( Tree, Start, Slen ),
				      depth( Tree, End, Elen ),
				      plus( NSlen, Nlen, Slen ),
				      plus( NElen, Nlen, Elen),
				      plus( NElen, NSlen, Len).

%Maximum distance between Start and some other vertex.

eccentricity(Tree,Len,Start ):- findall( E, (not(LeafNode == Start),
					leaf(Tree, LeafNode), 
					pathlength(Tree,Start,LeafNode, E)), 
					Lens),
				max_list(Lens, Len).
%Radius = minimum eccentricity of any vertex.
radius( Tree, R ):- findall(Leaf, leaf(Tree, Leaf), Leaves),
                    length(Leaves, Num), 
		    write('Number of Leaves for this tree is:'+ Num+'\n'),
	       	      radii(Tree, Leaves, R, 9999).
radii(Tree, [Leaf | Leaves], Radius, Curr) :- 
					      eccList(Tree,Leaf,Leaves,Lens),
					      max_list(Lens,Len),
					      length(Leaves, Num),
					      write(Num+' leaves left to check...\n'),
					      Len < Curr -> radii(Tree,Leaves,Radius,Len);
					                    radii(Tree,Leaves,Radius,Curr).
eccList(Tree, Leaf, [LeafNode | Leaves], [E | Rest]):- pathlength(Tree,Leaf,LeafNode,E),
						  %write('pathlength('+Tree+','+Leaf+','+LeafNode+','+E+') just finished.\n'),    
                                                       eccList(Tree,Leaf,Leaves,Rest).
eccList(_,_,[],[]).

%findall( E, eccentricity(Tree, E, Leaf), Es  ),
%	                           		          min_list(Es, LCurr),
%					          LCurr < Curr, radii(Tree, Leaves, Radius, LCurr);
%						  write(length(Leaves)+' leaves left to check...\n'),
%						  flush_output, 
%					          radii(Tree, Leaves, Radius, Curr).

radii( _, [], R, R).
radius_count( Tree, _, R ):- radius(Tree, R).
%Diameter = maximum eccentricity of any vertex.
diameter( Tree, D):- findall( E, (leaf(Tree, LeafNode), eccentricity(Tree, E, LeafNode)), Es ), 
		         max_list(Es, D ).
diameter_count(Tree, _, D):- diameter( Tree, D ).



%Parent can directly be concluded from the edge relationships
parent_of( TreeName, Parent, Child ):- edge( TreeName, 'directional', _, Parent, Child).
%The parent is an ancestor of the child.
ancestor_of( TreeName, Parent, Child ):- parent_of( TreeName, Parent, Child ).
%The ancestor is a parent of an ancestor of a descendant.
ancestor_of( TreeName, Ancestor, Descendent ):- parent_of( TreeName, Ancestor, Intermediate), ancestor_of( TreeName, Intermediate, Descendent).

child_of( TreeName, Child, Parent ):- parent_of( TreeName, Parent, Child ).
descendent_of( TreeName, Descendent, Ancestor ):- child_of( TreeName, Descendent, Intermediate ), descendent_of( TreeName, Intermediate, Ancestor ).
%These will get auto generated by the web script depending on what arity they should be.
%Ancestor is a common ancestor if it is an ancestor for each child.
%common_ancestor_of( TreeName, Ancestor, N1, N2, N3 ):- ancestor_of( TreeName,Ancestor, N1 ), ancestor_of( TreeName, Ancestor, N2 ), ancestor_of(TreeName, Ancestor, N3 ).
%An ancestor is distant if it is the ancestor of some node that is the common ancestor of N1, N2, and N3.
%distant_common_ancestor_of( TreeName, DistantAncestor, N1, N2, N3 ):- common_ancestor_of( TreeName, Ancestor, N1, N2, N3 ), ancestor_of( TreeName, DistantAncestor, Ancestor ).
%An ancester is the nearest common ancestor if it is the common ancestor and not a distant common ancestor.
%nearest_common_ancestor_of(TreeName, Nca, N1, N2, N3 ):- common_ancestor_of( TreeName, Nca, N1, N2, N3 ), \+ distant_common_ancestor_of( TreeName, Nca, N1, N2, N3 ).

common_ancestor_of(Tree, Ancestor, [Node | Nodes]) :- ancestor_of(Tree, Ancestor, Node), common_ancestor_of(Tree, Ancestor, Nodes).
common_ancestor_of(Tree, Ancestor, [Node | []]) :- ancestor_of(Tree, Ancestor, Node).

distant_common_ancestor_of(Tree, DistantAncestor, Nodes) :- common_ancestor_of(Tree, Ancestor, Nodes), ancestor_of(Tree, DistantAncestor, Ancestor).
nearest_common_ancestor_of(Tree, Nca, Nodes) :- common_ancestor_of(Tree, Nca, Nodes), not(distant_common_ancestor_of(Tree, Nca, Nodes)).



