%Common Ancestor Script
%Brandon Chisham
%Feb. 18, 2010

%Parent can directly be concluded from the edge relationships
parent_of( Parent, Child ):- edge( TreeNmae, 'directional', EdgeName, Parent, Child).
%The parent is also an ancestor.
ancestor_of( Parent, Child ):- parent_of( Parent, Child ).
%The ancestor is a parent of an ancestor of a descendant.
ancestor_of( Ancestor, Descendant ):- parent_of( Ancestor, Intermediate), ancestor_of(Intermediate, Descendant).
%Ancestor is a common ancestor if it is an ancestor for each child.
common_ancestor_of( Ancestor, N1, N2, N3 ):- ancestor_of( Ancestor, N1 ), ancestor_of( Ancestor, N2 ), ancestor_of(Ancestor, N3 ).
%An ancestor is distant if it is the ancestor of some node that is the common ancestor of N1, N2, and N3.
distant_common_ancestor_of( DistantAncestor, N1, N2, N3 ):- common_ancestor_of( Ancestor, N1, N2, N3 ), ancestor_of( DistantAncestor, Ancestor ).
%An ancester is the nearest common ancestor if it is the common ancestor and not a distant common ancestor.
nearest_common_ancestor_of( Nca, N1, N2, N3 ):- common_ancestor_of( Nca, N1, N2, N3 ), \+ distant_common_ancestor_of( Nca, N1, N2, N3 ).