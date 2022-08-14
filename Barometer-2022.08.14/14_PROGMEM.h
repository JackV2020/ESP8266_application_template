// ======================================================================
//  App web page content in PROGMEM
// ======================================================================

// ======================================================================
//  App setup bottom buttons to next/prev page 
// ( could be standard but template app does not need this )
// ======================================================================

// to be  preceded by SetupBottom 
static const char AppSetupPageButtons[] PROGMEM = R"====(
<script>
function switch_page(direction) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
 var date = new Date();
 var curDate = null;
 do { curDate = new Date(); }
 while(curDate-date < 500);
 window.location.href='/setup_app';
    }
  };
  xhttp.open("GET", "change_variable?setup_page="+direction, true);
  xhttp.send();
}
</script>
<div onclick="switch_page('prev'); " style="position: absolute; bottom:20;  left:0;border: 0px solid red;width:25%;height:100px;"></div>
<div onclick="switch_page('next'); " style="position: absolute; bottom:20; right:0;border: 0px solid red;width:25%;height:100px;"></div>
)====";
//

// ======================================================================
//  App setup pages
// ======================================================================

static const char appSetupPage_1[] PROGMEM = R"====(
<p>STATUS_TXT</p>
<form action="/setup_app" method="POST">
<button class="rnd_btn" style="position:fixed; top:20; left:5" name="saveappsetup" value="yes">Save</button>
<h3>Offsets</h3>
Temperature (TMP &degC)<br><input type="text" id="Temperature_offset" name="Temperature_offset" placeholder="1.23" value=TO>
<br><br>Pressure (PRS hPa)<br><input type="text" id="Pressure_offset" name="Pressure_offset" placeholder="1.23" value=PR>
<br><br>Humidity (HUM %)<br><input type="text" id="Humidity_offset" name="Humidity_offset" placeholder="1.23" value=HU>
<br><h3>Graphs</h3>
Gauges Update Interval<br>
<select style="width: 200px;" name="gaugeInterval" id="gaugeInterval">
<optgroup label="Gauges Update Interval :">
<option value="1">1 second</option>
<option value="5">5 seconds</option>
<option value="10">10 seconds</option>
<option value="15">15 seconds</option>
<option value="20">20 seconds</option>
<option value="30">30 seconds</option>
<option value="60">60 seconds</option>
</optgroup>
</select>
)====";

static const char appSetupPage_2[] PROGMEM = R"====(
<p>STATUS_TXT</p>
<form action="/setup_app" method="POST">
<button class="rnd_btn" style="position:fixed; top:20; left:5" name="saveappsetup" value="yes">Save</button>
<h3>Alarm Limits</h3>
)====";

// ======================================================================
//  App web page for gauges
// ======================================================================

// info on gauge :
// https://developers.google.com/chart/interactive/docs/gallery/gauge

// info on locale settings for decimal . or , ( first link howto and second link codes )
// https://developers.google.com/chart/interactive/docs/basic_load_libs#loadwithlocale
// https://www.science.co.il/language/Locale-codes.php

static const char appShowGauges[] PROGMEM = R"====(
<div id="showmoon" onclick="openFullscreen();" style="position:absolute;top:10px;left:10px;width:50px;height:50px;background-color:transparent;border-radius:50%;box-shadow: 20px 10px 0 0 gold;"></div>
<script>var notfs=true; var elem = document.getElementById("theBody");function openFullscreen() {  if (notfs) { if (elem.requestFullscreen) {    elem.requestFullscreen();  } else if (elem.webkitRequestFullscreen) {   elem.webkitRequestFullscreen(); } else if (elem.msRequestFullscreen) { elem.msRequestFullscreen(); } ; notfs = false ; } else  { document.exitFullscreen() ;notfs = true ;  }  }</script>
<script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
<script type="text/javascript">
google.charts.load('current', {'packages':['gauge'], 'language': 'nl'});
google.charts.setOnLoadCallback(drawChart);
function drawChart() {
  var dataT = google.visualization.arrayToDataTable([['Label', 'Value'],['DEGREEC', TEMPERATURE],]);
  var dataP = google.visualization.arrayToDataTable([['Label', 'Value'],['hPa', PRESSURE],]);
  var dataH = google.visualization.arrayToDataTable([['Label', 'Value'],['%', HUMIDITY]]);

  var size = window.innerHeight / 4 ;

  var optionsT = {width: 300, height: size, min: -20, max: 50, majorTicks: ['-20','-10','0','10','20','30','40','50'], minorTicks: 10,
    greenFrom:0, greenTo:25, yellowFrom:25, yellowTo: 35, redFrom: 35, redTo: 50 };
  var optionsP = {width: 300, height: size, min: 900, max: 1100, majorTicks: ['900','950','1000','1050','1100'], minorTicks: 5,
    redFrom:900, redTo:965, yellowFrom:965, yellowTo: 1035, greenFrom: 1035, greenTo: 1100 };
  var optionsH = {width: 300, height: size, min: 0, max: 100, majorTicks: ['0','10','20','30','40','50','60','70','80','90','100'], minorTicks: 5,
    greenFrom:20, greenTo:60, yellowFrom:60, yellowTo: 70, redFrom: 70, redTo: 100 };
  var chartT = new google.visualization.Gauge(document.getElementById('chart_divT'));
    chartT.draw(dataT, optionsT);
  var chartP = new google.visualization.Gauge(document.getElementById('chart_divP'));
    chartP.draw(dataP, optionsP);
  var chartH = new google.visualization.Gauge(document.getElementById('chart_divH'));
    chartH.draw(dataH, optionsH);

  setInterval(function() {
    getData();
  }, GAUGEINTERVAL000);

  function getData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        var result = this.responseText;
        result = result.substring(0,result.indexOf("}")+1);
        var jsonobj = JSON.parse(result);

        dataT.setValue(0, 1, jsonobj.Temperature);
        chartT.draw(dataT, optionsT);

        dataP.setValue(0, 1, jsonobj.Pressure);
        chartP.draw(dataP, optionsP);

        dataH.setValue(0, 1, jsonobj.Humidity);
        chartH.draw(dataH, optionsH);
      }
    };
    xhttp.open("GET", "json?update=yes", true);
    xhttp.send();
    }

  }

  function switch_page(pagenumber) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        var date = new Date();
        var curDate = null;
        do { curDate = new Date(); }
        while(curDate-date < 500);
        window.location.href='/app';
      }
    };
    xhttp.open("GET", "change_variable?barometer_page="+pagenumber, true);
    xhttp.send();
  }

