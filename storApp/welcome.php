#!./ServerData/cgi/php-cgi
<?php
session_start();

$theme = isset($_COOKIE['theme']) ? $_COOKIE['theme'] : 'light'; // Default to 'light' if not set


if(isset($_SESSION['username'])) {
    ?>
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Document</title>
        <?php if ($theme == 'dark') { ?>
            <link rel="stylesheet" href="dark.css">
        <?php } else { ?>
            <link rel="stylesheet" href="light.css">
        <?php } ?>
    </head>
    <body>
        <h1>yes i'm loging in </h1>
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

