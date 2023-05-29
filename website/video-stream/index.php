
<?php
    session_start();
    $securitycode = '';
    if(isset($_SESSION['device'])&&isset($_SESSION['name']))
    {
        $securitycode = $_SESSION['device'];
        // $substr_sercode = substr($securitycode,-6);
        $nameacc = $_SESSION['name'];
    }
    else
    {
        echo "failed";
        header('Location: /login');
    }
?>
<html>
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <title>ESP32 CAM Video Stream</title>
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
                    <li><a class = "select" href="/history_door">Quản lý khóa của bạn</a></li>
                    <li><a href="/about-us">Giới thiệu</a></li>
                    <li><a href="/Contact">Liên hệ</a></li>
                </ul>
                <div>
                <div class="user_info">
                    <span> Xin chào <?php echo $nameacc?></span>
                    <ul>
                        <li><a href="/modify-pass/">Đổi mật khẩu</a></li>
                        <li><a href="/login/logout.php">Đăng xuất</a></li>
                    </ul>
                </div>
            </div>
        </div>
        <div class="nav-control">
            <ul>
                <li><a href="#"><i class="fas fa-chevron-up"></i></a></li>
                <li><a href="/history_door/"><i class="fas fa-home"></i></a></li>
                <li><a href="/monitor/"><i class="fas fa-tablet-alt"></i></a></li>
                <li><a href="./"><i class="fas fa-sync"></i></a></li>
                <li><a href="/api/"><i class="fas fa-images"></i></a></li>
                <li><a href="http://khoacuaonline.000webhostapp.com/video_stream/"><i class="fas fa-video"></i></a></li>
            </ul>
        </div>
        <div id="stream-container" class="image-container">
            <h1>VIDEO CAM</h1>
            <span>Sử dụng cùng mạng wifi với WiFi được kết nối với ESP32 CAM để xem</span>
            <?php
                require_once('../connection.php');
                $sql = "Select * from ref_table where device_code='$securitycode'";
                $result = mysqli_query($conn,$sql);
                if(mysqli_num_rows($result))
                {
                    $row = mysqli_fetch_array($result);
                    $stream_url = $row['ref_stream'];
                }
                else
                {
                    echo "Có lỗi xảy ra vui lòng thử lại";
                }
            ?>
            <iframe id="stream" src="http://<?php echo $stream_url?>" width = "100%" height="100%">
        </div>
    </body>
    
</html>