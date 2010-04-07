<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl= "http://www.w3.org/1999/XSL/Transform"
                xmlns:nex="http://www.nexml.org/1.0" 
		xmlns="http://www.nexml.org/1.0" 
		xmlns:owl="http://www.w3.org/2002/07/owl#" 
		xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" 
		xmlns:skos="http://www.w3.org/2004/02/skos/core#" 
		xmlns:tb="http://treebase.org/terms#" 
		xmlns:xsd="http://www.w3.org/2001/XMLSchema#" 
		xmlns:xsi="http://www.w3.org/1999/XMLSchema-instance" 
		xml:base="http://purl.org/phylo/treebase/phylows/">
   <xsl:output method="text" encoding="utf-8" omit-xml-declaration="yes" />
   <xsl:template match="nex:row">
     <xsl:value-of select="./@id"/>,<xsl:value-of select="nex:seq/text()"/><xsl:text>&#10;</xsl:text>
   </xsl:template>
   <!--<xsl:template match="/">
      <xsl:apply-templates match="descendents::*"/>
   </xsl:template>-->
   <!--<xsl:template match="*" priority="-1"/>-->

</xsl:stylesheet>
