# ESP8266 application template

The Barometer folder is my template to setup applications for Wemos D1 Mini Pro in a standard way and may work for other ESP8266 devices. 
Feel free to modify the template for your own use.

Since I think an example is better than a bare template I included my barometer.
<br>The barometer app section has a page with 3 gauges and 8 graphs on which you can zoom in and out between 1 and 7 days.
<br>You can configure alarm limits for temperature, pressure change etc. to receive mail notifications.

It uses a bme280 connected to the I2C interface.

When you do not have a bme280 you can run it in  demo mode.

### Contents :
 - The template, what does the result look like ?
    - Barometer app specifics
 - The template, what more is in it ?
    - An access point with SSID and password
    - Demo mode, data and mail
    - Symmetric encryption for passwords
    - FTP server
    - Functions not in use
    - Examples to serve jpg and animated gif files
  - The template, how to compile the barometer
    - MMU Setting
    - Verifying, compiling and uploading
    - FTP server and FTP Server library notes
  - The template, how to create my own app
    - My naming convention for files
    - The main structure
    - Your code goes here
  - 'Passwords'
    - When your device is still on WiFi and you forgot the setup password
    - When your device is not on WiFi and you forgot the setup password

## The template, what does the result look like ?

Below are some screenshots. There are more in the screenshots folder and a description is on this page.

When my barometer is online you can have a live look at it : http://veraart.thehomeserver.net:81/

<table border = 1 bordercolor ="red" align = center>
<tr>
<td>
<img width="150" height="180" src="https://github.com/JackV2020/ESP8266_application_template/blob/main/screenshots/00_WiFi_Manager.png">
</td>
<td>
<img width="150" height="180" src="https://github.com/JackV2020/ESP8266_application_template/blob/main/screenshots/01_WiFi_Manager_info_page.png">
</td>
<td>
<img width="150" height="180" src="https://github.com/JackV2020/ESP8266_application_template/blob/main/screenshots/02_barometer_home_page.png">
</td>
<td>
<img width="150" height="180" src="https://github.com/JackV2020/ESP8266_application_template/blob/main/screenshots/03_setup_page.png">
</td>
</tr>

<tr>
<td>
<img width="150" height="180" src="https://github.com/JackV2020/ESP8266_application_template/blob/main/screenshots/04_setup_page_system.png">
</td>
<td>
<img width="150" height="180" src="https://github.com/JackV2020/ESP8266_application_template/blob/main/screenshots/05_setup_page_app_1.png">
</td>
<td>
<img width="150" height="180" src="https://github.com/JackV2020/ESP8266_application_template/blob/main/screenshots/06_setup_page_app_2.png">
</td>
<td>
<img width="150" height="180" src="https://github.com/JackV2020/ESP8266_application_template/blob/main/screenshots/09_barometer_gauges.png">
</td>
</tr>

<tr>
<td>
<img width="150" height="180" src="https://github.com/JackV2020/ESP8266_application_template/blob/main/screenshots/09_barometer_gauges_moon.png">
</td>
<td>
<img width="150" height="180" src="https://github.com/JackV2020/ESP8266_application_template/blob/main/screenshots/10_barometer_graph_1_day.png">
</td>
<td>
<img width="150" height="180" src="https://github.com/JackV2020/ESP8266_application_template/blob/main/screenshots/11_barometer_graph_7_days.png">
</td>
<td>
<img width="150" height="180" src="https://github.com/JackV2020/ESP8266_application_template/blob/main/screenshots/08_json_page.png">
</td>
</tr>

</table>

The template has a web server with a tree of pages which supports 
an easy way to setup new applications :

  - an accesspoint with on the home page one of the next 2 options :
      - the WiFi manager when the device is not connected to your WiFi yet.
      - info on how to browse to the device when it connected to your WiFI
  - after connecting to WiFi you can browse using that info and see :
      - a home page with 4 buttons to start :
          - your main app web page which you build yourself
          - a json page so other systems can read data (easy to add your data)
            - ( see curl example scripts for Windows and Linux )
          - a log page (default content but easy to add more)
          - a setup page with 4 buttons to start :
              - the setup of your app which you build yourself
              - the system setup for app password, email and timezone for ntp
              - ElagantOTA for upgrade software from your web browser
                    (ElagantOTA by Ayush Sharma)
              - the WiFi manager

On the bottom of the WiFi manager and setup pages you need to enter an app password.
Default setup password is a single dot (.) which can be changed from the setup pages. Not from the WiFi manager page.
You do not forget passwords but someone else may. They might check the 'Passwords' section below.

Navigate to a page by clicking on a button. 
Navigate one level back by double clicking in the middle of the top of the screen.

#### Barometer app specifics

