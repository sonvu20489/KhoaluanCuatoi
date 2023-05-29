<!DOCTYPE html>
<html>
    <head>
        <title>Đăng ký Sản phẩm</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="shortcut icon" href="/C-File/logo.png" type="image/x-icon">
        <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.1/css/all.min.css" integrity="sha512-+4zCK9k+qNFUR5X+cKL9EIR+ZOhtIloNl9GIKS57V1MyNsYpYcUrUeQc9vNfzsWfV28IaLL3i96P9sdNyeRssA==" crossorigin="anonymous" />
        <link rel="stylesheet" href="./assets/css/style.css">
        <link rel="stylesheet" href="./assets/css/css-toast.css">
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
        <div id="toast"></div>
        
        <form action="" method="POST" id = "form-1">
            <div>
                <p style="font-size: 24px">Đăng ký tài khoản thiết bị của bạn</p>
                <div class="form-group">
                <Label for="input-name"><b>Account name*</b></Label>
                <div class="input-field">
                    <input class ="form-control" type="text" placeholder="type your name" name="input-name" id="input-Accname">
                </div>
                
                <span class="form-message"></span>
                </div>

                <div class="form-group">
                    <Label for="input-email"><b>Your email</b></Label>
                    <div class="input-field">
                        <input class ="form-control" type="text" placeholder="Please type your email" name ="input-email" id="input-email">
                    </div>
                    <span class="form-message"></span>
                </div>

                <div class="form-group">
                    <Label for="input-username"><b>Username*</b></Label>
                    <div class="input-field">
                        <input class ="form-control" type="text" placeholder="6-10 characters" name="input-username" id="input-username">
                    </div>
                    
                    <span class="form-message"></span>
                </div>

                <div class="form-group">
                    <Label for="input-password"><b>Password*</b></Label>
                    <div class="input-field">
                        <input class ="form-control" type="password" placeholder="8-16 characters" name="input-password" id = "input-password">
                        <div class = "btnshow">
                            <i class="far fa-eye"></i>
                        </div>
                    </div>
                    
                    <span class="form-message"></span>
                </div>

                <div class="form-group">
                    <Label for="input-repass"><b>Retype your password*</b></Label>
                    <div class="input-field">
                        <input class ="form-control" type="password" placeholder="8-16 characters" name="input-repass" id="input-repass">
                        <div class = "btnshow">
                            <i class="far fa-eye"></i>
                        </div>
                    </div>
                    <span class="form-message"></span>
                </div>

                <div class="form-group">
                <lable for="input-idproduct"><b>Type your id prod*</b></lable>
                <div class="input-field">
                    <input class ="form-control" type="password" placeholder="code on the product" name="input-idproduct" id ="input-idprod">
                    <div class = "btnshow">
                        <i class="far fa-eye"></i>
                    </div>
                </div>
                
                <span class="form-message"></span>
                </div>

                <p style="color:red">* is required</p>
                <button class="form-submit"name="btn_submit">Đăng ký</button>
                <div id = "messageresponse">
                </div>
                <script type="text/javascript" src="./assets/js/Toast-mes.js"></script>
                <script type="text/javascript" src="./assets/js/validation.js"></script>
                <script>

                document.addEventListener('DOMContentLoaded', function () {
                    // Mong muốn của chúng ta
                    Validator({
                    form: '#form-1',
                    formGroupSelector: '.form-group',
                    errorSelector: '.form-message',
                    rules: [
                        Validator.isRequired('#input-Accname', 'Vui lòng nhập tên của bạn'),
                        Validator.isEmail('#input-email'),
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
                if(isset($_POST["btn_submit"]))
                {
                    $name = $_POST["input-name"];
                    $email = $_POST["input-email"];
                    $user = $_POST["input-username"];
                    $password = $_POST["input-password"];
                    $repass = $_POST["input-repass"];
                    $id = $_POST["input-idproduct"];
                    if($name == ""||$user==""||$password==""||$repass=="" || $id == "")
                    {
                        echo "<p style = \"color: red;\"> Vui lòng nhập đầy đủ thông tin</p>";
                    }
                    else
                    {
                        $sql = "select * from user_infor where username = '$user'";
                        $check=mysqli_query($conn, $sql);
                        if(mysqli_num_rows($check)>0)
                        {
                            echo "<p style = \"color: red;\"> Tài khoản đã tồn tại vui lòng nhập tài khoản khác</p>";
                            echo '<script type="text/javascript">showFailedToast()</script>';
                        }
                        else
                        {
                            if($password != $repass)
                            {
                                echo "<p style = \"color: red;\"> 2 mật khẩu đã nhập không trùng khớp</p>";
                            }
                            else
                            {
                                $sql = "select * from device_table where device_code = '$id' and available = 1";
                                $check = mysqli_query($conn, $sql);
                                if(mysqli_num_rows($check)==0)
                                {
                                    echo "<p style = \"color: red;\"> Thiết bị đã được đăng ký bằng tk khác hoặc không tồn tại</p>";
                                    echo "<script type=\"text/javascript\">showFailedToast1()</script>";
                                }
                                else
                                {
                                    $sql = "update device_table SET available = 0 where device_code = '$id'";
                                    if(mysqli_query($conn,$sql)===TRUE)
                                    {
                                        $sql = "select * from device_table where device_code='$id'";
                                        $result = mysqli_query($conn,$sql);
                                        if($result)
                                        {
                                            $row = mysqli_fetch_array($result);
                                            $id_device = $row['id'];
                                            $sub_str = substr($id,-6);
                                            $dir = "/api/$sub_str";
                                            mkdir($dir,0777);
                                            $historytbname = "history$id_device";
                                            $RFIDtbname = "RFID$id_device";
                                            $Fingertbname = "Finger$id_device";
                                            $sql = "CREATE TABLE IF NOT EXISTS `$Fingertbname` (
                                                `id` int(11) NOT NULL,
                                                `Name` varchar(10) COLLATE utf8_unicode_ci NOT NULL,
                                                `username` varchar(255) COLLATE utf8_unicode_ci NOT NULL
                                              ) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Lưu thông tin Finger ở trong hệ thôgns';
                                            ";
                                            mysqli_query($conn,$sql);
                                            $sql = "CREATE TABLE IF NOT EXISTS `$historytbname` (
                                                `id` int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
                                                `message` varchar(255) COLLATE utf8_unicode_ci NOT NULL,
                                                `Timestamp` datetime NOT NULL
                                              ) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='lưu thông tin mở cửa, cảnh báo';
                                            ";
                                            mysqli_query($conn,$sql);
                                            $sql = "CREATE TABLE IF NOT EXISTS `$RFIDtbname` (
                                                `slot` int(11) NOT NULL,
                                                `RFID_name` varchar(10) COLLATE utf8mb4_unicode_ci NOT NULL,
                                                `RFID_Code` longtext CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL CHECK (json_valid(`RFID_Code`))
                                              ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='Lưu thông tin thẻ RFID ở trong hệ thống';
                                            ";
                                            mysqli_query($conn,$sql);
                                            $sql = "INSERT INTO ref_table (
                                                device_code,
                                                ref_history,
                                                ref_RFID,
                                                ref_Finger,
                                                ref_stream,
                                                )VALUES(
                                                '$id',
                                                '$historytbname',
                                                '$RFIDtbname',
                                                '$Fingertbname',
                                                ''
                                                )";
                                            if(mysqli_query($conn,$sql))
                                            {
                                                echo 'Tạo bảng thành công';
                                            }
                                            else
                                            {
                                                echo "Tạo bảng thất bại";
                                            }
                                        }
                                        $sql = "INSERT INTO user_infor(
                                            name,
                                            username,
                                            password,
                                            email,
                                            device
                                            ) VALUES (
                                            '$name',
                                            '$user',
                                            '$password',
                                            '$email',
                                            '$id'
                                            )";
                                        if(mysqli_query($conn,$sql))
                                        {
                                            echo '<p style = "color: red;">Đã cập nhật thiết bị cho tài khoản';
                                            echo '<script type="text/javascript"> showSuccessToast()</script>';
                                            
                                        }
                                        else {
                                            echo "Error: " . $sql . "<br>" . mysqli_error($connect);
                                        }
                                    }
                                    else
                                    {
                                        echo 'Không thể cập nhật mã thiết bị';
                                    }
                                }
                            }
                        }
                    }
                }
                $conn->close();
                ?>
            </div>
        </form>
        
    </body>
</html>