<?php
if($_POST['formSubmit'] == "Submit")
{
	$errorMessage = "";
	
	if(empty($_POST['timeField1']))
	{
		$errorMessage .= "<li>time format not valid!</li>";
	}
	if(empty($_POST['timeField2']))
	{
		$errorMessage .= "<li>time format not valid!</li>";
	}
	// if(empty($_POST['formName']))
	// {
	// 	$errorMessage .= "<li>You forgot to enter a name!</li>";
	// }
	
	$varTime = $_POST['timeField1'].$_POST['timeField2'];
	//echo gettype($varTime);
	//$varTime +=$_POST['timeField2'];
	//$varName = $_POST['formName'];

	$command = "python contactArduino.py -t $varTime";
	//exec($command, $output= array());
	
	//print $output;
	$last_line = system($command, $retval);


	if(empty($errorMessage)) 
	{
		// $fs = fopen("mydata.csv","a");
		// fwrite($fs,$varName . ", " . $varTime . "\n");
		// fclose($fs);
		
		header("Location: thank-you.html");
		exit;
	}
}
?>
