<html><head><title>CDAO</title></head>
<body>
<?php
	//echo "Upload: " . $_FILES["inputfile"]["name"] . "<br />";
	//echo "Type: " . $_FILES["inputfile"]["type"] . "<br />";
	//echo "Size: " . ($_FILES["inputfile"]["size"] / 1024) . " Kb<br />";
	//echo "Stored in: " . $_FILES["inputfile"]["tmp_name"];
	//echo $_SERVER['REQUEST_TIME']."<br/>";
	//echo $_SERVER['REMOTE_ADDR']."<br/>";
	$upload_time=$_SERVER['REQUEST_TIME'];
	$upload_IP=str_replace(".","_",$_SERVER['REMOTE_ADDR']);
	
	echo $inputfile."<br/>";
	switch($_POST["format"]){
		case "M":
			$inputfile =substr($_FILES["inputfile"]["name"],0,-4);
			$inputfile=$inputfile."_".$upload_time."_".$upload_IP.".meg";
      			move_uploaded_file($_FILES["inputfile"]["tmp_name"],"upload/" .$inputfile);
			$command="cgi-bin/main upload/" . $inputfile;
			//echo "<br>".$command;
			exec($command);
			unlink("upload/".$inputfile);
			$outputfile=substr($inputfile,0,-3)."owl";
			//echo $outputfile."<br/>";
			echo "<a href=\"http://www.cs.nmsu.edu/~cdaostore/cgi-bin/import?ontology=http://www.cs.nmsu.edu/~tle/CDAO/upload/".$outputfile."\">Import to the triple-store</a><br/>";
			echo "<a href=\"http://www.cs.nmsu.edu/~tle/CDAO/upload/$outputfile\">Download Translated File</a>";
			//echo "<br>".$_FILES['inputfile']['tmp_name'];
			break;
		case "P":
			$inputfile =substr($_FILES["inputfile"]["name"],0,-4);
			$inputfile=$inputfile."_".$upload_time."_".$upload_IP;
			move_uploaded_file($_FILES["inputfile"]["tmp_name"],"upload/" . $inputfile);
			$command="cgi-bin/parser.out upload/". $inputfile;
			$outputfile=$inputfile."_CDAO.owl";
			echo "<a href=\"http://www.cs.nmsu.edu/~cdaostore/cgi-bin/import?ontology=http://www.cs.nmsu.edu/~tle/CDAO/upload/".$outputfile."\">Import to the triple-store</a><br/>";
			echo "<a href=\"http://www.cs.nmsu.edu/~tle/CDAO/upload/$outputfile\">Download Translated File</a>";
			exec($command);
			unlink("upload/" . $inputfile);
			break;
		case "N":
			$inputfile =$_FILES["inputfile"]["name"]."_".$upload_time."_".$upload_IP;
			move_uploaded_file($_FILES["inputfile"]["tmp_name"],"upload/" . $inputfile);
			$command="cgi-bin/cdao-import.sh upload/". $inputfile;
			echo "<a href=\"http://www.cs.nmsu.edu/~bchisham/cgi-bin/import?ontology=http://www.cs.nmsu.edu/~tle/CDAO/upload/$inputfile.cdao\">Import to the triple-store</a><br/>";
			echo "<a href=\"http://www.cs.nmsu.edu/~tle/CDAO/upload/$inputfile.cdao\">Download Translated File</a>";
			//echo "<br>".$command;
			$output=system($command,$return);
			unlink("upload/" .$inputfile);
			break;
	}
?>
</body>
</html>