</script>
<center>
<div onclick="switch_page('1'); " id="chart_divT" style="width: 175px;height: 175px;"></div>
<div onclick="switch_page('2'); " id="chart_divP" style="width: 175px;height: 175px;"></div>
<div onclick="switch_page('3'); " id="chart_divH" style="width: 175px;height: 175px;"></div>
</center>
)====";

// ======================================================================
//  App web page for charts 
//  with still some html comment <!-- --> which I may remove to save space
// ======================================================================

// 3 lines chart ( 3 comma seperated {data:....} sections in datasets )  example and chart documentation links
// https://www.w3schools.com/js/tryit.asp?filename=tryai_chartjs_lines_multi
// https://www.chartjs.org/docs/2.7.3/charts/line.html

static const char appShowChart[] PROGMEM = R"====(
<p>STATUS_TXT</p>
<script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.5.0/Chart.min.js"></script>
<font color="blue">LABEL</font>
<center><canvas onclick="window.location.href='/app';" id="CHARTID" style="width:80%;max-width:300px;height:300px;"></canvas></center>
<br><div>DATETIME.CDT</div>

<script>
var xValues = [XVALUES];
new Chart("CHARTID", {
  type: "line",
  data: {
    labels: xValues,
    datasets: [
      {data: DATA,
      pointRadius: 1,
      borderColor: "blue",
      borderWidth: 1,
      backgroundColor: "rgba(0,0,255,0.1)",
//      borderColor: "rgba(193,46,12,0.2)",
      fill: true}
    ]
  },
  options: {
    legend: {
      display: false
    },
    scales: {
      xAxes: [{gridLines: { color: "black" }, ticks: { fontColor: "blue" }, } ],
      yAxes: [{gridLines: { color: "black" }, ticks: { fontColor: "blue" }, } ]
      }
  }
});

function zoom(zoom_direction) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var date = new Date();
      var curDate = null;
      do { curDate = new Date(); }
      while(curDate-date < 500);
      window.location.href='/app';
    }
  };
  xhttp.open("GET", "change_variable?zoom_direction="+zoom_direction, true);
  xhttp.send();
}

function switch_page(direction) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var date = new Date();
      var curDate = null;
      do { curDate = new Date(); }
      while(curDate-date < 500);
      window.location.href='/app';
    }
  };
  xhttp.open("GET", "change_variable?barometer_page="+direction, true);
  xhttp.send();
}

</script>
<!--
//<center><button class="rnd_btn" style="position:fixed; bottom:20">Press</button></center>
//   https://www.w3schools.com/howto/howto_css_center_button.asp
//<center><div style="text-align: center;border: 3px solid green;width:100px;height:50px;"><button style="position: absolute; left: 50%;transform: translateX(-50%);">Button</button></div></center>
//<center><div style="text-align: center;border: 3px solid green;width:100px;height:50px;"><button class="rnd_btn" style="position: absolute; left: 50%;transform: translateX(-50%);">Button</button></div></center>
//   CENTERED DIV ---v
//<center><div style="position: absolute; bottom:20; left: 50%;transform: translateX(-50%);border: 3px solid green;width:300px;height:100px;" onclick="window.location.href = '/';"></div></center>
//<center><div ondblclick="window.location.href = '/barometer';" style="position: absolute; bottom:20; left: 50%;transform: translateX(-50%);border: 3px solid green;width:50%;height:50px;"></div></center>
-->
<center><div ondblclick="switch_page('0'); " style="position: absolute; top:20;    left: 50%;transform: translateX(-50%);width:50%;height:100px;border: 0px solid red;"></div></center>
<center><div ondblclick="switch_page('0'); " style="position: absolute; bottom:20; left: 50%;transform: translateX(-50%);width:50%;height:100px;border: 0px solid red;"></div></center>
<!--
//   CENTERED DIV ---^
//   CENTERED BUTTON ---v
//<button onclick="window.location.href = '/barometer';" class="rnd_btn" style="position: absolute; bottom:20; left: 50%;transform: translateX(-50%);">Barometer</button>
//   LEFT DIV ---
-->
<div onclick="switch_page('prev'); " style="position: absolute; bottom:20;  left:0;border: 0px solid red;width:25%;height:100px;"></div>
<div onclick="switch_page('next'); " style="position: absolute; bottom:20; right:0;border: 0px solid red;width:25%;height:100px;"></div>

<div onclick="zoom('in'); " style="position: absolute; top:20;  left:0;border: 0px solid red;width:25%;height:100px;"></div>
<div onclick="zoom('out');" style="position: absolute; top:20; right:0;border: 0px solid red;width:25%;height:100px;"></div>
)====" ;
