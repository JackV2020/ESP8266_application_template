# ESP8266 application template

The Barometer folder is my template to setup applications for Wemos D1 Mini Pro in a standard way and may work for other ESP8266 devices. 
Feel free to modify the template for your own use.

Since I think an example is better than a bare template I included my barometer.
<br>The barometer app section has a page with 3 gauges and 8 graphs, zoom in and out between 1 and 7 days.
<br>Options toconfigure alarm limits for temperature, pressure change etc. to receive mail notifications.

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
    - My code goes here
  - 'Passwords'
    - When the device is still on WiFi and we forgot the setup password
    - When the device is not on WiFi and we forgot the setup password
    - Add a reset button

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
      - the WiFi manager when the device is not connected to the WiFi yet.
      - info on how to browse to the device when it connected to the WiFI
  - after connecting to WiFi it is possible to browse using that info and see :
      - a home page with 4 buttons to start :
          - the main app web page which has to be created for each app
          - a json page so other systems can read data (easy to add app data)
            - ( see curl example scripts for Windows and Linux )
          - a log page (default content but easy to add more)
          - a setup page with 4 buttons to start :
              - the setup for the app which has to be created for each app
              - the system setup for app password, email and timezone for ntp
              - ElagantOTA for upgrade software from a web browser
                    (ElagantOTA by Ayush Sharma)
              - the WiFi manager

On the bottom of the WiFi manager and setup pages an app password has to be entered.
Default setup password is a single dot (.) which can be changed from the System Setup page.
We do not forget passwords but someone else may. They might check the 'Passwords' section below.

Navigate to a page by clicking on a button. 
Navigate one level back by double clicking in the middle of the top of the screen.

#### Barometer app specifics

Navigation:
<br>On the barometer gauges page click on any of the gauges to go to the 8 pages with graphs.
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
      - in the received mail is a message and a link on which browses to the device
      - (simply change the constant nr_limits in the code to increase/decrease the number of limits)

## The template, what more is in it ?

So there is already a lot of the web part in place and we can use some of my barometer 
code for other apps. But there is more....

#### An access point to configure WiFi settings

The thing needs to connect to our WiFi so we need to put the SSID and password of the WiFi in. 
This is why it has a WiFi manager running on its access point.
<br>The access point SSID is 'newdevice 192.168.4.1' where the numbers of the IP address may be different.
<br>The password is 1234567890. 
<br>Connect a laptop/phone to the SSID and logon.
<br>Note that it may take some time for the SSID to appear. Sometimes I disable WiFi on my phone, wait a bit and enable it again.
<br>After log in we can use the browser to go to the home page like : http://'the IP address'/
<br>We will see the WiFi manager page where we select an SSID and enter the password of the WiFi.
<br>Here we also can change the hostname newdevice to something else. ( See 'Demo mode' below )
<br>Enter the app password which initially is a dot (.) 
<br>Click save and a reboot will follow. Read what is on the screen.....
When we changed the hostname a new SSID will be created like 'thenewhostname 192.168.4.1' with password 1234567890.
Connect to that new SSID like we just did the first time and when the WiFi SSID 
and password we gave in on the web page are correct we will see how to browse to the device over our WiFi.
When the connection to the WiFi failed, we will see the WiFi manager page again.

The access point will shutdown 15 minutes after boot so when you need access to it later you need to reboot.

#### Demo mode, data and mail

When you do not have a bme280 you can run in demo mode.
<br>Just leave the default hostname newdevice in place or change it in test to run in demo mode.
For the barometer the demo mode will generate dummy data and will not send alarm mails.
Demo mode will only send a mail message during startup. In each mail there is a link to browse to the device.

#### Symmetric encryption for passwords
                
The template creates a LittleFS filesystem to store the log file, the app data, 
the app and the system configuration including passwords for setup, WiFi and email.
Before saving the passwords they are encryped with a symmetric encryption algorithm.
After reading them back they are decypted with the same algorithm so they can be used.
The algorithm uses 3 keys based on the hardware id of the ESP8266 chip.
So when we implement on 2 different ESP8266's we can't copy the configuration 
from one to the other because although the softare is the same, the keys will be different.
Because the result will be guaranteed something which does not work and we will end up on the WiFi manager on the access point to start configuring.

#### FTP server

The template has an FTP server in it.
With FTP we can add/delete/rename files and folders as we like so we can add static web pages and media files.
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
We find the entries /testjpg, /testgif and /testmedia in 20_setup_loop and these can be used after transfering the media folder to the device.
( Use the FTP server for that. )

## The template, how to compile the barometer

I assume you have a running Arduino IDE and you can compile and run some example code.
When I am wrong you have some work to do....

Put the Barometer folder from the github in the sketches folder.
Navigate into the Barometer folder and double click any of the '.ino' files to open them all in Arduino IDE.

