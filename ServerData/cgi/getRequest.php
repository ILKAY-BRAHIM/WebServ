#!./ServerData/cgi/php-cgi
<html lang="en">
<head>
<style>
   * {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
    font-family: 'Courier New', Courier, monospace;
    /* background-color: #f4f4f4; */
   }
   body {
         background-color: #d7d3d3;
         margin: 1.2rem 20%;
   }
   h1 {
    text-align: center;
    /* margin: 1.5em; */
   }
   h2 {
    margin: 1em 0;
   }
   p {
    margin: 0.5em;
    word-wrap: break-word;

   }
    .c {
     margin : 15%;
     padding: 3em;
     border-radius: 0.5em;
     box-shadow: 0 0 0.5em #ccc;
    }
    .m {
        background-color: #cdc8c8;
        padding : 1em;
        margin-top : 2em;
         border-radius: 0.5em;

    }
</style>
</head>
<body >
<div class="c">
	<h1>Test GET Request</h1>
    <?php
    if ($_SERVER["REQUEST_METHOD"] == "GET") {
        // Retrieving form data
        $name = isset($_GET['name']) ? $_GET['name'] : '';
        $email = isset($_GET['email']) ? $_GET['email'] : '';
        $message = isset($_GET['message']) ? $_GET['message'] : '';

        // You can perform validation or additional processing here

        // Displaying the submitted data
        echo "<h2>Submitted Data:</h2>";
        echo "<div class=\"m\">";
        echo "<p>Name: " . htmlspecialchars($name) . "</p>";
        echo "<p>Email: " . htmlspecialchars($email) . "</p>";
        echo "<p>Message: " . htmlspecialchars($message) . "</p>";
        echo "</div>";
    } else {
        // If the form is not submitted via GET method, you can redirect or handle it accordingly
        echo "<div class='c1'>";
        echo "<p>Form not submitted!</p>";
        echo "</div>";
    }
    ?>
</div>
</body>
</html>