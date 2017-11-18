# User friendly ping application for ns-3
## Course Code: CO300
     
### Overview

ns-3 already has an implementation of ping application. However, using it is not user friendly. In this project, the goal is to improve the usage of ping application in ns-3 by making it user friendly.        
      
### Usage     
      
An example program for User-friendly ping is provided in        

`ns-allinone-3.26/ns-3.26/examples/internet-apps`       
        
and sholud be executed as      
       
`./waf --run iping`        
       
To execute the various functionalities included in iping.cc, use the following commands:      
* `./waf --run "iping --c=3 --t=64 --v=True"`       
* `./waf --run "iping --s=17 --w=15 --v=True"`       
* `./waf --run "iping --i=3 --w=15 --v=True"`       
* `./waf --run "iping --q=true"`       

### References:

[1]  ns-3 ping source code.       
[2]  http://www.nsnam.org       
[3]  man ping - https://linux.die.net/man/8/ping
