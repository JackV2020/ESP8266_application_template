// ======================================================================
//  Same for any app <<Do not change>>
// ======================================================================

#include "39_includes.h"

void setup(){
  std_setup();
  app_setup();
}

void loop() {
  std_loop();
  app_loop();
}
// ======================================================================

/*

33_web has modofication :
// ######################
      barometer_page_nr =0;

Some Notes :

// ----------------------------------------------------------------------

The code tabs are numbered like xy_'name' where :

  x : 1 complete free for your code
      2 fixed variable, constant, function etc. names but you may modify value/code
      3 'fixed framework' code which you should not change

  y : 0 for setup and loop code ( both for x=1 and x=3)
      1 for vars ( x=1, 2 and 3)
      2 --- not used yet
      3 for code
      4 for PROGMEM
      5 for very specific code ( x=3 system Ticker )
      6 --- not used yet
      7 files functions
      8 toolbox with general handy functions
      9 prototypes and includes

// ----------------------------------------------------------------------

x9_prototypes.h

When I started coding everything compiled fine, also after I set up a number of tabs.
But at a moment when I added some code like a single function, the compiler
was not able to find previous defined functions anymore. Even when the new function was not used anywhere.
After creating 39_prototypes.h and putting all my functions in there compiling was fine again.
Following this I also created the 19_prototypes.h to prototype exisiting functions with
default values for parameters.

// ----------------------------------------------------------------------

39_includes.h includes all '.h files' in the right order.

An example...

Because it start with :
  #include "31_vars.h"            // standard variables, do not touch
  #include "21_vars.h"            // standard variables, value may be changed
  #include "11_vars.h"            // app variables
and 31_vars.h already contains :
  #include <Ticker.h>
you can use :
  Ticker appTicker;
in your 11_vars.h

Changing the order in 39_includes.h to :
  #include "11_vars.h"            // app variables
  #include "21_vars.h"            // standard variables, value may be changed
  #include "31_vars.h"            // standard variables, do not touch
causes an issue in 11_vars.h because you can't create a ticker before the '#include <Ticker.h>'

// ----------------------------------------------------------------------

Simple PROGMEM usage

I store strings in PROGMEM to save space for stack and heap :
  - simply use F("this string is stored in PROGMEM") in Serial.println,
    string concatenation, parameters to functions etc.
  - or use          : static const char appVersion[] PROGMEM = "V.2022.08.02";
    and use String(appVersion) to get it back in a string.
    ('static':required when defining the string in a function. I use it everywhere)
  - I use 'dynamic PROGMEM' to have dynamic web pages :
    static const char littletext[] PROGMEM = ".. SOMETHING ..";
    String little_html_part=String(littletext) ;
    little_html_part.replace("SOMETHING",someotherstring)
  - double quotes and \ itself are escaped with \" :
    static const char littletext[] PROGMEM = ".. text with a \" and a single \\ in it ..";
    or easier by the R"===( )==="constructor :
    static const char appVersion[] PROGMEM = R"===(some text with a "  a single \ in it)===";

// ----------------------------------------------------------------------

== favicon.ico messages in Serial Monitor

When you see messages about favicon.ico on the serial monitor :
  you can put a <link rel="shortcut icon" href="#"> in the html response.
  this is to avoid that there is also a request for the url /favicon.ico
  which results in a call to notFound.
  https://stackoverflow.com/questions/39149846/why-am-i-seeing-a-404-not-found-error-failed-to-load-favicon-ico-when-not-usin

// ----------------------------------------------------------------------

*/
