#!./php-cgi/php-cgi
<?php
$session_lifetime = 30 * 24 * 60 * 60; // 30 days in seconds
ini_set('session.gc_maxlifetime', $session_lifetime);
session_set_cookie_params($session_lifetime);
session_start();

$theme = isset($_COOKIE['theme']) ? $_COOKIE['theme'] : 'dark';

if(isset($_SESSION['username'])) {
    if(isset($_POST['update_submit'])) {
        // Handle updating session data
        $newUsername = $_POST['new_username'];
        $newTheme = $_POST['new_theme'];

        // Update session data
        if (!empty($newUsername))
            $_SESSION['username'] = $newUsername;
        // Update the theme cookie
        if (!empty($newTheme))
        {
            setcookie('theme', $newTheme, time() + (86400 * 30), "/");
            $theme = $newTheme;
        }
        ?>
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <title>Update Session</title>
            <?php if ($theme == 'dark') { ?>
                <link rel="stylesheet" href="dark.css">
            <?php } else { ?>
                <link rel="stylesheet" href="light.css">
            <?php } ?>
        </head>
        <body>
            <div class="welcome">
                <h1> <?php echo "Session and cookie updated successfully!"; ?> </h1>
                <h1><a href="welcome.php">Back to home</a></h1>
            </div>
        </body>
        </html>
        <?php
    } else {
        // Display form to update session data
        $theme = isset($_COOKIE['theme']) ? $_COOKIE['theme'] : 'dark';
        ?>
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <title>Update Session</title>       
            <?php if ($theme == 'dark') { ?>
                <link rel="stylesheet" href="dark.css">
            <?php } else { ?>
                <link rel="stylesheet" href="light.css">
            <?php } ?>
        </head>
        <body>
        <div class="update-form">
        <form method="post" action="<?php echo $_SERVER['PHP_SELF']; ?>">
            <label for="new_username">Update Username:</label>
            <input type="text" id="new_username" name="new_username"><br><br>

            <label for="new_theme">Select New Theme:</label>
            <select name="new_theme" id="new_theme">
                <option value="dark">Dark</option>
                <option value="light">Light</option>
            </select><br><br>

            <input type="submit" name="update_submit" value="Update Session">
        </form>
        </div>
        </body>
        </html>

        <?php
    }
} else {
    ?>
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <title>Update Session</title>
            <?php if ($theme == 'dark') { ?>
                <link rel="stylesheet" href="dark.css">
            <?php } else { ?>
                <link rel="stylesheet" href="light.css">
            <?php } ?>
        </head>
        <body>
            <h1> <?php echo "Session not found. Please log in."; ?>   </h1>
            <h1><a href="/login/login.php">Go to home</a></h1>
        </body>
        </html>
    <?php
}
?>
