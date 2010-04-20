%Brandon Chisham
%March 25, 2010
%Script including methods for doing count oriented queries.
%example tree

:- use_module(library( lists ) ).

%:- module( tree_count, [leaf_count/2, internal_count/2, node_count/2 ] ).

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

leaf( Tree, Node ):- node( Tree, Node ), \+ parent_of( Tree, Node, Node2 ).
root( Tree, Node ):- node( Tree, Node ), \+ parent_of( Tree, Node2, Node ).

internal_node( Tree, Node ):- node( Tree, Node ), \+ leaf( Tree, Node ).
%set the root depth.
depth( Tree, Node, 0 ):- root( Tree, Node ).
%define rule for recursibly computing the depth.
depth( Tree, Node, Depth ):- node( Tree, Node ), 
                       	     node( Tree, Node2 ), 
		             edge( Tree, T, Edge ,Node2, Node ), 
		             depth( Tree, Node2, Depth2 ), 
		       succ( Depth2, Depth ).

max_depth( Tree, Node, 0 ):- root( Tree, Node ).

%max_depth( Node, Depth ):- root( Node ), leaf( Node1 ), leaf( Node2 ), depth( Node1, Depth ), depth( Node2, Depth2 ), Depth > Depth2. 

max_depth( Tree, Node, Depth ):- root( Tree, Anst ),
                           nearest_common_ancestor_of( Tree, Anst, Node, Node2 ),
                           leaf( Tree, Node ), 
                           leaf( Tree, Node2 ),
	 		   depth( Tree, Node, Depth ), 
			   depth( Tree, Node2, Depth2 ), 
			   Depth > Depth2.
%count( Node, 1 ):- root( Node ).
%Count the number of nodes in a tree
node_count( Tree, Count ):- root( Tree, Node ), node_count( Tree, Node, Count ).
node_count( Tree, Node, Count ):- setof( Node, node( Tree, Node ), Nodes ), length( Nodes, Count ).
%Count the leaves in a tree.
leaf_count( Tree, Count ):- root( Tree, Node ), leaf_count( Tree, Node, Count ).
leaf_count( Tree, Node, Count ):- setof( Node, leaf( Tree, Node ), Nodes ), length( Nodes, Count ).
%Count the internal nodes in a tree.
internal_count( Tree, Count ):- root( Tree, Node ), internal_count( Tree, Node, Count ).
internal_count( Tree, Node, Count ):- setof( Node, internal_node( Tree,  Node ), Nodes ), length( Nodes, Count ).
%Finds descendents of a particular node. 
clade( Tree, Node, Member ):- ancestor_of( Tree, Node, Member ).
clade( Tree, Node, Member ):- node( Tree, Node ), Member = Node.
clade( Tree, Node ):- setof( Member, clade( Tree, Node, Member ), Members ), write( Members ).

pathlength( Tree, Start, End, Len ):- root(Tree, Start ), depth( Tree, End, Len ).

pathlength( Tree, Start, End, Len ):- nearest_common_ancestor_of( Tree, Nca, Start, End ), depth( Tree, Nca, Nlen ), depth( Tree, Start, Slen ), depth( Tree, End, Elen ), plus( Slen, Elen, Tlen ), plus( Len, Nlen, Tlen ), \+ancestor_of( Tree, Start, End  ).

pathlength( Tree, Start, End, Len ):- ancestor_of( Tree, Start, End ), depth( Tree, Start, Slen ), depth( Tree, End, Elen ), plus( Len, Slen, Elen ).

distance( Tree, Lhs, Rhs, Len ):- nearest_common_ancestor_of( Tree, Nca, Lhs, Rhs ), pathlength( Tree, Lhs, Nca, LNlen ), pathlength( Tree, Rhs, Nca, RNlen ), plus( LNlen, RNlen, Len ).

closer( N1, N2, N3 ):- distance( N1, N3, D13 ), distance( N2, N3, D23 ), D13 < D23.

%Maximum distance between Start and some other vertex.
eccentricity( Tree, Len, Start ):- findall( E, pathlength(Tree, Start, N2, E), Lens  ), max_list( Lens, Len ).

%Radius = minimum eccentricity of any vertex.
radius( Tree, R ):- findall( E, eccentricity(Tree, E, Start), Es  ), min_list(Es, R).

%Diameter = maximum eccentricity of any vertex.
diameter( Tree, D):- findall( E, eccentricity(Tree, E, Start), Es ), max_list(Es, D ).
