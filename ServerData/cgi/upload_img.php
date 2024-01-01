#!./ServerData/cgi/php-cgi
<html lang="en">
<head>
<style>
   * {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
    font-family: 'Courier New', Courier, monospace;
    /* background-color: #f4f4f4; */
   }
   body {
         background-color: #d7d3d3;
         /* padding: 20px; */
   }
   h1 {
    text-align: center;
    margin: 1.5em;
   }
   .c1 {
    display: flex;
    flex-direction: column;
    align-items: center;
   }
   form {
    max-width: 600px;
    margin: 2rem 2rem;
    background-color: #e3e1e1;
    padding: 20px;
    border-radius: 8px;
    box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
    
   }
</style>
</head>
<body >

<h1>Uploading images using PHP-CGI</h1>


<div class="c1">

   <form action="upload_img.php" method="POST" enctype="multipart/form-data">
         <input type="file" name="image" />
         <input type="submit"/>
   </form>


<?php
   if(isset($_FILES['image']))
   {
      $errors= array();
      $file_name = $_FILES['image']['name'];
      $file_size =$_FILES['image']['size'];
      $file_tmp = $_FILES['image']['tmp_name'];
      $file_type= $_FILES['image']['type'];
      $tmp = $_FILES['image']['name'];
      $tmp = explode('.',$tmp);
      $file_ext = strtolower(end($tmp));
      
      $extensions= array("jpeg","jpg","png");
      
      if(in_array($file_ext,$extensions) === false)
      {
         $errors[]="extension not allowed, please choose a JPEG or PNG file.";
      }
      $path = "./uploads/".$file_name ;
      move_uploaded_file($file_tmp,$path);
      
      
      if(empty($errors)==true)
      {
         echo "<p style=\"font-size: 1.2rem;\"> Success <br/></p>";
         echo "<img style=\"width: 25rem;\" src=".$path." width=50% />";
      }
      else
      {
         echo "<p style=\"font-size: 1.2rem;\"> Error : extension not allowed, please choose a JPEG or PNG file.<br/></p>";
         // print_r($errors);
      }
   }
   else
   {
      echo "No file has been uploaded";
   }
?>



</div>
</body>
</html>