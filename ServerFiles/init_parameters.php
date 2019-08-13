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
    #botomcenter{
      text-align: center;
    }
    #logo{
      text-align: center;
    }
  </style>
</head>
<body>
<!-- validation of credentials -->
<?php
$dbh=mysql_connect("localhost", "tesmater_cicla01", "Diopma2203*") 
or die ('I cannot connect to the database because: ' . mysql_error());
mysql_select_db ("tesmater_ciclado01");
$v_user = $_POST['user'];
$v_pass = $_POST['password'];
$sql = "SELECT idcredential FROM tesmater_ciclado01.credential where user = '$v_user' and pass = '$v_pass';";
$result = mysql_query($sql)
or die ('I cannot connect to the database because: ' . mysql_error()); // retorna 1 si hace el insert

if(mysql_num_rows($result) == 0){
    echo "Invalid credentials";
    exit;
}

$sql_select = "select * from  tesmater_ciclado01.parameters where id = 1"; 
// (select max(id) from tesmater_ciclado01.parameters)";
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
?>
<br><br><br><br><br><br><br><br>
<h1 id="header"> Cycling Thermal (IoT) </h1><br>
<!-- fin del php -->
<div id=botomcenter>
  <form action="/ciclado/bridge.php" method="POST">
    <table align="center">
        <tr>
          <th scope="col">Parameter</th>
          <th scope="col">Value</th>
        </tr>
        <tr>
          <!-- <td>1.</td> -->
          <td>Total cycles</td>
          <td><input type="number" class="input" name="total_c" id="total_c" value=<?php echo $v_total_c ?> ><br></td>
        </tr>
        <tr>
          <!-- <td>2.</td> -->
          <td>Performed cycles</td>
          <td><input type="number" class="input" name="performed_c" id="performed_c" value=<?php echo $v_performed ?>><br></td>
        </tr>
        <tr>
          <!-- <td>3.</td> -->
          <td>Maximum temperature sample</td>
          <td><input type="number" class="input" name="tmax_c" id="tmax_c" value="<?php echo $v_max_cycles ?>"><br></td>
        </tr>
        <tr>
          <!-- <td>4.</td> -->
          <td>Minimum temperature sample</td>
          <td><input type="number" class="input" name="tmin_c" id="tmin_c" value="<?php echo $v_t_min_cycles ?>"><br></td>
        </tr>
        <tr>
          <!-- <td>5.</td> -->
          <td>Maximum temperature hot furnance</td>
          <td><input type="number" class="input" name="tmax_h" id="tmax_h" value="<?php echo $v_t_max_hf ?>"><br></td>
        </tr>
        <tr>
          <!-- <td>6.</td> -->
          <td>Minimum temperature hot furnance</td>
          <td><input type="number" class="input" name="tmin_h" id="tmin_h" value="<?php echo $v_t_min_hf ?>"><br></td>
        </tr>
    </table><br>
    <button type="submit" name="iOK" id = "iOK" value ="1" class="btn btn-outline-dark">Send</button>
  </form>
</div>
<br><br><br><br><br>
<p id="logo"> <img src="img/LOGO_UB.png" alt="UB" width="90" height="25"> </p>
<p id="logo"> Final thesis by Jefferson Moreira, student at the University of Barcelona in the Department of Electronics.</p>

</body>
</html> 