Navigation:
<br>On the barometer gauges page you can click on any of the gauges to go to the 8 pages with graphs.
<br>On the graph pages click in the left and right bottom corner to go to another graph.
<br>On the graph pages click in the left and right top corner to zoom in and out between 1 and 7 days.
<br>Settings are on 2 pages :
  - offsets for humidity, temperature and pressure
  - update interval of the gauges ( dropdown , 1,5,10, etc seconds)
  - click bottom left and right corner to change page
  - 7 configurable alarm limits
      - select diabled/above/below 
      - enter a numeric value with 1 decimal
      - select one of 7 alarm types (including pressure change per hour and per 2 hours)
      - configure mail in system setup to receive alarm set and alarm clear mails
      - in the received mail is a message and a link on which you can click to browse to your device
      - (simply change the constant nr_limits in the code to increase/decrease the number of limits)

## The template, what more is in it ?

So there is already a lot of the web part in place and maybe you can use some of my barometer 
code for your app when you start. But there is more....

#### An access point to configure WiFi settings

The thing needs to connect to your WiFi so you need to put the SSID and password of your WiFi in. 
This is why it has a WiFi manager running on its access point.
<br>The access point SSID is 'newdevice 192.168.4.1' where the numbers of the IP address may be different.
<br>The password is 1234567890. 
<br>Connect your laptop/phone to the SSID and logon.
<br>Note that it may take some time for the SSID to appear. Sometimes I disable WiFi on my phone, wait a bit and enable it again.
<br> After you log in you can use your browser to go to the home page like : http://'the IP address'/
<br>You will see the WiFi manager page where you select an SSID and enter a password.
Here you also can change the hostname newdevice to something else. ( See 'Demo mode' below )
Click save and a reboot will follow. Read what is on the screen.....
When you changed the hostname a new SSID will be created like 'thenewhostname 192.168.4.1' with password 1234567890.
Connect to that new SSID like you just did the first time and when the WiFi SSID
 and password you gave in on the web page are correct you will see how you can browse to your device over your WiFi.
When there is an error, you will see the WiFi manager page again.
 
#### Demo mode, data and mail

When you do not have a bme280 you can run in demo mode.
<br>Just leave the default hostname newdevice in place or change it in test to run in demo mode.
For the barometer the demo mode will generate dummy data and will not send alarm mails.
Demo mode will only send a mail message during startup. In each mail there is a link you can click to browse to your device.

#### Symmetric encryption for passwords
                
The template creates a LittleFS filesystem to store the log file, your app data, 
the app and the system configuration including passwords for setup, WiFi and email.
Before saving the passwords they are encryped with a symmetric encryption algorithm.
After reading them back they are decypted with the same algorithm so they can be used.
The algorithm uses 3 keys based on the hardware id of your ESP8266 chip.
So when you implement on 2 different ESP8266's you can't copy the configuration 
from one to the other because although the softare is the same, the keys will be different.
Because the result will be guaranteed something which does not work and you will end up on the WiFi manager on the access point to start configuring.

#### FTP server

The template has an FTP server in it.
With FTP you can add/delete/rename files and folders as you like so you can add static web pages and media files.
The FTP server supports 2 accounts. 
One with write access and a second with read and optional write access. The template enables write access for the second account.

As FTP client I use FileZilla and in the Site Manager on tab 3 I need to limit the connections to 1.

#### Functions not in use

I also included some functions which are not used by the barometer but which I use in other apps.

Since they are not used the compiler does not put them in the final program which means they do not use any space.

  - http get and put functions returning the answer from the website in a string
  - https get and put do the same and to save program space there is no certificate validation
  - functions which I can use to post data to my droppoint.
  
Droppoint is a php page I have on a web server where I can drop messages which
  can then be processed by other processes on that web server.
  See 'String dropMail(String messageSubject, String messageToSend)'.
  ( This is not what this implementation uses to send mail but what I used
  before I got the sendMail routine working ;-)
  How to implement the droppoint.php page, the mail sending process and more.... :
  https://github.com/JackV2020/droppoint.
  
When you want to activate the droppoint in your code just go to file 24_PROGMEM.h and look for the remark under 'System setup page'. 
This will add an extra option to the system setup page to specify the droppoint.

#### Examples to serve jpg and animated gif files

I use no media files in my barometer yet but included this to show how it works.
There are 3 urls which show a jpg and an animated gif. 
You find the entries /testjpg, /testgif and /testmedia in 20_setup_loop and these can be used after transfering the media folder to your device.
( Use the FTP server for that. )

## The template, how to compile the barometer

I assume you have a running Arduino IDE and you can compile and run some example code.
When I am wrong you have some work to do....

Put the Barometer folder from the github in your sketches folder.
Navigate into the Barometer folder and double click any of the '.ino' files to open them all in Arduino IDE.

#### MMU Setting

This barometer is an application which needs extra heap :
  - Tools > MMU: "16KB cache +48KB IRAM and 2nd Heap (shared)"
<br>When you remove a lot of the code this may not be required anymore and you may select :
  - Tools > MMU: "32KB cache +32KB IRAM (balanced)"
<br>When you run into runtime memory issue use the first option again.

#### Verifying, compiling and uploading

Expect errors after the next action because you probably do not have all libraries in place yet.

Use the top left button to verify the code.

There will be some libraries missing.

In the code are notes with url's to find the required libraries.
Make sure to use the FTP server from my github. See below

