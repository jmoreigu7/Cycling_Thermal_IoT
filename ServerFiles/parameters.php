<!DOCTYPE html>
<html>
<head>
<title>Cycling Thermal (IoT) - Univeritat de Barcelona - Jefferson Moreira</title>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.0/jquery.min.js"></script>  
<link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css" integrity="sha384-MCw98/SFnGE8fJT3GXwEOngsV7Zt27NXFoaoApmYm81iuXoPkFOJwJ8ERdknLPMO" crossorigin="anonymous">
<script src="https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/js/bootstrap.min.js" integrity="sha384-ChfqqxuZUCnJSK3+MXmPNIyE6ZbWh2IMqE241rYiqJxyMiZ6OW/JmZQ5stwEULTy" crossorigin="anonymous"></script>
<link href="https://gitcdn.github.io/bootstrap-toggle/2.2.2/css/bootstrap-toggle.min.css" rel="stylesheet">
<script src="https://gitcdn.github.io/bootstrap-toggle/2.2.2/js/bootstrap-toggle.min.js"></script>
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
$sql_select = "select * from  tesmater_ciclado01.parameters where id = 1"; 
$Result=mysql_query($sql_select);
$row = mysql_fetch_assoc($Result);
$v_id           =     $row["id"];
$v_status       =     $row["status_id"];
$v_total_c      =     $row["total_cycles"];
$v_performed    =     $row["performed_cycles"];
$v_max_cycles   =     $row["t_max_cycles"];
$v_t_min_cycles =     $row["t_min_cycles"];
$v_t_max_hf     =     $row["t_max_hf"];
$v_t_min_hf     =     $row["t_min_hf"];
$v_t4_max       =     $row["t4_max"];
$v_t4_min       =     $row["t4_min"];
$v_tub          =     $row["tub_position"];
$v_rgb          =     $row["rgb_color"];
$v_status_furn  =     $row["status_furnance"];
$v_status_fan   =     $row["status_fan"];
$v_status_air   =     $row["status_airpump"];
$v_status_motor =     $row["status_motor"];
$v_status_gen   =     $row["status_general"];
?>

<div id=botomcenter>
  <form action="/ciclado/interpret.php" method="POST">
    <table align="center">
        <tr>
          <th scope="col">Parameter</th>
          <th scope="col">Value</th>
        </tr>
        <tr>
          <td>Status</td>
          <td><input type="number" class="input" name="status" id="status" value=<?php echo $v_status ?> ><br></td>
        </tr>
        <tr>
          <td>Total cycles</td>
          <td><input type="number" class="input" name="total_cycles" id="total_cycles" value=<?php echo $v_total_c ?> ><br></td>
        </tr>
        <tr>
          <td>Performed cycles</td>
          <td><input type="number" class="input" name="performed_cycles" id="performed_cycles" value=<?php echo $v_performed ?>><br></td>
        </tr>
        <tr>
          <td>Maximum temperature sample</td>
          <td><input type="number" class="input" name="tmax_sample" id="tmax_sample" value="<?php echo $v_max_cycles ?>"><br></td>
        </tr>
        <tr>
          <td>Minimum temperature sample</td>
          <td><input type="number" class="input" name="tmin_sample" id="tmin_sample" value="<?php echo $v_t_min_cycles ?>"><br></td>
        </tr>
        <tr>
          <td>Maximum temperature hot furnance</td>
          <td><input type="number" class="input" name="tmax_furnance" id="tmax_furnance" value="<?php echo $v_t_max_hf ?>"><br></td>
        </tr>
        <tr>
          <td>Minimum temperature hot furnance</td>
          <td><input type="number" class="input" name="tmin_furnance" id="tmin_furnance" value="<?php echo $v_t_min_hf ?>"><br></td>
        </tr>
        <tr>
          <td>Maximum temperature TH4</td>
          <td><input type="number" class="input" name="t4max" id="t4max" value="<?php echo $v_t4_max ?>"><br></td>
        </tr>
        <tr>
          <td>Minimum temperature TH4</td>
          <td><input type="number" class="input" name="t4min" id="t4min" value="<?php echo $v_t4_min ?>"><br></td>
        </tr>
        <tr>
          <td>Tub position</td>
          <td><input type="number" class="input" name="tub_position" id="tub_position" value="<?php echo $v_tub ?>"><br></td>
        </tr>
        <tr>
          <td>RGB color</td>
          <td><input  type="number" class="input" name="rgb" id="rgb" value="<?php echo $v_rgb ?>"><br></td>
        </tr>
        <tr>
        <tr>
          <td>Furnance status</td>
          <td><input  type="number" class="input" name="furnance" id="furnance" value="<?php echo $v_status_furn ?>"><br></td>
        </tr>
        <tr>
          <td>Fan status</td>
          <td><input type="number" class="input" name="fan" id="fan" value="<?php echo $v_status_fan ?>"><br></td>
        </tr>
        <tr>
          <td>Airpump status</td>
          <td><input type="number" class="input" name="airpump" id="airpump" value="<?php echo $v_status_air ?>"><br></td>
        </tr>
        <tr>
          <td>Motor status</td>
          <td><input type="number" class="input" name="motor" id="motor" value="<?php echo $v_status_motor ?>"><br></td>
        </tr>
        <tr>
          <td>General status</td>
          <td><input type="number" class="input" name="general" id="general" value="<?php echo $v_status_gen ?>"><br></td>
        </tr>
    </table><br>
    <table align="center">
    <tr>
      <th scope="col">
        <button type="submit" name="iOK" id = "iOK" value ="1" class="btn btn-outline-primary" >Send</button>
      </th>
      <th scope="col"> 
        <input type="button" value="Refresh" class="btn btn-outline-secondary" onclick="document.location.reload();">
      </th>
      <th scope="col">
      <form method="post" action="interpret.php">  
        <input type="submit" name="On" value="ON" class="btn btn-outline-success" />  
      </form>  
      </th>
      <th scope="col">
      <form method="post" action="interpret.php">  
        <input type="submit" name="Off" value="OFF" class="btn btn-outline-success" />  
      </form>  
      </th>
      <th scope="col">
      <form method="post" action="interpret.php">  
        <input type="submit" name="Stop" value="STOP" class="btn btn-outline-success" />  
      </form> 
      </th>
    </tr>
  </table>

  <!-- <center>
        <button class="btn btn-primary"><span class="glyphicon glyphicon-refresh"></span> Refresh</button>
  </center> -->
</div>

</body>
</html>