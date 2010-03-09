<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl= "http://www.w3.org/1999/XSL/Transform"
                xmlns:html="http://www.w3.org/1999/xhtml"
                xmlns="">
   <xsl:output method="xml" encoding="utf-8" omit-xml-declaration="yes"/>
   
   <xsl:template match="/">
     <!--<html:p>Target: <xsl:value-of select="$target"/></html:p>-->
     <xsl:for-each select="./*/tu[ @id = $target ]/tree">
         <a>
           <xsl:attribute name="href">../tree/html?tree=<xsl:value-of select="@name"/></xsl:attribute>
           <xsl:value-of select="./@name"/>
         </a>
         <br/>
     </xsl:for-each>
     <!--<xsl:apply-templates select="./*"/>-->
   </xsl:template>
</xsl:stylesheet>
