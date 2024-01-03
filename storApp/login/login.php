#!./ServerData/cgi/php-cgi
<?php
// session_start();

$valid_username = 'user';
$valid_password = 'password';

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $username = $_POST['username'];
    $password = $_POST['password'];

    if ($username === $valid_username && $password === $valid_password) {
        $_SESSION['authenticated'] = true;
        header('Location: welcome/welcome.php');
        exit();
    } else {
        echo 'Invalid credentials. Please try again.';
    }
}

?>
