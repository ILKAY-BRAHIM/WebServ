#!/usr/bin/php
<html lang="en">
<body >
<div >
	<h1>Test Upload</h1>
    #!/usr/bin/php
<?php
// Read from stdin
$requestBody = file_get_contents("php://stdin");

// Extract the boundary from CONTENT_TYPE
preg_match('/boundary=(.*)$/', $_SERVER['CONTENT_TYPE'], $matches);
$boundary = $matches[1];

    echo "boundary is ".$boundary, "<br/>";

// Split content by boundary
$blocks = preg_split("/-+$boundary/", $requestBody);
array_pop($blocks);

// Parse each block
foreach ($blocks as $block) {
    if (empty($block)) {
        continue;
    }

    // Split the block into header and content sections
    if (preg_match("/([\w\W]+)\r\n\r\n([\w\W]+)/m", $block, $matches)) {
        $blockHeaders = $matches[1];
        $blockContent = $matches[2];

        // Parse headers
        $headers = array();
        foreach (explode("\r\n", $blockHeaders) as $header) {
            list($name, $value) = explode(": ", $header);
            $headers[$name] = $value;
        }

        // Check if the block contains a file upload
        if (isset($headers['Content-Disposition']) && strpos($headers['Content-Disposition'], 'filename') !== false) {
            // Extract necessary data for file
            preg_match('/name=\"([^\"]*)\"; filename=\"([^\"]*)\"/i', $headers['Content-Disposition'], $fileMatches);
            $fieldName = $fileMatches[1];
            $fileName = $fileMatches[2];

            // Create a temp file
            $tmpFileName = tempnam(sys_get_temp_dir(), 'upload');
            // $tmpFileName = $tmpFileName . "." . "png";
            
            // Save content to the temp file
            file_put_contents($tmpFileName, $blockContent);

            // Collect file information for further processing (like moving to a final location)
            $_FILES[$fieldName] = array(
                'name' => $fileName,
                'type' => $headers['Content-Type'],
                'tmp_name' => $tmpFileName,
                'error' => 0,
                'size' => filesize($tmpFileName)
            );
        } else {
            // Regular form field - extract the name
            if (preg_match('/name=\"([^\"]*)\"/i', $headers['Content-Disposition'], $fieldMatches)) {
                $fieldName = $fieldMatches[1];
                // Add the field content (value) to the $_POST array
                $_POST[$fieldName] = ltrim($blockContent);
            }
        }
    }
}

// At this point, you would have the $_FILES and $_POST arrays populated similarly to how PHP does it natively.


// Check if the form was submitted
if($_SERVER["REQUEST_METHOD"] == "POST") {
    // Check if file was uploaded without errors
    if(isset($_FILES["pdfUpload"]) && $_FILES["pdfUpload"]["error"] == 0) {
        $file = $_FILES["pdfUpload"]; // Your uploaded file

        // Access the $_FILES array for uploaded file information
        $fileName = $file["name"]; // Original name of the file
        $fileType = $file["type"]; // file type
        $fileTempPath = $file["tmp_name"]; // temporary location on the server
        $fileError = $file["error"]; // any error code result from the upload
        $fileSize = $file["size"]; // size in bytes

        // You can perform operations such as moving the file
        $uploadDirectory = "/Users/rrasezin/Desktop/WebServ/ServerData/cgi/uploads/";
        $uploadPath = $uploadDirectory . basename($fileName);

        // Move the file from its temporary location to the desired upload directory
        // if(!move_uploaded_file($fileTempPath, $uploadPath)) {
        //     echo "Error: A problem occurred during file upload!";
        // }

        if (rename($fileTempPath, $uploadPath)) {
            echo "File is valid, and was successfully uploaded.\n";
        } else {
            echo "Upload failed";
        }
        

    } else {
        echo "Error: " . $_FILES["pdfUpload"]["error"];
    }
}

echo "s1 is ".$_POST["name"], "<br/>";
echo "s2 is ".$_POST["email"], "<br/>";
echo "s3 is ".$_FILES["pdfUpload"]["name"], "<br/>";
echo "s4 is ".$_FILES["pdfUpload"]["type"], "<br/>";
echo "s5 is ".$_FILES["pdfUpload"]["tmp_name"], "<br/>";
echo "s6 is ".$_FILES["pdfUpload"]["error"], "<br/>";
echo "s7 is ".$_FILES["pdfUpload"]["size"], "<br/>";

echo "uploadPath is ".$uploadPath, "<br/>";
echo "fileTempPath is ".$fileTempPath, "<br/>";


// echo "blocks is ".$blocks, "<br/>";
// write the block with echo
// echo $blocks[0], "<br/>";
// echo $blocks[1], "<br/>";
// echo $blocks[2], "<br/>";
// echo $blocks[3], "<br/>";
// echo $blocks[4], "<br/>";
// Process upload, validate, and clean up
?>

</div>
</body>
</html>