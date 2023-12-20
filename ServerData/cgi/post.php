#!/usr/bin/php
<html lang="en">
<body style="background-color: darkslateblue; color: lightgrey;">
<div style="display: flex; flex-direction: column; align-items: center; justify-content: center; height: 100%;">
	<h1>Passing form data to PHP-CGI</h1>


<?php


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

?>