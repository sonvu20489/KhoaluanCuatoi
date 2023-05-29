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
        <title>History of door </title>
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
            <li>
            <li><a href="#"><i class="fas fa-chevron-up"></i></a></li>
                <li><a href="/history_door/"><i class="fas fa-home"></i></a></li>
                <li><a href="/monitor/"><i class="fas fa-tablet-alt"></i></a></li>
                <li><a href="./"><i class="fas fa-sync"></i></a></li>
                <li><a href="/api/"><i class="fas fa-images"></i></a></li>
                <li><a href="http://khoacuaonline.000webhostapp.com/video_stream/"><i class="fas fa-video"></i></a></li>
            </ul>
        </div>
        <div class="history_table">
            
            <div class="contain">
                
                <h2>LỊCH SỬ RA VÀO CỬA</h2>
                <select name="NoMessage" id = "NoMessage">
                    <option value="">All</option>
                    <option value="25">25</option>
                    <option value="50">50</option>
                    <option value="100">100</option>
                </select>
                <table>
                    <thead>
                        <tr>
                            <td>Time</td>
                            <td>Message</td>
                        </tr>
                    </thead>
                    <tbody>
                    </tbody>
                </table>
                
            </div>
        </div>
        <div class="bell" id="bell">
            🔔
            <div class="message-box hide" id="message-box">
            <!-- Danh sách tin nhắn sẽ được thêm vào đây -->
            </div>
        </div>
        
        
    </body>
    <script type="text/javascript">
        // var source = new EventSource("fetch.php?ser_code=
        // if(isset($_GET['ser_code']))
        // {
        //     $securitycode = $_GET['ser_code'];
        //     echo $securitycode;
        // } 
        // ");
        // console.log(source);
        // source.onmessage=function(event)
        // {
        //     // console.log(event.data);
        //     var arrayData = JSON.parse(event.data);
        //     // console.log(arrayData);
        //     var datacontainer = document.querySelector('tbody');
        //     if(arrayData.success == 1)
        //     {
        //         datacontainer.innerHTML="";
        //         arrayData.receive.forEach(function(element) {
        //                 datacontainer.innerHTML+=`
        //                 <tr>
        //                     <td>${element.Timestamp}</td>
        //                     <td>${element.message}</td>
        //                 </tr>
        //             `;
        //         });
        //     }
        // }
        let select_menu = document.querySelector("#NoMessage");
        let value = ''
        select_menu.addEventListener("change",function(){
            value = this.value;
            
            // let http = new XMLHttpRequest();
            // http.open('POST', 'fetch.php?ser_code=2f2eb859-e213-4cee-909d-c358a59bc9c5');
            // http.setRequestHeader("Content-type","application/x-www-form-urlencoded");
            // http.send('NoMessage='+value);
            // http.onload = function(){
            //     if(this.readyState == 4 && this.status == 200)
            //     {
            //         console.log(this.responseText);
            //         let respone = JSON.parse(this.responseText);
            //         console.log(respone);
            //     }
            // }
        })
        const get_Value = function()
        {    
            let http = new XMLHttpRequest();
            http.open('POST', 'fetch.php?ser_code=<?php 
                echo $securitycode;
            ?>');
            http.setRequestHeader("Content-type","application/x-www-form-urlencoded");
            http.send('NoMessage='+value);
            http.onload = function(){
                if(this.readyState == 4 && this.status == 200)
                {
                    let respone = JSON.parse(this.responseText);
                    var datacontainer = document.querySelector('tbody');
                    if(respone.success == 1)
                    {
                        datacontainer.innerHTML="";
                        respone.receive.forEach(function(element) {
                                datacontainer.innerHTML+=`
                                <tr>
                                    <td>${element.Timestamp}</td>
                                    <td>${element.message}</td>
                                </tr>
                            `;
                        });
                    }
                }
            }
        }
        get_Value();
        setInterval(get_Value,3000);


        function updateAlerts() {
        // Tạo đối tượng XMLHTTPRequest để gửi yêu cầu lấy dữ liệu từ server
        const xhr = new XMLHttpRequest();
        xhr.open("GET", "./fetch1.php");
        xhr.onload = function () {
          if (xhr.status === 200) {
            // Xử lý dữ liệu cảnh báo trả về từ server
            const alertData = JSON.parse(xhr.responseText);
            const Time = alertData.alert;
            const alertMessage = alertData.message;

            // Thêm tin nhắn mới vào danh sách tin nhắn
            const messageBox = document.getElementById('message-box');
            const message = document.createElement("div");
            const messageText = document.createElement("div");
            const messageContent = document.createElement("p");

            message.classList.add("message");
            messageText.classList.add("message-text");
            messageContent.innerText = Time + ":" + alertMessage;

            messageText.appendChild(messageContent);
            message.appendChild(messageText);
            messageBox.appendChild(message);
            messageBox.scrollTop = messageBox.scrollHeight - messageBox.offsetHeight;
            // messageBox.prepend(message);
            // // Hiển thị khung thông báo nếu có tin nhắn mới
            // messageBox.classList.remove("hide");
          }
        };
        xhr.send();
      }

      // Cập nhật các tin nhắn mới nhất sau một khoảng thời gian nhất định
    setInterval(function () {
    updateAlerts();
    }, 5000);
        const bell = document.getElementById("bell");
        const messageBox = document.getElementById("message-box");
        bell.addEventListener("click", function () {
            messageBox.classList.toggle("hide");
        });
    </script>
    <!-- <script src="jsfile.js"></script> -->
</html>