%Procedure for consulting gzipped source files.

gzconsult( File ):- gzopen( File, read, In ),
		    load_files( File, [stream( In ), format( source )]).
