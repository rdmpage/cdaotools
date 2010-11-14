/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package phylipalrequin.model;

/**
 *
 * @author bchisham
 */
public interface MatrixDataModel extends DataModel {

    int getNTax();
    int getNChar();

    String getTaxonName( int taxonNumber );
    
    char getCharacterState( int taxonNumber, int charNumber );

}
