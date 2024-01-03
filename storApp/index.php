#!./ServerData/cgi/php-cgi
<?php
session_start();

// Check if the theme is set in session or cookie
if (isset($_GET['theme'])) {
    $theme = $_GET['theme'];

    // Set the theme in session and cookie
    $_SESSION['theme'] = $theme;
    setcookie('theme', $theme, time() + (86400 * 30), "/"); // Cookie for 30 days
} elseif (isset($_SESSION['theme'])) {
    $theme = $_SESSION['theme'];
} elseif (isset($_COOKIE['theme'])) {
    $theme = $_COOKIE['theme'];
} else {
    // Default theme
    $theme = 'light';
}
?>

<!DOCTYPE html>
<html>
<head>
    <title>Login Page</title>
    <?php
    // Apply styles based on the theme
    if ($theme === 'dark') {
        echo '<style>body { background-color: #333; color: #fff; }</style>';
    } else {
        echo '<style>body { background-color: #fff; color: #333; }</style>';
    }
    ?>
</head>
<body>
    <div class="login-container">
        <form id="loginForm" action="login.php" method="post">
            <h2>Login</h2>
            <input type="text" name="username" placeholder="Username" required>
            <input type="password" name="password" placeholder="Password" required>
            <button type="submit">Login</button>
        </form>
    </div>

    <?php
    // Generate links to switch themes
    $darkThemeLink = $_SERVER['PHP_SELF'] . "?theme=dark";
    $lightThemeLink = $_SERVER['PHP_SELF'] . "?theme=light";
    ?>

    <a href="<?php echo $darkThemeLink; ?>">Dark Mode</a>
    <a href="<?php echo $lightThemeLink; ?>">Light Mode</a>
</body>
</html>
