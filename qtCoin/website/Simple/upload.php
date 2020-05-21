<?php

//~ ini_set('display_startup_errors', 1);
//~ ini_set('display_errors', 1);
//~ error_reporting(-1);

$file = basename($_FILES["fileToUpload"]["name"]);
#$file = mb_strimwidth 	 ( $file , 0, 25, "-")  ;

$target_mdir = "uploads/transactions/";
$target_pdir = "uploads/pic/";
//$target_mfile = $target_mdir . basename($_FILES["fileToUpload"]["name"]);
//$target_pfile = $target_mdir . basename($_FILES["fileToUpload"]["name"]);
$target_mfile = $target_mdir . $file;
$target_pfile = $target_pdir . $file;

$uploadOk = 1;
$emailnotify=1;

//check valid email

        $to = 'admin@netpipe.ca';
        $subject = 'New transaction';
        $message = 'new file upload'; 
        $from = 'admin@netpipe.ca';
        
//check valid email
//$name = test_input($_POST["emailid"]);
//function validateEMAIL($EMAIL) {
    //$v = "/[a-zA-Z0-9_-.+]+@[a-zA-Z0-9-]+.[a-zA-Z]+/";
	//https://stackoverflow.com/questions/12026842/how-to-validate-an-email-address-in-php
   // return (bool)preg_match($v, $EMAIL);
//}
//$email = "john.doe@example.com";

$email = $_POST["emailid"];

if (filter_var($email, FILTER_VALIDATE_EMAIL)) {
  echo("$email is a valid email address<br>");
} else {
  echo("$email is not a valid email address<br>");
}

//~ $email = test_input($_POST["emailid"]);
//~ function test_input($data) {
  //~ $data = trim($data);
  //~ $data = stripslashes($data);
  //~ $data = htmlspecialchars($data);
  //~ return $data;
//~ }
 
// Check if image file is a actual image or fake image
if(isset($_POST["submit"])) {
    $checkimage = getimagesize($_FILES["fileToUpload"]["tmp_name"]);
    
    if($checkimage != false) {
        echo "File is an image - " . $checkimage["mime"] . "." . "<br>";
        $uploadOk = 1;
        $target_file = $target_pfile;
    } else {
        echo "File is not an image. <br> ";
        $uploadOk = 1;  // set to 1 because were uploading video too
        $target_file = $target_mfile;
    }
}

 $imageFileType = strtolower(pathinfo($target_file,PATHINFO_EXTENSION));

// Check if file already exists
if (file_exists($target_file)) {
    echo "Sorry, file already exists.<br>";
    $uploadOk = 0;
}

// Check file size
if ($_FILES["fileToUpload"]["size"] > 510500000) {
    echo "Sorry, your file is too large.<br>";
    $uploadOk = 0;
}

// Allow certain file formats
if($imageFileType != "jpg" && $imageFileType != "webm" && $imageFileType != "png" && $imageFileType != "jpeg"
&& $imageFileType != "gif" && $imageFileType != "JPG" && $imageFileType != "WEBM" && $imageFileType != "PNG" && $imageFileType != "JPEG"
&& $imageFileType != "GIF" && $imageFileType != "XVID" && $imageFileType != "mov" && $imageFileType != "xvid" && $imageFileType != "AVI" 
&& $imageFileType != "ZIP" && $imageFileType != "7z" && $imageFileType != "7Z" && $imageFileType != "zip" && $imageFileType != "rar"
&& $imageFileType != "RAR" && $imageFileType != "tar" && $imageFileType != "gz" && $imageFileType != "GZ" && $imageFileType != "TAR"
&& $imageFileType != "PDF" && $imageFileType != "pdf" && $imageFileType != "DJVU" && $imageFileType != "djvu" && $imageFileType != "txt"
&& $imageFileType != "TXT" && $imageFileType != "DOC" && $imageFileType != "BLEND" && $imageFileType != "blend" && $imageFileType != "obj"
&& $imageFileType != "OBJ" && $imageFileType != "STL" && $imageFileType != "stl") {
    echo "only xvid,mov,avi,WEBM-preferred,JPG, JPEG, PNG & GIF files are allowed.<br>";
    $uploadOk = 0;
}

 if ($uploadOk == 0) {
	echo "Sorry, your file was not uploaded.";
 } else {
		
	 if ( $checkimage == false ) {
	  echo "Uploading transaction <br>";
	   $name = preg_replace("/[^A-Z0-9._-]/i", "_", $target_file); // safe filename
	  $uploaded = move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_mdir . $file);

	 } else {	
	  echo "Upload Picture";
	  $uploaded = move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_pdir . $file);
	 }
 
	if ($uploaded == "1"){	
        echo "The file ". $file. " has been uploaded.<br>";
        
        //write textfile with email address
        //if(isset($_POST['emailid']){
            $myfile = fopen("$target_file.txt", "w") or die("Unable to open file!<br>");
            $txt = $_POST["emailid"];
            echo $txt;
            fwrite($myfile, $txt);
            fclose($myfile);
            

    if ($emailnotify == "1"){        
        // Sending email

        if(mail($to, $subject, $message)){
            echo '<br>Your mail has been sent successfully. <br>';
        } else {
            echo 'Unable to send email. Please try again.<br>';
        }
	}
        
      //  };
    } else {
        echo "Sorry, there was an error uploading your file.<br>";
    }
}
?> 
