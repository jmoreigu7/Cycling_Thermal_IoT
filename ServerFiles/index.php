<!DOCTYPE html>
<html>
<head>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.0/jquery.min.js"></script>  
<link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css" integrity="sha384-MCw98/SFnGE8fJT3GXwEOngsV7Zt27NXFoaoApmYm81iuXoPkFOJwJ8ERdknLPMO" crossorigin="anonymous">
<script src="https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/js/bootstrap.min.js" integrity="sha384-ChfqqxuZUCnJSK3+MXmPNIyE6ZbWh2IMqE241rYiqJxyMiZ6OW/JmZQ5stwEULTy" crossorigin="anonymous"></script>
<title>Cycling Thermal (IoT) - Univeritat de Barcelona - Jefferson Moreira</title>
  <style>
    table{
      border: 2px solid black;
      text-align: center;
      border-radius: 10px;
    }
    #header {
      text-align: center;
    }
    #credentials{
      text-align: center;
    }
    #logo{
      text-align: center;
    }
  </style>
</head>
<body>
<br><br><br><br><br><br><br><br><br>
  <h1 id="header"> Cycling Thermal (IoT) </h1><br>
<div id="credentials">
  <form action="/ciclado/init_parameters.php" method="POST">
    <table align="center" id="table">
        <tr>
          <td>User</td>
          <td><input type="text" class="input" name="user" id="user"></td>
        </tr>
        <tr>
          <td>Password</td>
          <td><input type="password" class="input" name="password" id="pass"></td>
        </tr>
    </table><br>
    <button type="submit" class="btn btn-outline-dark">Send</button>
  </form>
</div>
<br><br><br><br><br><br><br><br><br><br><br>  
<p id="logo"> <img src="img/LOGO_UB.png" alt="UB" width="90" height="25"> </p>
<p id="logo"> Final thesis by Jefferson Moreira, student at the University of Barcelona in the Department of Electronics.</p>

</body>
</html> 
