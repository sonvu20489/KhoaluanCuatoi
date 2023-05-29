<?php
$server_username = "id20314831_admin";
$server_password = "Khoacuaaninh123@@";
$server_host = "localhost";
$database = 'id20314831_dbcuaanninh';

$conn = mysqli_connect($server_host,$server_username,$server_password,$database) or die("không thể kết nối tới database");
mysqli_query($conn,"SET NAMES 'UTF8'");
