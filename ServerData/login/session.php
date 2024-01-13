#!./php-cgi/php-cgi
<?php
// Start the session
session_start();

// Handle GET request
if ($_SERVER["REQUEST_METHOD"] == "GET") {
    // Retrieve value from URL parameter (GET)
    $get_value = isset($_GET['value']) ? $_GET['value'] : 'Not set';
    
    // Display the received GET value
    echo "GET Value: $get_value";
}

// Handle POST request
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Retrieve value from form data (POST)
    $post_value = isset($_POST['value']) ? $_POST['value'] : 'Not set';
    
    // Display the received POST value
    echo "POST Value: $post_value";
}

// Handle cookies
if (!isset($_COOKIE['example_cookie'])) {
    // Set a cookie with a value
    setcookie('example_cookie', 'Hello, this is a cookie!', time() + 3600, "/");
    echo "<br>Cookie has been set!";
} else {
    // Display the value of the existing cookie
    echo "<br>Cookie Value: " . $_COOKIE['example_cookie'];
}

// Handle sessions
if (!isset($_SESSION['example_session'])) {
    // Set a session variable
    $_SESSION['example_session'] = 'Hello, this is a session!';
    echo "<br>Session variable has been set!";
} else {
    // Display the value of the existing session variable
    echo "<br>Session Value: " . $_SESSION['example_session'];
}
?>
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>PHP Page with GET, POST, Cookies, and Session</title>
</head>

<body>
    <h1>PHP Page with GET, POST, Cookies, and Session</h1>

    <!-- Form for sending POST request -->
    <form method="post" action="">
        <label for="post_value">Enter value for POST:</label>
        <input type="text" name="value" id="post_value">
        <input type="submit" value="Submit POST">
    </form>

    <!-- Link for sending GET request -->
    <a href="?value=Hello%20from%20GET">Send GET with value</a>
</body>

</html>
