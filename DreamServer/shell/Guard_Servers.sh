#!/bin/bash

#use "nohup bash Guard_servers.sh &" to boot the script

SERVERLIST=("bash" "pretty")

TIME_INTERVAL=3

#echo $? "$?" is the return-value of grep etc.

while true
do
	for s in ${SERVERLIST[@]}
	do
		if ps|grep $s > /dev/null
		then
			echo $s has found
		else
			echo $s not found
			#restart if processes have been not running:
			#nohup hello.o &
		fi
		
		sleep $TIME_INTERVAL
	done
done

:<<eof
comments for some lines
comments for some lines
comments for some lines
eof