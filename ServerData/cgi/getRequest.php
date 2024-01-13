#!./php-cgi/php-cgi
<html lang="en">
<head>
<style>
   * {
    margin: 0;
    padding: 0;
    margin-top: 10%;
    box-sizing: border-box;
    font-family: 'Courier New', Courier, monospace;
   }
   body {
        /* margin-top: 20%; */
         background-color: #f4f9f9;
         margin: 1.2rem 10%; /* Adjusted margin to center content */
   }
   h1 {
    text-align: center;
    margin: 1em 0;
   }
   h2 {
    margin: 1em 0;
   }
   p {
    margin: 2.5em;
    word-wrap: break-word;
   }
    .c {
     margin: 5%; /* Adjusted margin for better visibility */
     padding: 1em; /* Reduced padding */
     border-radius: 0.5em;
     box-shadow: 0 0 0.5em #ccc;
    }
    .m {
        background-color: #fff; /* Changed background color */
        padding: 1em;
        margin-top: 1em; /* Adjusted margin-top */
        border-radius: 0.5em;
        box-shadow: 0 0 0.5em #ccc; /* Added box shadow */
    }
</style>
</head>
<body>
<div class="c">
    <h1>GET Request</h1>
    <?php
    if ($_SERVER["REQUEST_METHOD"] == "GET") {
        // Retrieving form data
        $name = isset($_GET['name']) ? $_GET['name'] : '';
        $email = isset($_GET['email']) ? $_GET['email'] : '';
        $message = isset($_GET['message']) ? $_GET['message'] : '';

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
