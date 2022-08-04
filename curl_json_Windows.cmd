@echo off
goto start

This is a demo script to create a single line curl command to read a value.

Call this scipt by entering the IP address (not the host name but the IP address) like :

	curl_json_Windows.cmd 192.168.2.5

The first curl result will show the json data.

Each key-value pair is handled as a single letter in the script.

So when the first curl result starts with :

{"sta_hostname":"barometer" , "appVersion":"V.2022.08.02" , etc............... 

The appVersion would be found by the script below by using the letter b in the single line curl command.

There are some examples at the end for Temperature etc.

So what you actually need is the code after the line with ':examples' below.
and the letter that you have to put in for the key-value pair.

Temperature is element o
Pressure1h is element t
Pressure2h is element u

:start
cls
set Wemos_IP=%1
Echo.
Echo........... curl test Full result
Echo.
for /f "tokens=* delims=," %%a in ('curl http://%Wemos_IP%/json -s') do echo %%a

:: Skip some code I used to developing this and go to the final examples

goto examples

Echo.
Echo........... With the next line locate the key-value pair for your entry, COUNT pairs in the previous result
Echo.
:: ----- change the last letter of the next line ( i is boottime and contains colons )
for /f "tokens=1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26 delims=,}" %%a in ('curl http://%Wemos_IP%/json -s') do echo %%i


echo.
Echo........... Filter the value for boottime with ^"'s
echo.
:: ----- put the letter you found in the ('echo %%i') part at the end 
for /f "tokens=1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26 delims=,}" %%a in ('curl http://%Wemos_IP%/json -s') do for /f "tokens=1,* delims=:" %%u in ('echo %%i') do echo %%v


:: ----- save the result in variable value1
:: ----- put the letter you found in the ('echo %%i') part at the end 
for /f "tokens=1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26 delims=,}" %%a in ('curl http://%Wemos_IP%/json -s') do for /f "tokens=1,* delims=:" %%u in ('echo %%i') do set value1=%%v
:: You can use the value1 in your script :
echo %value1%


Echo.
Echo........... Value of for boottime without ^"'s
Echo.
:: Note the ^'s in the last for construction on the next lines. This is to be able to strip the double quotes.
:: ----- put the letter you found in the ('echo %%i') part before the ('echo %%v') at the end
:: ----- WATCH IT, there is an extra  ('echo %%v') at the end, do not change that
for /f "tokens=1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26 delims=,}" %%a in ('curl http://%Wemos_IP%/json -s') do for /f "tokens=1,* delims=:" %%u in ('echo %%i') do for /f tokens^=1^ delims^=^" %%y in ('echo %%v') do echo %%y


:: ----- save the result in variable value2
:: ----- put the letter you found in the ('echo %%i') part before the ('echo %%v') at the end
for /f "tokens=1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26 delims=,}" %%a in ('curl http://%Wemos_IP%/json -s') do for /f "tokens=1,* delims=:" %%u in ('echo %%i') do for /f tokens^=1^ delims^=^" %%y in ('echo %%v') do set value2=%%y
echo %value2%

:examples

Echo.
Echo........... Temperature is element o
Echo.
:: ----- put the letter you found (now o for temperature) in the ('echo %%i') part before the ('echo %%v') at the end
:: ----- change the letter below the 'V' at the end of this line ------------------------------------------------------------------------------------------------------------------------V
for /f "tokens=1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26 delims=,}" %%a in ('curl http://%Wemos_IP%/json -s') do for /f "tokens=1,* delims=:" %%u in ('echo %%o') do for /f tokens^=1^ delims^=^" %%y in ('echo %%v') do set value3=%%y
echo Temperature : %value3%


Echo.
Echo........... Pressure1h is element t
Echo.
:: ----- change the letter below the 'V' at the end of this line ------------------------------------------------------------------------------------------------------------------------V
for /f "tokens=1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26 delims=,}" %%a in ('curl http://%Wemos_IP%/json -s') do for /f "tokens=1,* delims=:" %%u in ('echo %%t') do for /f tokens^=1^ delims^=^" %%y in ('echo %%v') do set value4=%%y
echo Pressure Change / hour : %value4%


Echo.
Echo........... Pressure2h is element u
Echo.
:: ----- change the letter below the 'V' at the end of this line ------------------------------------------------------------------------------------------------------------------------V
for /f "tokens=1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26 delims=,}" %%a in ('curl http://%Wemos_IP%/json -s') do for /f "tokens=1,* delims=:" %%u in ('echo %%u') do for /f tokens^=1^ delims^=^" %%y in ('echo %%v') do set value5=%%y
echo Pressure Change / 2 hours : %value5%


Echo.
Echo........... appVersion is element b
Echo.
:: ----- change the letter below the 'V' at the end of this line ------------------------------------------------------------------------------------------------------------------------V
for /f "tokens=1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26 delims=,}" %%a in ('curl http://%Wemos_IP%/json -s') do for /f "tokens=1,* delims=:" %%u in ('echo %%b') do for /f tokens^=1^ delims^=^" %%y in ('echo %%v') do set value6=%%y
echo appVersion: %value6%

Echo.
:eof