#### MMU Setting

This barometer is an application which needs extra heap :
  - Tools > MMU: "16KB cache +48KB IRAM and 2nd Heap (shared)"
<br>When we remove a lot of the code this may not be required anymore and we may select :
  - Tools > MMU: "32KB cache +32KB IRAM (balanced)"
<br>When we run into runtime memory issue use the first option again.

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
  - it has a String property FTPaction which we can use to log messages

## The template, how to create a new app

Open Arduino IDE by clicking any of the ino files.
Use the Save As function to save as a different name and a new folder with all files is created.
That's it. Now we can start to modify things to create a new application.

You can manage and use an app setup password on the system setup page so nobody but you
can change the settings. I use the password on the app setup pages the same way.

#### My naming convention for files

To create all web pages, store data, run some code to keep time synchronised, WiFi running, 
perform initial startup, run the code etc. there are quite some functions, variables and data 
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

When we double click any of the '.ino' files all files will open in Arduino IDE.
<br>The files appear in Arduino IDE as seperate tabs, sorted by the numbers.
<br>The leftmost is the app and should also not be changed.
<br>The structure is like:
  - leftmost tab is the application itself and needs no change
    - it includes 39_includes.h
        - which includes all 'nn_vars.h' and 'nn_prototypes.h' files in the right order
    - std_setup which is standard and needs no change (30_setup_loop)
    - app_setup which has a standard name but can be changed (20_setup_loop)
    - std_loop which is standard and needs no change (30_setup_loop)
    - app_loop which has a standard name but can be changed (20_setup_loop)

#### My code goes here

After reading the above it should be clear that for what I create :
  - variables etc. which are used by more tabs like 13_app and 20_setup_loop go into 11_vars.h
  - app functions go into 13_app
  - web functions go into 13_web
  - PROGMEM data goes into 14_PROGMEM.h
  - for every function I create in 13_xxx I create an entry in 19_prototypes.h

I may want to change in but not rename functions etc. in :
  - 20_setup_loop   change code but keep function names the same
  - 21_vars.h       change values like app_name, appVersion, password settings and some strings which appear on web pages
  - 23_web          write serve_setup_app, modify serve_json, modify serve_change_variable (**)
  - 24_PROGMEM.h    All web pages including app pages are built using this data. Be careful to change.
  - 27_files        has 2 routines saveAppData and readAppData which I modify to handle app data
  - 28_general      functions with standard names I like to use in other apps
  - 29_prototypes.h a prototype for each 2x_... function used in more tabs

(**) Is served by the web server on http://'thedevice'/change_variable and accepts parameters to change values of variables in the app in the ESP8266.
Example variables are the page numbers of the setup pages, the page numbers and zoom levels of the barometer graphs pages. 
This is used from web pages via XMLHttpRequest in javascript routines.


## 'Passwords'

The setup passwords will be in a safe place and will not be forgotten. However to make sure we can get in in case of...

#### When the device is still on WiFi and we forgot the setup password

There is a line in 21_vars.h :
  - static const char app_password_secret ..... ; with a password on the dotted line. 
  - this password can be used on the system setup page to reset the setup password to the default (.)

#### When the device is not on WiFi and we forgot the setup password

we can not get it on the WiFi to go to the system setup page to reset the password. :-(

In std_setup() there is a line which starts the ftp server :
  - ftpServer.begin(sta_hostname,app_password,ftp_emergency_usr,ftp_emergency_pwd);  
  - ftp_emergency_usr and ftp_emergency_pwd are also in 21_vars.h and are not changed by the program.
  - and lucky us, in std_setup() there is also ftpServer.FTPUser2WriteAccess = true;

So connect to the access point of the device and use FileZilla to connect to the access point addres 
using the ftp_emergency_usr and ftp_emergency_pwd and delete the file configuration.txt.
This leaves all app data, logging and all other files we put on it in place.
Reboot the device and reconfigure everything.

( FileZilla > Site Manager on tab 3 limit the connections to 1.)

#### Add a reset button

Another method is to be prepared with a reset button.

You can connect a switch between D5 and 0 volt and add some code.

In the setup of 20_setup_loop you add :

  pinMode(D5, INPUT_PULLUP);

In the loop of 20_setup_loop you add :

  if(!digitalRead(D5))
  {
    deleteFile(String(file_config));
    rebootRequestCounter = 2;
    rebootRequestReason = F("Reset button pressed");
  }

When you press the switch :
  - the configuration file is deleted
  - the system ticker (35_systemTicker) will save your app data
  - the system ticker logs an event in the system log
  - the system ticker reboots your device
  
App data and logging stays available so after connecting to the access point and configuring WiFi you can configure the system settings and all app settings, data and logging is still in place.
