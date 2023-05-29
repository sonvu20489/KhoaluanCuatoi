<?php
    $securitycode = '';
    if(isset($_GET['ser_code']))
    {
        $securitycode = $_GET['ser_code'];
    }
?>
<html>
    <head>
        <title>Test history</title>
    </head>
    <body>
        <select name="NoMessage" id = "NoMessage">
            <option value="">All</option>
            <option value="1">25</option>
            <option value="2">50</option>
            <option value="3">100</option>
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
            http.open('POST', 'fetch.php?ser_code=<?php if(isset($_GET['ser_code']))
            {
                $securitycode = $_GET['ser_code'];
                echo $securitycode;
            }?>');
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
    </script>
    <!-- <script src="jsfile.js"></script> -->
</html>