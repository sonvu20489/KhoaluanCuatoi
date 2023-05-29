<?php
    session_start();
    if(isset($_SESSION['device'])&&isset($_SESSION['name']))
    {
        header('Location: /history_door');
    }
    ob_start();
?>

<!DOCTYPE html>
<html>
    <head>
        <title>Đăng nhập tài khoản</title>
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
                    <li><a href="/history_door">Quản lý khóa của bạn</a></li>
                    <li><a href="/about-us">Giới thiệu</a></li>
                    <li><a href="/Contact">Liên hệ</a></li>
                </ul>
                <div>
            </div>
        </div>
        <form action="" method="POST" class="form" id="form-2">
            <p style="font-size: 24px">Đăng nhập để giám sát hệ thống</p>
            <div class="form-group">
                <Label for="input-username"><b>Username</b></Label>
                <div class="input-field">
                    <input class ="form-control" type="text" placeholder="Enter your username" name="input-username" id="input-username">
                </div>
                
                <span class="form-message"></span>
            </div>

            <div class="form-group">
                <Label for="input-password"><b>Password</b></Label>
                <div class="input-field">
                    <input class ="form-control" type="password" placeholder="Enter your password" name="input-password" id = "input-password">
                </div>
                
                <span class="form-message"></span>
            </div>
            <a style = "text-align:unset" href="../reset-password">Forgot your password? </a>
            <br>
            <a style="text-align:unset" href="../register/">Don't have account? Register?</a>
            <?php
            require_once("../connection.php");
            if (isset($_POST["btn_submit"]))
            {
                $username = $_POST["input-username"];
                $password = $_POST["input-password"];
                //làm sạch thông tin, xóa bỏ các tag html, ký tự đặc biệt 
                //mà người dùng cố tình thêm vào để tấn công theo phương thức sql injection
                $username = strip_tags($username);
                $username = addslashes($username);
                $password = strip_tags($password);
                $password = addslashes($password);
                if ($username == "" || $password =="") {
                    echo "<p style = \" color: red \" >username hoặc password bạn không được để trống! </p>";
                }
                else {
                    $sql = "select * from user_infor where username = '$username'";
                    $result = mysqli_query($conn, $sql);
                    if(mysqli_num_rows($result))
                    {
                        $row = mysqli_fetch_array($result);
                        $_SESSION['name'] = $row['name'];
                        $_SESSION['device'] = $row['device'];
                        $_SESSION['username'] = $row['username'];
                        header('Location: /history_door');
                    }
                    else
                    {
                        echo "<p style = \" color: red \" >username or password is wrong </p>";
                    }
                }
            }
            ob_flush();

        ?>
            <button class="form-submit" name="btn_submit">Đăng nhập</button>
        </form>
        
    </body>
</html>