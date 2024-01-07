#!./php-cgi/php-cgi
<?php
$session_lifetime = 30 * 24 * 60 * 60; // 30 days in seconds
ini_set('session.gc_maxlifetime', $session_lifetime);
session_set_cookie_params($session_lifetime);
session_start();

$theme = isset($_COOKIE['theme']) ? $_COOKIE['theme'] : 'dark';

if(isset($_SESSION['username'])) {
    ?>
        <!DOCTYPE html>
        <html>
        <head>
            <title>Styled Form</title>
            <?php if ($theme == 'dark') { ?>
                    <link rel="stylesheet" href="../dark.css">
                <?php } else { ?>
                    <link rel="stylesheet" href="../light.css">
                <?php } ?>
        </head>
        <body>
            <h1> <?php echo "Hello, ".$_SESSION['username']."!";  ?> </h1>
            <h1><a href="../welcome.php">Go to welcome page</a></h1>
        </body>
        </html>
    <?php
} else {
    if(isset($_POST['submit'])) {
        $username = $_POST['username'];

        // Set the theme cookie
        if(isset($_POST['theme'])) {
            $theme = $_POST['theme'];
            setcookie('theme', $theme, time() + (86400 * 30), "/"); // Cookie set for 30 days
        }

        // Setting the session
        $_SESSION['username'] = $username;
        ?>
        <!DOCTYPE html>
        <html>
        <head>
            <title>Styled Form</title>
            <?php if ($theme == 'dark') { ?>
                    <link rel="stylesheet" href="../dark.css">
                <?php } else { ?>
                    <link rel="stylesheet" href="../light.css">
                <?php } ?>
        </head>
        <body>
            <h1> <?php echo "Hello, $username!";  ?> </h1>
            <h1><a href="../welcome.php">Go to welcome page</a></h1>
        </body>
        </html>
        <?php
    } else {
        ?>
        <!DOCTYPE html>
        <html>
        <head>
            <title>Styled Form</title>
            <?php if ($theme == 'dark') { ?>
                    <link rel="stylesheet" href="../dark.css">
                <?php } else { ?>
                    <link rel="stylesheet" href="../light.css">
                <?php } ?>
        </head>
        <body>
            <div class="update-form">
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
?>

    







