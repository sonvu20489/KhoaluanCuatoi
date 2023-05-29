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
        header('Location: /login');
    }
?>
<html>
    <head>
    <title> ESP Database </title>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="shortcut icon" href="/C-File/logo.png" type="image/x-icon">
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
        <?php 
            require_once('../connection.php');
            $sql = "SELECT * FROM ref_table where device_code = '$securitycode'";
            $result = mysqli_query($conn,$sql);
            if(mysqli_num_rows($result))
            {
                $row = mysqli_fetch_array($result);
                $ref_RFID = $row['ref_RFID'];
                $ref_Finger = $row['ref_Finger'];
            }
            else
            {
                echo "Có lỗi xảy ra vui lòng thử lại";
            }
        ?>
        <div class="container">
            <div class="RFID_table">
                <h2>Dữ liệu RFID </h2>
                <table>
                    <thead>
                        <tr>
                            <td>slot</td>
                            <td>Name</td>
                            <td>RFID Card</td>
                        </tr>
                    </thead>
                    <tbody>
                        <?php
                            $sql = "SELECT * FROM $ref_RFID ORDER BY slot ASC";
                            $result = mysqli_query($conn, $sql);
                            if($result)
                            {
                                if(mysqli_num_rows($result))
                                {
                                    
                                    while ($row = $result->fetch_assoc())
                                    {
                                        $slot = $row['slot'];
                                        $Name = $row['RFID_name'];
                                        $RFID = $row['RFID_Code'];
                                        echo '<tr>'
                                                .'<td> ' . $slot . '</td>'
                                                .'<td> ' . $Name . '</td>'
                                                .'<td> ' . $RFID . '</td>'
                                                . '</tr>'; 
                                                
                                    }
                                }
                                else
                                {
                                    '<tr><td colspan = "3">Không có data trên cơ sở dữ liệu</td></tr>';
                                }
                                $result->free();
                            }      
                        ?>
                    </tbody>
                </table>
            </div>
            
            <div class="Finger_Table">
                <h2>Dữ liệu RFID </h2>
                <table>
                    <thead>
                        <tr>
                            <td>id</td>
                            <td>Name</td>
                        </tr>
                    </thead>
                    <tbody>
                        <?php
                            $sql = "SELECT * FROM $ref_Finger ORDER BY id ASC";
                            $result = mysqli_query($conn, $sql);
                            if($result)
                            {
                                if(mysqli_num_rows($result))
                                {
                                    
                                    while ($row = $result->fetch_assoc())
                                    {
                                        $id = $row['id'];
                                        $Name = $row['Name'];
                                        echo '<tr>'
                                                .'<td> ' . $id . '</td>'
                                                .'<td> ' . $Name . '</td>'
                                                . '</tr>'; 
                                                
                                    }
                                }
                                else
                                {
                                    '<tr><td colspan = "3">Không có data trên cơ sở dữ liệu</td></tr>';
                                }
                                $result->free();
                            }      
                        ?>
                    </tbody>
                </table>
            </div>
        </div>
    </body>
</html>