<!DOCTYPE html>
<html>
<head>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.0/jquery.min.js"></script>  
<link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css" integrity="sha384-MCw98/SFnGE8fJT3GXwEOngsV7Zt27NXFoaoApmYm81iuXoPkFOJwJ8ERdknLPMO" crossorigin="anonymous">
<script src="https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/js/bootstrap.min.js" integrity="sha384-ChfqqxuZUCnJSK3+MXmPNIyE6ZbWh2IMqE241rYiqJxyMiZ6OW/JmZQ5stwEULTy" crossorigin="anonymous"></script>
<title>Cycling Thermal (IoT) - Univeritat de Barcelona - Jefferson Moreira</title>
  <style>
    table{
      border: 0px solid green;
      text-align: center;
      border-radius: 4px;
    }
    #header {
      text-align: center;
    }
    #logo{
      text-align: center;
    }
  </style>
    <script type="text/javascript">
            window.onload = function () {
                var dataLength = 0;
                var data1 = [];
                var data2 = [];
                var data3 = [];
                var data4 = [];
                var updateInterval = 2000;
                updateChart();
                function updateChart() {
                    $.getJSON("data.php", function (result) {
                        // if (dataLength !== result.length) {
                            for (var i = dataLength; i < result.length; i++) {
                                data1.push({
                                    x: parseFloat(result[i].time),
                                    y: parseFloat(result[i].valory1)
                                });
                                data2.push({
                                    x: parseFloat(result[i].time),
                                    y: parseFloat(result[i].valory2)
                                });
                                data3.push({
                                    x: parseFloat(result[i].time),
                                    y: parseFloat(result[i].valory3)
                                });
                                data4.push({
                                    x: parseFloat(result[i].time),
                                    y: parseFloat(result[i].valory4)
                                });

                            if (data1.length > dataLength) {data1.shift();}
                            if (data2.length > dataLength) {data2.shift();}
                            if (data3.length > dataLength) {data3.shift();}
                            if (data4.length > dataLength) {data4.shift();}
                            dataLength = result.length;
                            chart.render();
                            }
                        // }
                    });
                }
                // updateChart();
                var chart = new CanvasJS.Chart("chartContainer", {
                    exportEnabled: true,
                    zoomEnabled: true,
                    zoomType: "xy",
                    theme: "light2", //"light1", "light2","dark1", "dark2"
                    // backgroundColor: "#F5DEB3",
                    // culture:  "es",
                    title: {
                        text: "Cycling Thermal (IoT)"
                    },
                    axisX: {
                        includeZero: false,
                        title: "Time",
                        // interval: 5,
                        // intervalType: "minute",
                        gridColor: "lightblue" ,
                        gridDashType: "dot",
                        gridThickness: 2,
                    },
                    axisY: {
                        includeZero: false,
                        title: "Temperature (\xB0C)",
                    },
                    data: [{type: "spline", name: "Hot Furnance", showInLegend: true, xValueType: "dateTime", dataPoints: data1},
                           {type: "spline", name: "Cold Furnance", showInLegend: true, xValueType: "dateTime", dataPoints: data2},
                           {type: "spline", name: "Sample", showInLegend: true, xValueType: "dateTime", dataPoints: data3},
                           {type: "spline", name: "Motor", showInLegend: true, xValueType: "dateTime", dataPoints: data4}],
                    // xValueType: "dataTime", markerSize: 0,
                });
                setInterval(function () {updateChart()}, updateInterval);
                function onoff(){
                    currentvalue = document.getElementById('onoff').value;
                    if(currentvalue == "Off"){
                        document.getElementById("onoff").value="On"; // action="on.php" method="POST";

                    }else{
                        document.getElementById("onoff").value="Off"; //action="off.php" method="POST";
                    }
                }
            }
            // top.window.location.reload();
    </script>

</head>
<body>
<!-- <h1 id="header"> Cycling Thermal (IoT) </h1><br> -->
<!-- validation of credentials -->

    <div id="chartContainer" style="height: 470px; max-width: 1020px; margin: 0px auto;"></div>
    <?php // code php
    ?>
    <!-- <input type="button" value="On" id="onoff" action="on.php" method="POST"> -->
    <script src="https://canvasjs.com/assets/script/jquery-1.11.1.min.js"></script>
    <script src="http://www.tesmaterials.com/ciclado/canvasjs.min.js"></script>
<!-- fin del php -->
<!-- <p id="logo"> <img src="img/LOGO_UB.png" alt="UB" width="90" height="25"> </p>
<p id="logo"> Thesis by Jefferson Moreira from the Department of Electronics of UB.</p> -->
</body>
</html>