<?php
// Lấy thông tin cảnh báo từ server và trả về dưới dạng JSON
$alert = array(
  'message' => 'Alert message!',
  'alert' => date('h:i:s, A',time())
);
echo json_encode($alert);
?>