#!/usr/bin/php
<html lang="en">
<body style="background-color: darkslateblue; color: lightgrey;">
<div style="display: flex; flex-direction: column; align-items: center; justify-content: center; height: 100%;">
	<h1>Passing form data to PHP-CGI</h1>


<?php

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
?>