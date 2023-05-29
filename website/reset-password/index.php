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
        <title>Trang Khôi phục tài khoản</title>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="shortcut icon" href="/C-File/logo.png" type="image/x-icon">
        <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.1/css/all.min.css" integrity="sha512-+4zCK9k+qNFUR5X+cKL9EIR+ZOhtIloNl9GIKS57V1MyNsYpYcUrUeQc9vNfzsWfV28IaLL3i96P9sdNyeRssA==" crossorigin="anonymous" />
        <link rel="stylesheet" href="./assets/css/style.css">
        <link rel="stylesheet" href="./assets/css/topnav.css">
        <link rel="stylesheet" href="./assets/css/responsive.css">
        <script src="./assets/js/javascript.js"></script>
        <style>
            .form-group.invalid .form-control {
                border-color: #f33a58;
            }

            .form-group.invalid .form-message {
                color: #f33a58;
            }
        </style>
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
        <form action="" method="POST" class="form" id = "form-1">
            <p style="font-size: 24px">Khôi phục tài khoản cho thiết bị của bạn</p>
            <div class="form-group">
                <Label for="input-username"><b>Username</b></Label>
                <div class="input-field">
                    <input class ="form-control" type="text" placeholder="Enter your username" name="input-username" id="input-username">
                </div>
                <span class="form-message"></span>
            </div>
            <div class="form-group">
                <Label for="input-password"><b>New Password</b></Label>
                <div class="input-field">
                    <input class ="form-control" type="password" placeholder="8-16 characters" name="input-password" id = "input-password">
                    <div class = "btnshow">
                        <i class="far fa-eye"></i>
                    </div>
                </div>
                
                <span class="form-message"></span>
            </div>
            <div class="form-group">
                <Label for="input-repass"><b>Confirm Password</b></Label>
                <div class="input-field">
                    <input class ="form-control" type="password" placeholder="8-16 characters" name="input-repass" id="input-repass">
                    <div class = "btnshow">
                        <i class="far fa-eye"></i>
                    </div>
                </div>
                <span class="form-message"></span>
            </div>
            <div class="form-group">
                <lable for="input-idproduct"><b>Type your id prod</b></lable>
                <div class="input-field">
                    <input class ="form-control" type="password" placeholder="code on the product" name="input-idproduct" id ="input-idprod">
                    <div class = "btnshow">
                        <i class="far fa-eye"></i>
                    </div>
                </div>
                
                <span class="form-message"></span>
            </div>
            
            <script type="text/javascript" src="./assets/js/validation.js"></script>
            <script>

            document.addEventListener('DOMContentLoaded', function () {
                // Mong muốn của chúng ta
                Validator({
                form: '#form-1',
                formGroupSelector: '.form-group',
                errorSelector: '.form-message',
                rules: [
                    Validator.minLength('#input-username', 6),
                    Validator.minLength('#input-password', 8),
                    Validator.isRequired('#input-repass'),
                    Validator.isRequired('#input-idprod',"Vui lòng nhập mã thiết bị của bạn"),
                    Validator.isConfirmed('#input-repass', function () {
                    return document.querySelector('#form-1 #input-password').value;
                    }, 'Mật khẩu nhập lại không chính xác')
                ],
                onSubmit: ''
                });
            });
            

            </script>
            <?php
                require_once('../connection.php');
                if(isset($_POST['btn_submit']))
                {
                    $user = $_POST["input-username"];
                    $password = $_POST["input-password"];
                    $id = $_POST["input-idproduct"];
                    $sql = "SELECT * FROM user_infor where username = '$user' AND device = '$id'";
                    $result = mysqli_query($conn,$sql);
                    if(mysqli_num_rows($result)>0)
                    {
                        $row = mysqli_fetch_array($result);
                        $db_password = $row['password'];
                        if($password == $db_password)
                        {
                            echo "<p style = \"color: red;\">mật khẩu đã nhập trùng với mật khẩu cũ! vui lòng nhập mật khẩu khác</p>";
                        }
                        else
                        {
                            $sql = "Update user_infor SET password = '$password' where username = '$user'";
                            if(mysqli_query($conn,$sql) === TRUE)
                            {
                                echo "<p style = \"color: red;\">Mật khẩu của bạn đã được cập nhật</p>";
                            }
                            else
                            {
                                echo "<p style = \"color: red;\">Có lỗi xảy ra vui lòng thử lại";
                            }
                        }
                        
                    }
                    else
                    {
                        echo "<p style = \"color: red;\"> Product id sai hoặc tài khoản không tồn tại</p>";
                    }
                }
            ?>
            <button class="form-submit"name="btn_submit">Khôi phục ngay</button>
        </form>
    </body>
</html>