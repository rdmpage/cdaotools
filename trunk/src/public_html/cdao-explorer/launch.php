<?php header('Content-type: application/x-java-jnlp-file'); /*header "Content-type: text-plain"*/  ?>
<?php
$PHYLOWS_TREE_URI="http://www.cs.nmsu.edu/~cdaostore/cgi-bin/phylows/tree";
$TREE_NAME=$_GET['tree'];

if ($TREE_NAME){
   $TREE_URL = "$PHYLOWS_TREE_URI/$TREE_NAME?format=graphml";
   $ARGUMENT="<argument>$TREE_URL</argument>";
}


echo<<<END
<?xml version="1.0" encoding="UTF-8"?>
<jnlp codebase="http://www.cs.nmsu.edu/~bchisham/cdao-explorer/" href="launch.php?tree=$TREE_NAME" spec="1.0+">
    <information>
        <title>CDAO Explorer</title>
        <vendor>New Mexico State University</vendor>
        <homepage href="http://www.cs.nmsu.edu/~cdaostore/"/>
        <description>CDAO Explorer allows users to select and visualize data in the CDAO-store as well as add additional annotations to that data.</description>
        <description kind="short">CDAO Explorer</description>
    <icon href="cdao-splash.jpg" kind="splash"/>
<icon href="cdao-triplestore-logo.jpg" kind="default"/>
</information>
<security>
<all-permissions/>
</security>
    <resources>
<j2se version="1.5+"/>
<jar eager="true" href="CDAOExplorer.jar" main="true"/>
    <jar href="lib/swing-layout-1.0.4.jar"/>
<jar href="lib/appframework-1.0.3.jar"/>
<jar href="lib/swing-worker-1.1.jar"/>
<jar href="lib/pellet-cli.jar"/>
<jar href="lib/pellet-core.jar"/>
<jar href="lib/pellet-datatypes.jar"/>
<jar href="lib/pellet-dig.jar"/>
<jar href="lib/pellet-el.jar"/>
<jar href="lib/pellet-explanation.jar"/>
<jar href="lib/pellet-jena.jar"/>
<jar href="lib/pellet-modularity.jar"/>
<jar href="lib/pellet-owlapi.jar"/>
<jar href="lib/pellet-pellint.jar"/>
<jar href="lib/pellet-query.jar"/>
<jar href="lib/pellet-rules.jar"/>
<jar href="lib/pellet-test.jar"/>
<jar href="lib/servlet.jar"/>
<jar href="lib/aterm-java-1.6.jar"/>
<jar href="lib/owlapi-bin.jar"/>
<jar href="lib/xsdlib.jar"/>
<jar href="lib/relaxngDatatype.jar"/>
<jar href="lib/lucene-1.4.3.jar"/>
<jar href="lib/mail.jar"/>
<jar href="lib/smtp.jar"/>
<jar href="lib/mailapi.jar"/>
<jar href="lib/activation.jar"/>
<jar href="lib/jaxb-api.jar"/>
<jar href="lib/jaxb-impl.jar"/>
<jar href="lib/jaxb1-impl.jar"/>
<jar href="lib/jaxb-xjc.jar"/>
<jar href="lib/jsr173_api.jar"/>

</resources>
    <application-desc main-class="cdaoexplorer.Main">
       $ARGUMENT      
    </application-desc>
</jnlp>
END;
?>
