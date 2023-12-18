#!/usr/bin/php
<html lang="en">
<body style="background-color: darkslateblue; color: lightgrey;">
<div style="display: flex; flex-direction: column; align-items: center; justify-content: center; height: 100%;">
	<h1>Passing form data to PHP-CGI</h1>


<?php
	if(isset($_GET["s1"]) || isset($_GET["s2"]))
	{
		echo "Using the GET method to pass data<br/>";
		echo "s1 is ".$_GET["s1"].".<br/>";
		echo "s2 is ".$_GET["s2"].".<br/>";
	}
	else if(isset($_POST["s1"]) || isset($_POST["s2"]))
	{
		echo "Using the POST method to pass data<br/>";
		echo "s1 is ".$_POST["s1"].".<br/>";
		echo "s2 is ".$_POST["s2"].".<br/>";
	}
?>

	<h2>Post mode</h2>
<form action="./form.php" method="post">
 	<p>s1: <input type="text" name="s1" /></p>
 	<p>s2: <input type="text" name="s2" /></p>
 	<p><input type="submit" /></p>
</form>

 	<h2>Get mode</h2>
 <form action="./form.php" method="get">
 	<p>s1: <input type="text" name="s1" /></p>
 	<p>s2: <input type="text" name="s2" /></p>
 	<p><input type="submit" /></p>
 </form>




</div>
</body>
</html>