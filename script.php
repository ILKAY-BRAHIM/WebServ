<?php
// Check if the form was submitted
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Check if file was uploaded without errors
    echo "Hello";
    if (isset($_FILES["pdfUpload"]) && $_FILES["pdfUpload"]["error"] == 0) {
        // Define allowed file types
        $allowed = ["pdf" => "application/pdf"];
        $filename = $_FILES["pdfUpload"]["name"];
        $filetype = $_FILES["pdfUpload"]["type"];
        $filesize = $_FILES["pdfUpload"]["size"];
    
        // Verify file extension
        $ext = pathinfo($filename, PATHINFO_EXTENSION);
        if (!array_key_exists($ext, $allowed)) die("Error: Please select a valid file format.");
    
        // Verify file size - 5MB maximum
        $maxsize = 5 * 1024 * 1024;
        if ($filesize > $maxsize) die("Error: File size is larger than the allowed limit.");
    
        // Verify MYME type of the file
        if (in_array($filetype, $allowed)) {
            // Check whether file exists before uploading it
            if (file_exists("upload/" . $filename)) {
                echo $filename . " is already exists.";
            } else {
                move_uploaded_file($_FILES["pdfUpload"]["tmp_name"], "upload/" . $filename);
                echo "Your file was uploaded successfully.";
            } 
        } else {
            echo "Error: There was a problem uploading your file. Please try again."; 
        }
    } else {
        echo "Error: " . $_FILES["pdfUpload"]["error"];
    }

    // Capture and sanitize other form data
    $name = htmlspecialchars($_POST['name']);
    $email = htmlspecialchars($_POST['email']);
    $message = htmlspecialchars($_POST['message']);

    // Process your form data here: store in database, send email, etc.
    // ...

    echo "Name: $name\n";
    echo "Email: $email\n";
    echo "Message: $message\n";
}
?>