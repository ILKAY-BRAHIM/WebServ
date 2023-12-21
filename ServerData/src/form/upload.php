#!/usr/bin/php
<html lang="en">
<body >
<div >
	<h1>Test Upload</h1>

<?php
// Check if the form was submitted
if($_SERVER["REQUEST_METHOD"] == "POST") {
    // Check if file was uploaded without errors
    if(isset($_FILES["fileToUpload"]) && $_FILES["fileToUpload"]["error"] == 0) {
        $file = $_FILES["fileToUpload"]; // Your uploaded file

        // Access the $_FILES array for uploaded file information
        $fileName = $file["name"]; // Original name of the file
        $fileType = $file["type"]; // file type
        $fileTempPath = $file["tmp_name"]; // temporary location on the server
        $fileError = $file["error"]; // any error code result from the upload
        $fileSize = $file["size"]; // size in bytes

        // You can perform operations such as moving the file
        $uploadDirectory = "/uploads/";
        $uploadPath = $uploadDirectory . basename($fileName);

        // Move the file from its temporary location to the desired upload directory
        if (move_uploaded_file($fileTempPath, $uploadPath)) {
            echo "File has been uploaded successfully";
        } else {
            echo "Error occurred during the file upload";
        }
    } else {
        echo "Error: " . $_FILES["fileToUpload"]["error"];
    }
}
?>

</div>
</body>
</html>