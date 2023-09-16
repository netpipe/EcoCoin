<!DOCTYPE html>
<?php
	session_start();
	require_once 'login_query.php';
?>
<html xml:lang="en" lang="en">
<head>
    <meta name="Keywords" content="keywords" /><meta name="Description" content="qtCoin" />

    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <meta http-equiv="refresh" content="50">
    <!--should be set higher for less traffic use DEVELOPMENT MODE-->

    <style type="text/css" media="screen, projection">
        @import "style.css";
    </style>
    
    <title>xChange</title>
    
</head>
<!-- Remember to chmod 0755 uploads directory -->


<!--body {
  background: lightblue url("img_tree.gif") no-repeat fixed center;
  background-image: url('')
} -->

<body style="background:#968D87">
 <div id="wrapper">
	 <!--logo-->
    <div align="center" style="color:black;background-color:#4A5568;border-radius: 5px 5px 5px 5px;border:2px black solid;height:100%;width:100%;">
        <div style="margin:1px; border-radius: 15px 15px 15px 15px; border:black solid 1px; background:#515151">
        	<img src="qtcoin2.png" width="800px" />
        </div>
       <!-- <img src="logo3.png" width="1000px" /> -->
	<!--<div style="border:2px black solid;" > style="border:2px black solid;" -->
	 <ul id="TJK_dropDownMenu">
		<li id="AB" style="background:#616C7A ; width:12% "> <a href="index.php">Menu</a>	
  		 <ul>
			<li> <a href="./index.php">Home</a>  </li>
			<li> <a href="?page=coins">Investments</a>  </li>
		 </ul>
		</li>
		<li id="CF" style="background:#5C6776 ; width:12% ">  <a href="?page=licence">Content Licence</a> </li>
        <li id="3m" style="background:#546070 ; width:12% "> <a href="?page=rmcookie"> </a>	</li>
        <li id="3m" style="background:#4A5568 ; width:12% "> <a href="forums.php"> </a>	</li>
        <li id="3m" style="background:#3A455B ; width:12% "> <a href="forums.php"> </a>	 </li>
        <li id="3m" style="text-align:right;background:#2D3851;width:40%"> <div style="margin-right:20px;"> <a> <?php print(date("D M d Y | G:i:s"));?> </a> </div> </li>
	  </ul>
	<!-- </div> menu-->

    <form action="upload.php" method="post" enctype="multipart/form-data">
        <p style="color:#F78989">use wallet generated file to upload transaction
        <input type="file" name="fileToUpload" id="fileToUpload">
        <input type="submit" value="Upload" name="submit">
        </p>
    </form>
    
	<div>
		<h3>PHP - Login And Registration</h3>
		<hr/>
		<?php
			echo "session val:: Username: ".$_SESSION['username'].".</br>";
			echo "session val:: Password: ".$_SESSION['password']."</br>";
		?>
		<a href="login.php">Logout</a>
		<h1>Welcome <?php echo $username; ?> !</h1>
	</div>
    
<?php
//~ ini_set('display_errors', 1);
//~ ini_set('display_startup_errors', 1);
//error_reporting(E_ALL);
$sitename="http://www.qtCoin.ca";
         //   $pwd = getcwd();
            //chdir('test');
        
        
include_once("render.php");
	
	
        //start of green div content box
        echo '<div id="vbody" style="border-radius: 15px 15px 15px 15px;clear:both;text-align:center;background-color:green; margin:20px;border:2px black solid;" >';
        
        
        //GET PAGE CALLBACK
        $page=$_GET["page"];

        // CONTACT
        if  ( $page == "contact" && true){
            echo '<a href="' . 'mailto:admin@' . $sitename.'"' .'> Email </a>';
          // rendervideo($test);
          
		// VIDEOS
        }  
                elseif ( $page == "books" ) {
			echo '<p align="middle"><B>' . $sitename . ' Books</B>';
			$bookpath = "books";
			$dirs = glob($bookpath . '/*' , GLOB_ONLYDIR);
			foreach($dirs as $dirs2) {
				render($dirs2.".html");
				//get first file from dir to display as picture
			}
            echo "</p>";
        }  
									
        elseif  ( $page == "" ){
			echo "main page";
		echo '<div class="wrapper">';
echo			'<div class="progress-bar">';
	echo			'<span class="progress-bar-fill" style="width: 80%;"></span>';
			echo '</div>';
		echo '</div>';



			render("books");
			//render("stories");

			}

        echo '</div>'; //end of green content box

      ?>
<progress max="100" value="80"></progress>
	<div id="footer2" style="border-radius: 15px 15px 15px 15px;margin:10px;background:#d0d0d0;clear:both;border:1px black solid;">
	    			<br><br>

<!--
		<h1>VideoHost</h1><br>
		<img src="const.gif" /><br><br>
		
				<abbr title="U">WIKI - comming soon</abbr>
-->
<!--
//         <h3>This is a heading</h3>
//         <p>This is a paragraph.</p>
//         <div style="background-color:green">
//         <p>This is a paragraph.</p>
//         //  echo rand(1, 10)."<br>";
-->

        <i> xChange<br>
        <br> Running this site requires no Javascript or Flash<br><br></i>


	</div>	<!-- footer --> 
	
    <br>

  </div> <!--wrapper-->
  

</body>
</html>
