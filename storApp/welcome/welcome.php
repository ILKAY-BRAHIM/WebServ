#!./ServerData/cgi/php-cgi
<?php
session_start();

// if (!isset($_SESSION['authenticated']) || $_SESSION['authenticated'] !== true) {
//     header('Location: ../index.html');
//     exit();
// }
?>

<!DOCTYPE html>
<html>
<head>
    <title>Welcome</title>
</head>
<body>
    <h1>Welcome!</h1>
    <p>You have successfully logged in.</p>
</body>
</html>
