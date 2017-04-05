# Project "Logs analysis"  
## Table of contents  
1. [Introduction](README.md#introduction)  
2. [How to run?](README.md#how_to_run)  
    [Assamptions](README.md#assamptions)
3. [Results](README.md#results)    
    [Feature 1](README.md#feature1)  
    [Feature 2](README.md#feature2)  
    [Feature 3](README.md#feature3)  
    [Feature 4](README.md#feature4)  
    [Response code statistics](README.md#feature5)  
    [Incorrect](README.md#feature6)  
    [Not found](README.md#feature7)  
    [Server error](README.md#feature8)  
    [Most popular resources](README.md#feature9)  
4. [Briefly about algorithms](README.md#algorithm)  
5. [Future problems](README.md#conclusion)  

<a name="introduction"></a>
## Introduction  
Picture yourself as a backend engineer for a NASA fan website that generates a large amount of Internet traffic data. Your challenge is to perform basic analytics on the server log file, provide useful metrics, and implement basic security measures.  

<a name="how_to_run"></a>
## How to run?  
1. This Program uses _*boost*_ library. To install ths library type following in the Linux terminal
 (root rights are needed)   : 
```{sh }
sudo apt-get install libboost-all-dev
```
    
2. If the first step is successfully completed, run **src.sh** script to compile original .cpp with C++11 by typing:
```{sh }
./run.sh
```
3. You will see the time of Program work and results will be stored at "/log_output" directory.

<a name="assamptions"></a>
### Assumptions
Here and further:
* *correct request* - any recognizable request with codes not equal to 400;
* *bad request* - recognisable request with code 400;
* *incorrect request* - not recognisable request.
We asume:
1. Directory structure is fixed;  
2. All incorrect requests are stored in *"/log_output/incorrect.txt"* file and we don't process them;  
3. Order of request parts is fixed and they are separated by spaces;
4. The time zone is the same for all requests;
5. Input consists of only one non-empty file "log.txt";
<a name="results"></a>
## Results  
<a name="feature1"></a>
### Feature 1 (hosts.txt)  
List the top 10 most active host/IP addresses that have accessed the site.

The file *"/log_output/hosts.txt"* contains 10 or all (whatever is less) most active hosts/IP.
Counted all correct and bad requests. 

_Format_: *Host/IP,frequency*
```
    piweba3y.prodigy.com,22177
    piweba4y.prodigy.com,14831
    piweba1y.prodigy.com,12776
```
<a name="feature2"></a>
### Feature 2 (resources.txt)  
List the top 10 resources that consume the most bandwidth on the site.  

The file *"/log_output/resources.txt"* contains 10 or all (whatever is less) most bandwidth on the site.
Counted all requests except incorrect ones.  

_Format_: *resource*
```
    /shuttle/countdown/count.gif
    /shuttle/missions/sts-70/movies/woodpecker.mpg
    /shuttle/missions/sts-71/movies/crew-arrival-t38.mpg

```
<a name="feature3"></a>
### Feature 3 (hours.txt)  
List the top 10 busiest (or most frequently visited) 60-minute periods.  

The file *"/log_output/hours.txt"* contains 10 or all (whatever is less) most busiest 60-minute periods.
Counted all requests except incorrect ones.  

_Format_: *start time,busyness*
```
    13/Jul/1995:01:58:30 -0400,36000
    13/Jul/1995:01:58:29 -0400,35999
    13/Jul/1995:01:58:28 -0400,35998
```
<a name="feature4"></a>
### Feature 4 (blocked.txt)
Detected patterns of three failed login attempts from the same IP address over 20 seconds so that all further attempts to the site can be blocked for 5 minutes. List of all requests which should be blocked. 

Clear definition can be found [here]( https://github.com/InsightDataScience/fansite-analytics-challenge/blob/master/README.md#details-of-implementation "here")

The file *"/log_output/blocked.txt"* contains all "blocked".
Counted all requests except inroccert ones.  

_Format_: *lines*
```
    207.109.29.70 - - [01/Jul/1995:01:28:41 -0400] "GET /images/WORLD-logosmall.gif HTTP/1.0" 200 669
    207.246.17.94 - - [01/Jul/1995:02:51:28 -0400] "GET /history/history.html HTTP/1.0" 200 1602
    215.298.34.27 - - [01/Jul/1995:03:27:25 -0400] "POST /login HTTP/1.0" 401 1420
``` 
<a name="feature5"></a>
### Response code statistics (code_statistics.txt)
List of all server response codes with their frequencies.

The file *"/log_output/code_statistics.txt"* contains all codes with their frequencies.
Counted all requests except incorrect ones.

This metrics can be useful for system diagnosis.

_Format_: *code,frequency*
```
    200,569
    300,5
    400,52
``` 
<a name="feature6"></a>
### Incorrect  
The file *"/log_output/incorrect.txt"* contains all incorrect requests. Usually it's empty.  

_Format_: *lines*
```
    207.1001/Jul/1995:01:28:41 -0400] "GET /images/WORLD-logosmall.gif HTTP/1.0" 500 669
    0501 1602 qwerty
    215.298.34.27 - - [01/Jul/1995:03:27: /login HTTP/1.0" 500 1420
``` 
<a name="feature7"></a>
### Not Found (not_found.txt)  
List of the most frequently not found pages.

The file *"/log_output/not_found.txt"* contains 20 or all (whatever is less) not found requests with frequency.
Counted all requests with server code response 404.

This metrics can be useful for preparation for the next bug fix session.

_Format_: *resource,frequency*
```
    /pub/winvn/readme.txt,667
    /pub/winvn/release.txt,547
    /history/apollo/apollo-13.html,286
``` 
<a name="feature8"></a>
### Server errors (server_error.txt)  
List of all requests with server response code more than or equal to 500.

The file *"/log_output/server_error.txt"* contains all requests with server error code.

This metrics can be useful for system administrators.

_Format_: *lines*
```
    207.109.29.70 - - [01/Jul/1995:01:28:41 -0400] "GET /images/WORLD-logosmall.gif HTTP/1.0" 500 669
    207.246.17.94 - - [01/Jul/1995:02:51:28 -0400] "GET /history/history.html HTTP/1.0" 501 1602
    215.298.34.27 - - [01/Jul/1995:03:27:25 -0400] "POST /login HTTP/1.0" 500 1420
``` 
<a name="feature9"></a>
### Most popular resources (pop_resources.txt)  
List of the most popular/frequent resources.

The file *"/log_output/pop_resources"* contains 50 or all (whatever is less) most popular site resources.

This metrics can be useful for further statistics research. For example, what type of resources(images, gifs ect) is the most popular?

_Format_: *resource,frequency*
```
    /shuttle/countdown/count.gif,256
    /shuttle/missions/sts-70/movies/woodpecker.mpg,23
    /shuttle/missions/sts-71/movies/crew-arrival-t38.mpg,3
```   
<a name="algorithm"></a>
## Briefly about algorithms
The main idea is to read logs line by line. Parse string into parts with space symbols.
We have three types of parse result: correct request (10 parts), bad request (8 parts), incorrect request (unknown)
With every line we renew features with the following logic:
* For the first feature we use map to store frequencies. Add 1 host/IP for every line we read.
* For the second feature we use the same method.
* For the third feature we keep current hour busyness by seconds (3600 seconds) and all previous hours busyness by hours.
* For the fourth feature we keep map for 401 requests and cut everything older than 20 seconds with a new 401 request. For currently blocked addresses we keep 5 minutes interval and renew with every non-empty second.
* For the code statistics we add new code for every request.
* For the Not found the same meyhod.
* For the Most popular resources the same method.
<a name="conclusion"></a>
## Future problems  
1. Work further on third feature and choose disjoint busiest hours, because current realisation is not informative (all periods differ by few seconds);
2. Continue to work with most popular requests by type of files and/or by topics;
3. In the real project: print all resources with only one visit and analyse them. 
