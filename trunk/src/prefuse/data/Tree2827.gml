<?xml version="1.0" encoding="UTF-8"?>


<!-- NODE_QUERY: SELECT ?node WHERE { ?node <http://www.evolutionaryontology.org/cdao.owl#part_of> <http://www.cs.nmsu.edu/~epontell/S975A2691.tre#Tree2827>.  ?node rdf:type <http://www.evolutionaryontology.org/cdao.owl#Node> . } -->
<!-- EDGE_QUERY: SELECT ?edge ?src ?dest WHERE { ?dest <http://www.evolutionaryontology.org/cdao.owl#part_of> <http://www.cs.nmsu.edu/~epontell/S975A2691.tre#Tree2827>. ?src <http://www.evolutionaryontology.org/cdao.owl#part_of> <http://www.cs.nmsu.edu/~epontell/S975A2691.tre#Tree2827>. ?edge <http://www.evolutionaryontology.org/cdao.owl#has_Child_Node> ?dest . ?edge <http://www.evolutionaryontology.org/cdao.owl#has_Parent_Node> ?src. } -->

<graphml xmlns="http://graphml.graphdrawing.org/xmlns"  
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns
     http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd">
<graph id="http://www.cs.nmsu.edu/~epontell/S975A2691.tre#Tree2827" edgedefault="directional">
<node id="http://www.cs.nmsu.edu/~epontell/S975A2691.tre#Arytaina_adenocarpi"/>
<node id="http://www.cs.nmsu.edu/~epontell/S975A2691.tre#Arytaina_devia_G"/>
<node id="http://www.cs.nmsu.edu/~epontell/S975A2691.tre#Arytaina_devia_insularis_C"/>
<node id="http://www.cs.nmsu.edu/~epontell/S975A2691.tre#Arytaina_devia_insularis_P"/>
<edge id="http://www.cs.nmsu.edu/~epontell/S975A2691.tre#tree_Arytaina_adenocarpi" source="http://www.cs.nmsu.edu/~epontell/S975A2691.tre#tree" target="http://www.cs.nmsu.edu/~epontell/S975A2691.tre#Arytaina_adenocarpi"/>
<edge id="http://www.cs.nmsu.edu/~epontell/S975A2691.tre#node2_Arytaina_devia_G" source="http://www.cs.nmsu.edu/~epontell/S975A2691.tre#node2" target="http://www.cs.nmsu.edu/~epontell/S975A2691.tre#Arytaina_devia_G"/>
<edge id="http://www.cs.nmsu.edu/~epontell/S975A2691.tre#node1_Arytaina_devia_insularis_C" source="http://www.cs.nmsu.edu/~epontell/S975A2691.tre#node1" target="http://www.cs.nmsu.edu/~epontell/S975A2691.tre#Arytaina_devia_insularis_C"/>
<edge id="http://www.cs.nmsu.edu/~epontell/S975A2691.tre#node2_Arytaina_devia_insularis_P" source="http://www.cs.nmsu.edu/~epontell/S975A2691.tre#node2" target="http://www.cs.nmsu.edu/~epontell/S975A2691.tre#Arytaina_devia_insularis_P"/>
<edge id="http://www.cs.nmsu.edu/~epontell/S975A2691.tre#tree_node1" source="http://www.cs.nmsu.edu/~epontell/S975A2691.tre#tree" target="http://www.cs.nmsu.edu/~epontell/S975A2691.tre#node1"/>
<edge id="http://www.cs.nmsu.edu/~epontell/S975A2691.tre#node1_node2" source="http://www.cs.nmsu.edu/~epontell/S975A2691.tre#node1" target="http://www.cs.nmsu.edu/~epontell/S975A2691.tre#node2"/>
</graph>
</graphml>
