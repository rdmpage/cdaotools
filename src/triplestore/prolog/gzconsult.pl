%Procedure for consulting gzipped source files.
%Brandon Chisham
%April 1, 2010
gzconsult( File ):- gzopen( File, read, In ),
		    load_files( File, [stream( In ), format( source )]).
