<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl= "http://www.w3.org/1999/XSL/Transform"
                xmlns:html="http://www.w3.org/1999/xhtml"
                xmlns="">
   <xsl:output method="xml" encoding="utf-8" omit-xml-declaration="yes"/>
   
   <xsl:template match="/">
     <!--<html:p>Target: <xsl:value-of select="$target"/></html:p>-->
     <xsl:for-each select="./*/tu[ @id = $target ]/tree">
         <xsl:value-of select="@name"/>
         <a>
           <xsl:attribute name="href">../tree/query?format=html&amp;tree=<xsl:value-of select="@name"/></xsl:attribute>
           Query
         </a>
         <a>
           <xsl:attribute  name="href">../../cdao-explorer/launch.php?tree=<xsl:value-of select="@name"/></xsl:attribute>
           View
         </a>
         <br/>
     </xsl:for-each>
     <!--<xsl:apply-templates select="./*"/>-->
   </xsl:template>
</xsl:stylesheet>
