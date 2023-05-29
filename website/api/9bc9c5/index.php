
<!DOCTYPE html>
<?php
    session_start();
    $securitycode = '';
    if(isset($_SESSION['device'])&&isset($_SESSION['name']))
    {
        $securitycode = $_SESSION['device'];
        $substr_sercode = substr($securitycode,-6);
        $nameacc = $_SESSION['name'];
    }
    else
    {
        header('Location: /login');
    }
?>
<html>
<head>
  
  <title>ESP32-CAM Photo Gallery</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <link rel="shortcut icon" href="/C-File/logo.png" type="image/x-icon">
  <style>
    .flex-container {
      display: flex;
      flex-wrap: wrap;
      background-color: whitesmoke;
      max-width: 1200px;
      margin: 10px auto;
    }
    .flex-container > div {
      text-align: center;
      margin: 10px;
    }
  </style>
  <meta charset="utf-8">
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.1/css/all.min.css" integrity="sha512-+4zCK9k+qNFUR5X+cKL9EIR+ZOhtIloNl9GIKS57V1MyNsYpYcUrUeQc9vNfzsWfV28IaLL3i96P9sdNyeRssA==" crossorigin="anonymous" />
  <link rel="stylesheet" href="./assets/css/style.css">
  <link rel="stylesheet" href="./assets/css/topnav.css">
  <link rel="stylesheet" href="./assets/css/responsive.css">
  <script src="./assets/js/javascript.js"></script>
</head>
<body>
  <div class="top-nav" id="#">
      <div class="navigation">
          <span class="symbol_menu">≡</span>
          <ul class="menu">
              <li><a href="/">Trang chủ</a></li>
              <li><a class = "select"href="/history_door">Quản lý khóa của bạn</a></li>
              <li><a href="/about-us">Giới thiệu</a></li>
              <li><a href="/Contact">Liên hệ</a></li>
          </ul>
          <div class="user_info">
              <span> Xin chào <?php echo $nameacc?></span>
              <ul>
                  <li><a href="/login/logout.php">Đổi mật khẩu</a></li>
                  <li><a href="/login/logout.php">Đăng xuất</a></li>
              </ul>
          </div>
      </div>
  </div>
  <div class="nav-control">
      <ul>
          <li><a href="#"><i class="fas fa-chevron-up"></i></a></li>
          <li><a href="/history_door/"><i class="fas fa-home"></i></a></li>
          <li><a href=""><i class="fas fa-sync"></i></a></li>
          <li><a href="/api/<?php echo $substr_sercode?>"><i class="fas fa-images"></i></a></li>
          <li><a href="http://khoacuaonline.000webhostapp.com/video_stream/"><i class="fas fa-video"></i></a></li>
      </ul>
  </div>
<h2>ESP32-CAM Photo Gallery</h2>
<?php
  // Image extensions
  $image_extensions = array("png","jpg","jpeg","gif");

  // Check delete HTTP GET request - remove images
  if(isset($_GET["delete"])){
    $imageFileType = strtolower(pathinfo($_GET["delete"],PATHINFO_EXTENSION));
    if (file_exists($_GET["delete"]) && ($imageFileType == "jpg" ||  $imageFileType == "png" ||  $imageFileType == "jpeg") ) {
      echo "File found and deleted: " .  $_GET["delete"];
      unlink($_GET["delete"]);
    }
    else {
      echo 'File not found - <a href="gallery.php">refresh</a>';
    }
  }
  // Target directory
  $dir = './uploads/';
  if (is_dir($dir)){
    echo '<div class="flex-container">';
    $count = 1;
    $files = scandir($dir);
    rsort($files);
    foreach ($files as $file) {
      if ($file != '.' && $file != '..') {?>
        <div>
          <p><a href="?delete=<?php echo $dir . $file; ?>">Delete file</a> - <?php echo $file; ?></p>
          <a href="<?php echo $dir . $file; ?>">
            <img src="<?php echo $dir . $file; ?>" style="width: 350px;" alt="" title=""/>
          </a>
       </div>
<?php
       $count++;
      }
    }
  }
  if($count==1) { echo "<p>No images found</p>"; } 
?>
  </div>
</body>
</html>