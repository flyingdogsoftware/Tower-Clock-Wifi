const char HTML_TOP[]  = R"=====(
<html>
<meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
<head>
    <title>Tower Clock</title>
    <style>
        body {
                font-family: 'Spectral SC', serif;
                background-color: #72281d;
                color:white;    /* ff5940 */
                text-align:center;            
        }
        .tower {
            max-width:500px;
        }
        .direction {
            display: inline-block;
            border: 1px solid white;
            border-radius: 100%;
            padding: 5px;
            fill:white;
            cursor: pointer;
        }
        .direction:hover {
            background-color: white;
            fill:  #72281d;
        }
        .start_clock {
            fill: white;
            cursor: pointer;  
            display: inline-block;
        }
        .clock {
            width:36px;
            height:36px;
            vertical-align: -5px;
        }

        .space {
            margin-right:20px;
        }
        .set_mode {
            border: 1px solid white;
            color:white;
            text-decoration: none;
            padding: 5px;
            margin-right: 20px;
            border-radius: 5px;
        }
        .set_mode:hover {
            background-color:white;
            color: #72281d;
        }
    </style>
    <link rel="preconnect" href="https://fonts.gstatic.com">
    <link href="https://fonts.googleapis.com/css2?family=Spectral+SC:wght@200&display=swap" rel="stylesheet"> 
</head>
<body>


<h1>Tower-Clock</h1>
<a href="/up.html" class="direction space">
    <svg width="24" height="24" xmlns="http://www.w3.org/2000/svg" fill-rule="evenodd" clip-rule="evenodd"><path d="M11 2.206l-6.235 7.528-.765-.645 7.521-9 7.479 9-.764.646-6.236-7.53v21.884h-1v-21.883z"/></svg>
</a>
<a href="/right.html" class="direction space">
    <svg width="24" height="24" xmlns="http://www.w3.org/2000/svg" fill-rule="evenodd" clip-rule="evenodd"><path d="M21.883 12l-7.527 6.235.644.765 9-7.521-9-7.479-.645.764 7.529 6.236h-21.884v1h21.883z"/></svg>
</a>
<a href="/down.html" class="direction space">
    <svg width="24" height="24" xmlns="http://www.w3.org/2000/svg" fill-rule="evenodd" clip-rule="evenodd"><path d="M11 21.883l-6.235-7.527-.765.644 7.521 9 7.479-9-.764-.645-6.236 7.529v-21.884h-1v21.883z"/></svg>
</a>
<a href="/left.html" class="direction space">
    <svg width="24" height="24" xmlns="http://www.w3.org/2000/svg" fill-rule="evenodd" clip-rule="evenodd"><path d="M2.117 12l7.527 6.235-.644.765-9-7.521 9-7.479.645.764-7.529 6.236h21.884v1h-21.883z"/></svg>
</a>
<a href="/start.html" class="start_clock">
    <svg xmlns="http://www.w3.org/2000/svg" class="clock" width="24" height="24" viewBox="0 0 24 24"><path d="M11 6v8h7v-2h-5v-6h-2zm10.854 7.683l1.998.159c-.132.854-.351 1.676-.652 2.46l-1.8-.905c.2-.551.353-1.123.454-1.714zm-2.548 7.826l-1.413-1.443c-.486.356-1.006.668-1.555.933l.669 1.899c.821-.377 1.591-.844 2.299-1.389zm1.226-4.309c-.335.546-.719 1.057-1.149 1.528l1.404 1.433c.583-.627 1.099-1.316 1.539-2.058l-1.794-.903zm-20.532-5.2c0 6.627 5.375 12 12.004 12 1.081 0 2.124-.156 3.12-.424l-.665-1.894c-.787.2-1.607.318-2.455.318-5.516 0-10.003-4.486-10.003-10s4.487-10 10.003-10c2.235 0 4.293.744 5.959 1.989l-2.05 2.049 7.015 1.354-1.355-7.013-2.184 2.183c-2.036-1.598-4.595-2.562-7.385-2.562-6.629 0-12.004 5.373-12.004 12zm23.773-2.359h-2.076c.163.661.261 1.344.288 2.047l2.015.161c-.01-.755-.085-1.494-.227-2.208z"/></svg>
</a>
<br><br>
Set Mode:<br><br>
<a href="/set_mode_second.html" class="set_mode">Second</a><a href="/set_mode_minute.html" class="set_mode">Minute</a><a href="/set_mode_hour.html" class="set_mode">Hour + Minute</a>
<br><br>
<a href="/set_mode_crazy.html" class="set_mode">Crazy!</a>
<br><br>

)=====";


const char HTML_BOTTOM[]= R"=====(
    <br><img src="https://upload.wikimedia.org/wikipedia/commons/a/ae/World_landmarks_icons_-_Big_Ben.svg" class="tower">
    <br><br>
    <a href="/reset_wifi.html" class="set_mode">Reset Wifi</a>
  </body>
)=====";