When you have all libraries installed there will be no errors anymore.
Click the little looking glass in the top right corner to start the Serial Monitor.
And can use the 2nd button from the left to compile and upload the code.

After uploading see 'An access point to configure WiFi settings' to get connected.

#### FTP server and FTP Server library notes

Make sure to use https://github.com/JackV2020/FTPClientServer
  which was forked from https://github.com/dplasa/FTPClientServer
    which was forked from https://github.com/nailbuster/esp8266FTPServer
and is modified so :
  - it contains a bug fix for ESP8266 ( see details in the README.md )
  - does not support 1 but 2 users
  - it has a bool property FTPUser2WriteAccess to set the access type of the 2nd user
  - it has a String property FTPaction which you can use to log messages

## The template, how to create a new app

Open Arduino IDE by clicking any of the ino files.
Use the Save As function to save as a different name and a new folder with all files is created.
That's it. Now you can start to modify things to create your own application.

You can manage and use an app setup password on the system setup page so nobody but you
can change the settings. I use the password on the app setup pages the same way.

#### My naming convention for files

To create all web pages, store data, run some code to keep time synchronised, WiFi running, 
perform initial startup, run your code etc. there are quite some functions, variables and data 
to be put into the program generating thousands of lines of code.
As you read above there is not one file but there are more files.

I use this naming so I see all my app related files in the leftmost tabs in the IDE.

To use the template as I meant it to be for myself, files starting with  :
  - 3 should not be changed and only be used as reference
  - 2 have functions, variables etc. which could be changed but should not be renamed (they are used in 3)
  - 1 are for the app.

The files also have a second number which I try to keep the same in all 3 sections like
  - 1 for variables (when variables are not local to a tab but are needed in more tabs)
  - 3 for program code, both app and web functions
  - 4 for PROGMEM (this stores data in flash saving RAM for data)
  - 7 for file system routines ( basic read, write, rename etc, but also save/read app/config data etc.)
  - 9 for prototypes (when functions are not local to the file put a prototype in here)

#### The main structure

When you double click any of the '.ino' files all will open in Arduino IDE.
<br>The files appear in Arduino IDE as seperate tabs, sorted by the numbers.
<br>The leftmost is the app and should also not be changed.
<br>The structure is like:
  - leftmost tab is the application itself and needs no change
    - it includes 39_includes.h
        - which includes all 'nn_vars.h' and 'nn_prototypes.h' files in the right order
    - std_setup which is standard and needs no change (30_setup_loop)
    - app_setup which has a standard name but can be changed by you (20_setup_loop)
    - std_loop which is standard and needs no change (30_setup_loop)
    - app_loop which has a standard name but can be changed by you (20_setup_loop)

#### Your code goes here

After reading the above it should be clear that for what you create :
  - your variables etc. which are used by more tabs like 13_app and 20_setup_loop go into 11_vars.h
  - your app functions go into 13_app
  - your web functions go into 13_web
  - your PROGMEM data goes into 14_PROGMEM.h
  - for every function you create in 13_xxx you create an entry in 19_prototypes.h

You want to change in but not rename functions etc. in :
  - 20_setup_loop   change code but keep function names the same
  - 21_vars.h       change values like app_name, appVersion, password settings and some strings which appear on web pages
  - 23_web          write your serve_setup_app, modify serve_json, modify serve_change_variable (**)
  - 24_PROGMEM.h    All web pages including yours are built using this data. Be careful to change.
  - 27_files        has 2 routines saveAppData and readAppData which you want to modify to handle your data
  - 28_general      functions with standard names I like to use in other apps
  - 29_prototypes.h a prototype for each 2x_... function used in more tabs

(**) Is served by the web server on http://'yourdevice'/change_variable and accepts parameters to change values of variables in the app in the ESP8266.
Example variables are the page numbers of the setup pages, the page numbers and zoom levels of the barometer graphs pages. 
This is used from web pages via XMLHttpRequest in javascript routines.


## 'Passwords'

The setup passwords will be in a safe place and will not be forgotten. However to make sure you can get in in case of...

#### When your device is still on WiFi and you forgot the setup password

There is a line in 21_vars.h :
  - static const char app_password_secret[] PROGMEM = "....."; with a password on the dotted line. 
  - this password can be used on the system setup page to reset the setup password to the default (.)

#### When your device is not on WiFi and you forgot the setup password

You can not get it on the WiFi to go to the system setup page to reset the password. :-(

In std_setup() there is a line which starts the ftp server :
  - ftpServer.begin(sta_hostname,app_password,ftp_emergency_usr,ftp_emergency_pwd);  
  - ftp_emergency_usr and ftp_emergency_pwd are also in 21_vars.h and are not changed by the program.
  - and lucky you, in std_setup() there is also ftpServer.FTPUser2WriteAccess = true;

So connect to the access point of your device and use FileZilla to connect to the access point addres 
using the ftp_emergency_usr and ftp_emergency_pwd and delete the file configuration.txt.
This leaves all app data, logging and all other files you put on it in place.
Reboot the device and reconfigure everything.

( FileZilla > Site Manager on tab 3 limit the connections to 1.)
 
