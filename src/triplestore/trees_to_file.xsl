<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns="">
        <xsl:output method="text" omit-xml-declaration="yes" indent="no"  />
        <xsl:strip-space elements="*"/> 
        <xsl:template match="file">
                <xsl:for-each select="tree">
                        <xsl:value-of select="../@id" />
                        <xsl:text> </xsl:text>
                        <xsl:value-of select="@id" />
                        <xsl:text>&#10;</xsl:text>
                </xsl:for-each>
        </xsl:template>
</xsl:stylesheet>
