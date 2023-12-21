#!/usr/bin/php
<html lang="en">
<body style="background-color: darkslateblue; color: lightgrey;">
<div style="display: flex; flex-direction: column; align-items: center; justify-content: center; height: 100%;">
	<h1>Passing form data to PHP-CGI</h1>

	<h2>Post mode</h2>
	<form action="./post.php" method="post" enctype = "application/x-www-form-urlencoded">
	 	<p>name: <input type="text" name="name" /></p>
	 	<p>email: <input type="text" name="email" /></p>
	 	<p><input type="submit" /></p>
	</form>
	
	 	<h2>Get mode</h2>
	 <form action="./post.php" method="get">
	 	<p>name: <input type="text" name="name" /></p>
	 	<p>email: <input type="text" name="email" /></p>
	 	<p><input type="submit" /></p>
	</form>

</div>
</body>
</html>