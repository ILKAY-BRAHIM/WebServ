#!/Users/bchifour/Desktop/WEBSERV/ServerData/cgi/php-cgi
<html lang="en">
<head>
   
</head>
<body style="background-color: lightgrey; color: red;">

<h1>Uploading images using PHP-CGI</h1>


<div style="display: flex; flex-direction: column; align-items: center; justify-content: center; height: 100%;">

   <form action="uplud.php" method="POST" enctype="multipart/form-data">
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
      echo "Success <br/>";
      

      if(empty($errors)==true)
      {
         echo "<img src=".$path." width=50% />";
      }
      else
      {
         print_r($errors);
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