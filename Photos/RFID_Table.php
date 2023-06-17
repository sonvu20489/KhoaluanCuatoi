<?php
    require_once('../connection.php');
    // Check if the request method is POST
    if(isset($_GET['ser_code']))
    {
        $securitycode = $_GET['ser_code'];
        if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    
            // Get the JSON data from the request body
            $json = file_get_contents('php://input');
            
            // Decode the JSON data
            $data = json_decode($json,true);
            $rfid = $data['RFID'];
            $slot = $data['slot'];
            $Name = $data['Name'];
            $RFID = array();
            for($i = 0;$i<count($Name);$i++)
            {
                array_push($RFID,json_encode($rfid[$i]));
            }
            // Handle the JSON data
            // For example, you can print it out
          //   print_r($data);
            $sql = "Select * from ref_table where device_code = '$securitycode'";
            $result = mysqli_query($conn,$sql);
            if(mysqli_num_rows($result))
            {
                $row = mysqli_fetch_array($result);
                $ref_RFID = $row['ref_RFID'];
                for ($i = 0;$i<count($Name);$i++)
                {
                    $sql = "INSERT INTO `$ref_RFID` (
                        `slot`,
                        `RFID_name`,
                        `RFID_code`
                        ) VALUES (
                        '$slot[$i]',
                        '$Name[$i]',
                        '$RFID[$i]'
                        )";
                    $result1 = mysqli_query($conn,$sql);
                    if($result)
                    {
                        echo $slot[$i]."\n";
                        echo $Name[$i]."\n";
                        echo $RFID[$i]."\n";
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
    }
    $conn->close();
?>