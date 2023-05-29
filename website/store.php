<?php



    require_once('../connection.php');
    if(isset($_POST['type'])&&isset($_POST['ser_code'])&&isset($_POST['action']))
    {
        $type = $_POST['type'];
        $action = $_POST['action'];
        $securitycode = $_POST['ser_code'];
        $sql = "Select * from ref_table where device_code = '$securitycode' ";
        $result = mysqli_query($conn, $sql);
        if(mysqli_num_rows($result)>0)
        {
            $row = mysqli_fetch_array($result);
            
            $ref_Finger = $row['ref_Finger'];
            $ref_RFID = $row['ref_RFID'];

            echo $ref_RFID;
        }
        else
        {
            echo "đã có lỗi xảy ra";
        }
        if($action=='add')
        {

            if($type == 'RFID')
            {
                if(isset($_POST['id'])&&isset($_POST['Name'])&&isset($_POST['RFID_code']))
                {
                    $id = $_POST['id'];
                    $Name = $_POST['Name'];
                    $RFID_code = $_POST['RFID_code'];
                    $sql = "INSERT INTO $ref_RFID (
                        `slot`,
                        `RFID_name`,
                        `RFID_Code`
                        ) VALUES (
                        '$id',
                        '$Name',
                        '$RFID_code'
                        )";
                    if(mysqli_query($conn,$sql))
                    {
                        echo "Tạo thành công";
                    }
                    else
                    {
                        echo "có lỗi xảy ra vui lòng thử lại";
                    }
                } 
            }
            else if($type == 'Finger')
            {
                if(isset($_POST['id'])&&isset($_POST['Name']))
                {
                    $id = $_POST['id'];
                    $Name = $_POST['Name'];
                    
                    $sql = "INSERT INTO $ref_Finger (
                        id,
                        Name,
                        username
                        ) VALUES (
                        '$id',
                        '$Name',
                        ''
                        )";
                    if(mysqli_query($conn,$sql))
                    {
                        echo "Thêm dữ liệu thành công";
                    }
                    else
                    {
                        echo "có lỗi xảy ra vui lòng thử lại";
                    }
                }
            }
        }
        if($action == 'del')
        {
            if($type == 'RFID')
            {
                if(isset($_POST['RFID_code']))
                {
                    $RFID_code = $_POST['RFID_code'];
                    $sql = "delete from $ref_RFID where RFID_Code = '$RFID_code'";
                    if(mysqli_query($conn,$sql))
                    {
                        echo "xóa thẻ RFID: $RFID_code";
                    }
                    else
                    {
                        echo "Có lỗi xảy ra vui lòng thử lại";
                    }
                }
            }
            else if($type == 'Finger')
            {
                if(isset($_POST['id']))
                {
                    $IDFinger = $_POST['IDFinger'];
                    $sql = "delete from $ref_Finger where id = '$IDFinger'";
                    if(mysqli_query($conn,$sql))
                    {
                        echo "xóa vân tay, id: $IDFinger";
                    }
                    else
                    {
                        echo "Có lỗi xảy ra trong quá trình xóa vân tay";
                    }
                }
            }
        }
        
        
    }
   
?>