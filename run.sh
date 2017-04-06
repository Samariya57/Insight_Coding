#!/bin/sh

echo "Now we are going to compile the original c++ file..."

g++ -std=c++11 ./src/Log_analysis.cpp -o Program

if [ $? -ne 0 ]
then
	echo "Compillation failed."
else
	echo "Compillation successfully completed."
	echo "Start the program..."
	time=$(date +%s)
	##ls | xargs -n1 --max-procs=4 ./Program
	./Program
	if [ $? -ne 0 ] 
	then 
		echo "Time of work is " $(($(date +%s)-$time)) " second(s)"
		echo "Program works incorrectly."
		while true; do
		    read -p "Do you want to restart the script?" yn
		    case $yn in
			[Yy]* ) ./run.sh; break;;
			[Nn]* ) exit;;
			* ) echo "Please answer yes or no.";;
		    esac
		done
	else
		echo "Time of work is " $(($(date +%s)-$time)) " second(s)"
		echo "Program works correctly."	
	fi
	echo "Results are stored in the following repo: "log_output""
fi
