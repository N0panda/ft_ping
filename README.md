# ft_ping

### 42 School project

ft_ping is my basic implementation of the function ping under Linux.

## usage

->  Use the command "make" to build the project <br />
->  Run : ./ft_ping -h to see the usage. <br />


## options

-> -h   :   Display usage <br />
-> -v   :   Display the error code if the reply is not an ICMP_REPLY <br />
-> -4   :   Use only ipv4 <br />
-> -6   :   USE only ipv6 [TODO, ipv6 not supported at the moment] <br />
-> -t   :   Modify Time to Live <br />
-> -c   :   Number of ECHO_REQUEST to send <br />
-> -w   :   Time between each echo_request (Not a timeout like in the original ping) <br />

## Incoming options TODO

-> Support ipv6 <br />
-> Add a timeout option <br />
