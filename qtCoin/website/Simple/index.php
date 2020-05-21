<!DOCTYPE html>
<html xml:lang="en" lang="en">
<head>
    <meta name="Keywords" content="keywords" /><meta name="Description" content="qtCoin" />

    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <meta http-equiv="refresh" content="50">
    <!--should be set higher for less traffic use DEVELOPMENT MODE-->

    <style type="text/css" media="screen, projection">
        @import "style.css";
    </style>
    
    <title>qtCoin</title>
    
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
    

    
<?php
//~ ini_set('display_errors', 1);
//~ ini_set('display_startup_errors', 1);
//error_reporting(E_ALL);
$sitename="http://www.qtCoin.ca";
         //   $pwd = getcwd();
            //chdir('test');
        
        
	function render($file){
		$fileinfo=pathinfo($file);
	 							 $dirname = basename($file);
	 							 $filename = $fileinfo['filename'];
	 							 $extension = $fileinfo['extension'];
	// echo '<div style="background-color:red;width:auto;height:auto">'; 
	// echo "test";
				echo '<p>';
		echo '<div id="videobox" style="border-radius: 15px 15px 15px 15px;margin:20px;background-color:#c1c1c1;height:200px;float:left;border:2px black solid;width:330px;" >';
			//TITLEBAR for content box
		    echo '<div style="margin-left:  background-color:black;margin:2px">';
				//first bubble
				echo '<div style="border-radius: 15px 15px 15px 15px;width:10%; text-align:center; float:left;background-color:black;height:20px;">';
				echo "PIC";
				echo '</div>';
				//second bubble
				echo '<div style="border-radius: 15px 15px 15px 15px;width:80%; text-align:center; float:left;background-color:#5C6776;height:20px;">';
				if ( $extension == "") {
					echo '<a href="?page=' . $file . '">' . $dirname . '</a>';
				}
				if ( $extension == "html") {
					echo '<a href="./books/' . $filename . '/index.html">' . $filename . '</a>';
				}
				else {
					echo '<a href="' . $file . '">' . $file . '</a>';
				}
				echo '</div>';
				//3rd bubble
				echo '<div style="border-radius: 15px 15px 15px 15px;width:10%; text-align:center; float:left;background-color:#3A455B;height:20px;">';
				echo '</div>';
			    echo '</div>'; //titlebar

		   // echo '<div style="clear: both;vertical-align: middle;width:auto;height:auto" >';
		   
				//content box
				    echo '<div style="clear:both;width:auto;height:auto" >';
					 // echo '<video controls  style="clear:both;" poster="title_anouncement.jpg" width="250">';


					 if ( $extension == "webm" ) {
						echo '<video controls style="" preload="none" loop="1" poster="play.png" width="300">';
							echo '<source src="' . $file . '"' .'type="video/webm";codecs="vp8, vorbis" />'; 
						echo '</video>';
					 } elseif ( $extension == "" ) { 
						// echo "testing123";
						 echo '<div align="middle" style="height:300px;width:300px"/>';
						 echo '<p><a href="?page=' . $file . '">' ;
						 echo '<img src="./' . $file . '/' . $dirname . '.png' . '"' . ' alt="' . $file . '"' . ' width="' . '160px"' . ' height="' . '160px"' . '/></a></p>';
						 echo "</div>";		
						// echo "</p>";
					 }
					 elseif ( $extension == "html" ) { 
						// echo "testing123";
						 //echo $dirname;
						 echo '<div align="middle" style="height:300px;width:300px">';
						 echo '<p><a href="./books/' . $filename . '/index.html">' ;

						 echo '<img src="./books/' . $filename . '/' . $filename . '.jpg' . '"' . ' alt="' . $filename . '"' . ' width="' . '160px"' . ' height="' . '160px"' . '/></a></p>';
						 echo "</div>";		
						// echo "</p>";
						// need jpg or png option
					 }
					 else { 
						 echo "hi";
						 //~ echo "<img src=" . '"http://grandgallery.net/' . $file . 'width="400px"' . "/>"; 
						 echo "<p><a href=" . './' . $file . ">";
						 echo '<img src="./' . $file . '"' . ' alt="'. $file .'"' .' width="' .'160px"' . ' height="'.'160px"' . '/>'; 
						 echo '</a></p>';	
							}
				echo '</div>';//content box
	   echo '</div>'; //videobox
	   echo "</p>";

	}  //contentbox generator
	
	
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

        <i> qtCoin<br>
        <br> Running this site requires no Javascript or Flash<br><br></i>


	</div>	<!-- footer --> 
	
    <br>

  </div> <!--wrapper-->
  

</body>
</html>
