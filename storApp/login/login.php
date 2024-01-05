#!./ServerData/cgi/php-cgi
<?php
// Ensure no output before session_start() or header() functions
session_start();

// Check if the theme preference is set in the cookie, otherwise set default
$theme = isset($_COOKIE['theme']) ? $_COOKIE['theme'] : 'light';

if(isset($_SESSION['username'])) {
    echo "Hello, ".$_SESSION['username']."!";
} else {
    if(isset($_POST['submit'])) {
        $username = $_POST['username'];
        // You can perform validation here before creating the session

        // Set the theme cookie
        if(isset($_POST['theme'])) {
            $theme = $_POST['theme'];
            setcookie('theme', $theme, time() + (86400 * 30), "/"); // Cookie set for 30 days
        }

        // Setting the session
        $_SESSION['username'] = $username;
        echo "Hello, $username!"; // Greeting the user after creating the session

        ?>
        <h1><a href="../welcome.php">Go to welcome page</a></h1>
        <?php
        // echo "<br><a href='login.php'>Go to welcome page</a>";
    } else {
        // Display form to get username and theme preference
        ?>
        <!DOCTYPE html>
<html>
<head>
    <title>Styled Form</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #d7d3d3;
            padding: 20px;
            margin-top : 10%;
        }

        .container {
          display: flex;
          justify-content: space-between;
        }

        form {
            max-width: 600px;
            width: 40%;
            margin: 0 auto;
            background-color: #e3e1e1;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
        }

        label {
            display: block;
            margin-bottom: 5px;
        }

        input[type="text"],
        input[type="email"],
        select,
        input[type="file"] {
            width: 80%;
            background-color: #fff;
            padding: 8px;
            margin-bottom: 15px;
            border: 1px solid #756b6b;
            border-radius: 4px;
        }

        input[type="submit"] {
            background-color: #2a342c;
            color: #d7c4c4;
            padding: 10px 20px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            margin-top: 10px;
        }
        select {
            width: 80%;
            background-color: #fff;
            padding: 8px;
            margin-bottom: 15px;
            border: 1px solid #756b6b;
            border-radius: 4px;
        }
        input[type="submit"]:hover {
            background-color: #45a049;
        }

    </style>
</head>
<body>
    <div class="container">
        <form method="post" action="<?php echo $_SERVER['PHP_SELF']; ?>">
            <label for="username">Enter your name:</label>
            <input type="text" id="username" name="username" required><br><br>

            <label for="theme">Select Theme:</label>
            <select name="theme" id="theme">
                <option value="light" <?php echo ($theme === 'light') ? 'selected' : ''; ?>>Light</option>
                <option value="dark" <?php echo ($theme === 'dark') ? 'selected' : ''; ?>>Dark</option>
            </select><br><br>

            <input type="submit" name="submit" value="Submit">
        </form>
        </div>
    </body>
</html>
        <?php
    }
}

// You can use the $theme variable to apply the selected theme to your HTML/CSS
?>

    
</body>
</html>







