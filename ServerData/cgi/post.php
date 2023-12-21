#!/usr/bin/php
<html lang="en">
<body style="background-color: darkslateblue; color: lightgrey;">
<div style="display: flex; flex-direction: column; align-items: center; justify-content: center; height: 100%;">
	<h1>Passing form data to PHP-CGI</h1>


<?php

<<<<<<< HEAD
        if (getenv('REQUEST_METHOD') == 'POST') {
            echo "Using the POST method to pass data<br/>";
            $stdin = fopen('php://stdin', 'r');
            $data = stream_get_contents($stdin);
            parse_str($data, $dataa);
            $_POST = $dataa;
            echo "s1 is ".$_POST["name"], "<br/>";
            echo "s2 is ".$_POST["email"], "<br/>";
        }
        else if (getenv('REQUEST_METHOD') == 'GET') {
        echo "Using the GET method to pass data<br/>";
        $var = getenv('QUERY_STRING');
        parse_str($var, $dataa);
        $_GET = $dataa;
        echo "s1 is ".$_GET["name"], "<br/>";
        echo "s2 is ".$_GET["email"], "<br/>";
        }
=======

    echo "Using the POST method to pass data<br/>";

    $stdin = fopen('php://stdin', 'r');
    // $input = fgets($stdin);
    $input = stream_get_contents($stdin);
    // $input = 
    parse_str($input, $output);
    // echo $output;
    echo "s1 is ".$_POST["s1"].".<br/>";
    echo $output['name'],"\n"; // would output John
    // echo "\n";
    echo $output['email']; // would output 30
    // echo $input;

>>>>>>> be7b5ad2cffcc7710ab9a52965db17298e47d848
?>