static const char INDEX_HTML[] PROGMEM = R"rawliteral(
<html>

  <head>
  
    <title>ESP32-CAM Stream Web Server</title>
    
    <meta name="viewport" content="width=device-width, initial-scale=1">
    
    <style>
    
      body { font-family: Arial; text-align: center; margin:0px auto; padding-top: 30px;}

      /* ----------------------------------- Toggle Switch */
      .switch {
        position: relative;
        display: inline-block;
        width: 90px;
        height: 34px;
        top: 10px;
      }

      .switch input {display:none;}

      .slider {
        position: absolute;
        cursor: pointer;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background-color: #D3D3D3;
        -webkit-transition: .4s;
        transition: .4s;
        border-radius: 34px;
      }

      .slider:before {
        position: absolute;
        content: "";
        height: 26px;
        width: 26px;
        left: 4px;
        bottom: 4px;
        background-color: #f7f7f7;
        -webkit-transition: .4s;
        transition: .4s;
        border-radius: 50%;
      }

      input:checked + .slider {
        background-color: #5b9dd8;
      }

      input:focus + .slider {
        box-shadow: 0 0 1px #2196F3;
      }

      input:checked + .slider:before {
        -webkit-transform: translateX(26px);
        -ms-transform: translateX(26px);
        transform: translateX(55px);
      }

      .slider:after {
        content:'OFF';
        color: white;
        display: block;
        position: absolute;
        transform: translate(-50%,-50%);
        top: 50%;
        left: 70%;
        font-size: 10px;
        font-family: Verdana, sans-serif;
      }

      input:checked + .slider:after {  
        left: 25%;
        content:'ON';
      }
      /* ----------------------------------- */

      /* ----------------------------------- button1 / Capture Button */
      .button1 {
        display: inline-block;
        padding: 10px 20px;
        font-size: 14px;
        cursor: pointer;
        text-align: center;
        text-decoration: none;
        outline: none;
        color: #fff;
        background-color: #4CAF50;
        border: none;
        border-radius: 30px;
      }

      .button1:hover {background-color: #3e8e41}

      .button1:active {
        transform: scale(0.9,0.9)
      }

      .button1:disabled {
        opacity: 0.6;
        cursor: not-allowed;
        pointer-events: none;
      }
      /* ----------------------------------- */

      /* ----------------------------------- button2 / Stream Button */
      .button2 {
        display: inline-block;
        padding: 10px 20px;
        font-size: 14px;
        cursor: pointer;
        text-align: center;
        text-decoration: none;
        outline: none;
        color: #fff;
        background-color: #935cfb;
        border: none;
        border-radius: 30px;
      }

      .button2:hover {background-color: #7c38fa}

      .button2:active {
        transform: scale(0.9,0.9)
      }

      .button2:disabled {
        opacity: 0.6;
        cursor: not-allowed;
        pointer-events: none;
      }
      /* ----------------------------------- */

      /* ----------------------------------- Stream and Capture Viewer */
      img {
        width: auto ;
        max-width: 100% ;
        height: auto ; 
      }
      /* ----------------------------------- */
      
    </style>
    
  </head>
  
  <body>
    
    <img src="" id="photo">
    
    <br><br>
    
    <span style="font-size:15;">LED Flash : &nbsp;</span>
    
    <label class="switch">
      <input type="checkbox" id="togLEDFlash" onclick="LEDFlash()">
      <div class="slider round"></div>
    </label>
    
    <br><br>
    
    <button class="button2" onclick="viewer('stream')" id="Strm">Stream</button>

    <br><br>
    
    <p id="st"></p>
    
    <script>
      /* ----------------------------------- Calls the video stream link and displays it */ 
      window.onload = document.getElementById("photo").src = window.location.href.slice(0, -1) + ":81/stream";
      document.getElementById("Strm").disabled = true;
      send_cmd("stream");
      /* ----------------------------------- */

      /* ----------------------------------- Information text to be displayed when saving image to MicroSD card */
      var myTmr;
      var itext = 31;
      let text = "Saving pictures to MicroSD card...";
      var valTaTephotostate = 10;
      /* ----------------------------------- */

      /* :::::::::::::::::::::::::::::::::::::::::::::::: Function to send commands to turn on or turn off the LED Flash */
      function LEDFlash() {
        var tgLEDFlash = document.getElementById("togLEDFlash");
        var tgState;
        if (tgLEDFlash.checked == true) tgState = 1;
        if (tgLEDFlash.checked == false) tgState = 0;
        send_cmd(tgState);
      }
      /* :::::::::::::::::::::::::::::::::::::::::::::::: */

      /* :::::::::::::::::::::::::::::::::::::::::::::::: Function to choose to show streaming video or captured image */
      function viewer(x) {
        if (x == "capture") {
          document.getElementById("Cptr").disabled = true;
          send_cmd(x);
          window.stop();
          document.getElementById("photo").src = window.location.href.slice(0, -1) + ":81/capture";
          textSaveToMicroSD();
          start_timer();
        } 
        if (x == "stream") {
          window.location.reload();
        }
      }
      /* :::::::::::::::::::::::::::::::::::::::::::::::: */

      /* :::::::::::::::::::::::::::::::::::::::::::::::: Function for sending commands */
      function send_cmd(cmds) {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/action?go=" + cmds, true);
        xhr.send();
      }
      /* :::::::::::::::::::::::::::::::::::::::::::::::: */

      /* :::::::::::::::::::::::::::::::::::::::::::::::: Start and stop the timer */
      function start_timer() {
        myTmr = setInterval(myTimer, 1000)
      }
      
      function stop_timer() {
        clearInterval(myTmr)
      }
      /* :::::::::::::::::::::::::::::::::::::::::::::::: */

      /* :::::::::::::::::::::::::::::::::::::::::::::::: Timer to check image saving process to MicroSD card */
//      function myTimer() {
//        getTakephotostate();
//        textSaveToMicroSD();
//        if (valTaTephotostate == 0) {
//          document.getElementById("st").innerHTML = "";
//          document.getElementById("Strm").disabled = false;
//          stop_timer();
//        }
//      }
      /* :::::::::::::::::::::::::::::::::::::::::::::::: */

      /* :::::::::::::::::::::::::::::::::::::::::::::::: Function to display text information when saving image to MicroSD card */
//      function textSaveToMicroSD() {
//        itext++;
//        if(itext>34) itext = 31;
//        let result = text.substring(0, itext);
//        document.getElementById("st").innerHTML = result;
//      }
      /* :::::::::::::::::::::::::::::::::::::::::::::::: */

      /* :::::::::::::::::::::::::::::::::::::::::::::::: Function to receive the process status of saving images to MicroSD card */
//      function getTakephotostate() {
//        var xhttp = new XMLHttpRequest();
//        xhttp.onreadystatechange = function() {
//          if (this.readyState == 4 && this.status == 200) {
//            valTaTephotostate = this.responseText;
//          }
//        };
//        xhttp.open("GET", "/takephotostate", true);
//        xhttp.send();
//      }
      /* :::::::::::::::::::::::::::::::::::::::::::::::: */
    </script>
  
  </body>
  
</html>
)rawliteral";
