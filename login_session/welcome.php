#!./php-cgi/php-cgi
<?php
$session_lifetime = 30 * 24 * 60 * 60; // 30 days in seconds
ini_set('session.gc_maxlifetime', $session_lifetime);
session_set_cookie_params($session_lifetime);
session_start();

$theme = isset($_COOKIE['theme']) ? $_COOKIE['theme'] : 'light'; // Default to 'light' if not set


if(isset($_SESSION['username'])) {
    ?>
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Welcome</title>
        <?php if ($theme == 'dark') { ?>
            <link rel="stylesheet" href="dark.css">
        <?php } else { ?>
            <link rel="stylesheet" href="light.css">
        <?php } ?>
    </head>
    <body>
        <div class="welcome">
            <h1>Welcome <?php echo $_SESSION['username'] ?>  </h1>
            <h1><a href="update_session.php">Update Session</a></h1>
        </div>
    </body>
    </html>

    <?php
} else {
    {
        header("Location: login/login.php");
        exit();
    }
}
?>

