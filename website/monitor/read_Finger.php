<?php
    require_once('../connection.php');
    // Check if the request method is POST
    header('Content-type: text/html');

    if(isset($_GET['ser_code'])&&isset($_POST['id']) &&isset($_POST['Name']))
    {

        $securitycode = $_GET['ser_code'];
        $id = $_POST['id'];
        $Name = $_POST['Name'];
        $id = json_decode($id);
        $Name = json_decode($Name);
        $sql = "Select * from ref_table where device_code = '$securitycode'";
        $result = mysqli_query($conn,$sql);
        if(mysqli_num_rows($result))
        {
            $row = mysqli_fetch_array($result);
            $ref_Finger = $row['ref_Finger'];
            $sql = "Delete from $ref_Finger";
            $result = mysqli_query($conn,$sql);
            if($result === True)
            {
                echo "Deleted!";
            }
            else
            {
                echo "Failed Delete!";
            }
            for ($i = 0;$i<count($Name);$i++)
            {
                $sql = "INSERT INTO `$ref_Finger` (
                    `id`,
                    `Name`,
                    `username`
                    ) VALUES (
                    '$id[$i]',
                    '$Name[$i]',
                    ''
                    )";
                $result1 = mysqli_query($conn,$sql);
                if($result)
                {
                    echo $id[$i]."\n";
                    echo $Name[$i]."\n";
                }
                else
                {
                    echo "Error: " . $sql . "<br>" . mysqli_error($conn);
                }
            }
        }
        else
        {
            echo 'có lỗi xảy ra vui lòng thử lại';
        }
    }
    $conn->close();
?>