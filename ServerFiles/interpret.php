<!DOCTYPE html>
<html>
<meta http-equiv="refresh" content="5; URL=http://www.tesmaterials.com/ciclado/parameters.php">
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
      function onoff(){
        currentvalue = document.getElementById('onoff').value;
        if(currentvalue == "Off"){
          document.getElementById("onoff").value="On"; // action="on.php" method="POST";
        } else {
          document.getElementById("onoff").value="Off"; //action="off.php" method="POST";
        }
      }
      function submitForm(action) {
        var form = document.getElementById('form1');
        form.action = action;
        form.submit();
      }
    </script>

</head>
<body>

<?php 
$dbh=mysql_connect("localhost", "tesmater_cicla01", "Diopma2203*") 
or die ('I cannot connect to the database because: ' . mysql_error());
mysql_select_db ("tesmater_ciclado01");

$id_ok        = $_POST['iOK'];
$v_status     = $_POST['status'];
$total_c      = $_POST['total_cycles'];
$performed_c  = $_POST['performed_cycles'];
$tmax_c       = $_POST['tmax_sample'];
$tmin_c       = $_POST['tmin_sample'];
$tmax_h       = $_POST['tmax_furnance'];
$tmin_h       = $_POST['tmin_furnance'];
$t4_max       = $_POST['t4max'];
$t4_min       = $_POST['t4min'];
$tub_pos      = $_POST['tub_position'];
$rgb          = $_POST['rgb'];
$furnance     = $_POST['furnance'];
$fan          = $_POST['fan'];
$airpump       = $_POST['airpump'];
$motor        = $_POST['motor'];  
$general      = $_POST['general'];

$v_on         = $_POST['On'];
$v_off        = $_POST['Off'];
$v_stop       = $_POST['Stop'];
$v_on_motor   = $_POST['OnMotor'];
$v_off_motor  = $_POST['OffMotor'];
$v_on_furn    = $_POST['OnFurnance'];
$v_off_furn   = $_POST['OffFurnance'];
$v_on_fan     = $_POST['OnFan'];
$v_off_fan    = $_POST['OffFan'];
$v_on_air     = $_POST['OnAir'];
$v_off_air    = $_POST['OffAir'];
$v_on_gen     = $_POST['OnGeneral'];
$v_off_gen    = $_POST['OffGeneral'];
$v_up         = $_POST['Up'];
$v_down       = $_POST['Down'];

if($id_ok == 1){
    $query = mysql_query("UPDATE tesmater_ciclado01.parameters SET status_id = '$v_status', total_cycles = '$total_c',
    performed_cycles = '$performed_c', t_max_cycles = '$tmax_c', t_min_cycles = '$tmin_c', t_max_hf = '$tmax_h', 
    t_min_hf = '$tmin_h', t4_max = '$t4_max', t4_min = '$t4_min', rgb_color = '$rgb', 
    status_furnance = '$furnance', status_fan = '$fan', status_airpump = '$airpump', status_motor = '$motor',
    status_general = '$general', tub_position = '$tub_pos' WHERE id=1;")  
    or die ('I cannot connect to the database because: ' . mysql_error()); // retorna 1 si hace el insert
    if (!$query){echo "Error! Check your code";} else {
      echo "Update successful";
    }
}

if($v_on == "ON"){
  $query = mysql_query("UPDATE tesmater_ciclado01.parameters SET status_id = 1 WHERE id=1;")  
  or die ('I cannot connect to the database because: ' . mysql_error()); // retorna 1 si hace el insert
  if (!$query){echo "Error! Check your code";} else {
  // echo "Update successful";
  }
}

if($v_off == "OFF"){
  $query = mysql_query("UPDATE tesmater_ciclado01.parameters SET status_id = 10 WHERE id=1;")  
  or die ('I cannot connect to the database because: ' . mysql_error()); // retorna 1 si hace el insert
  if (!$query){echo "Error! Check your code";} else {
  // echo "Update successful";
  }
}

if($v_stop == "STOP"){
  $query = mysql_query("UPDATE tesmater_ciclado01.parameters SET status_id = 0 WHERE id=1;")  
  or die ('I cannot connect to the database because: ' . mysql_error()); // retorna 1 si hace el insert
  if (!$query){echo "Error! Check your code";} else {
  // echo "Update successful";
  }
}

?>

<form id="form1">
  <input type="button" onclick="submitForm('parameters.php')" onclick="document.location.reload();" value="Go back" class="btn btn-outline-primary"/>
  <!-- <input type="button" onclick="submitForm('page2.php')" value="submit 2" /> -->
</form>

<!-- <input type="button" value="On" id="onoff" action="on.php" method="POST"> -->
<!-- fin del php -->
<!-- <p id="logo"> <img src="img/LOGO_UB.png" alt="UB" width="90" height="25"> </p>
<p id="logo"> Thesis by Jefferson Moreira from the Department of Electronics of UB</p> -->
</body>
</html>