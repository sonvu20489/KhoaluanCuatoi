<?php
    error_reporting(0);
    header("Cache-control: no-store");
    header("Content-type: text/event-stream");
    require_once('../connection.php');
    $p = '';
    $respone = array();
    if(isset($_GET['ser_code']))
    {
        $sercuritycode = $_GET['ser_code'];
        $sql = "select * from ref_table where device_code = '$sercuritycode'";
        $result = mysqli_query($conn,$sql);
        if($result->num_rows>0)
        {
            $row = mysqli_fetch_array($result);
            $ref_history = $row['ref_history'];
            //while(true)
            //{
                if(isset($_POST['NoMessage']))
                {
                    $NoMes = $_POST['NoMessage'];
                    if($NoMes != '')
                        $result = $conn->query("select * from $ref_history ORDER BY Timestamp DESC limit $NoMes");
                    else
                    $result = $conn->query("select * from $ref_history ORDER BY Timestamp DESC");
                }
             
                else
                    $result = $conn->query("select * from $ref_history ORDER BY Timestamp DESC");
                $r = array();
                if($result->num_rows > 0)
                {
                    while($row = $result->fetch_assoc())
                    {
                        $param = array();
                        $param['message'] = $row['message'];
                        $param['Timestamp'] = $row['Timestamp'];
                        
                        // $param['Timestamp'] = date("Y-m-d H:i:s", strtotime($row['Timestamp'] . " + 11 hours"));
                        array_push($r,$param);
                    }
                    $respone['success'] = 1;
                    $respone['receive'] = array();
                    $respone['receive'] = $r;
                    $n = json_encode($respone);
                // if(strcmp($p,$n)!= 0)
                // {
                    echo $n . "\n\n";
                }
                else
                {
                    $respone['success'] = 0;
                    $respone['message'] = 'The table is wrong or not exist';
                    echo json_encode($respone);
                }
                
                
                //     $p = $n;
                // }
                // ob_end_flush();
                // flush();
                // sleep(1);
            //}
        }
        else
        {
            $respone['success'] = 0;
            $respone['message'] = 'sercurity code is wrong or not exist';
            echo json_encode($respone);
        }
    }
    else
    {
        $respone['success'] = 0;
        $respone['message'] = 'Not enough param to query';
        echo json_encode($respone);
    }
?>