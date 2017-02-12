#!/bin/bash

graphfile=graph.gv
addressfile=addresses.txt
port=5000

if [ "$#" -lt 3 ]
then
	echo $#
	echo "./start.h NumberOfNodes Config Graphfile [AddressFile]"
else

	numberOfNodes=$1
	config=$2
	graphfile=$3

	gnome-terminal -e "./Debug/NetAVA $addressfile listener"

	if [ "$#" -lt 4 ]
	then
		# Erstelle Adressdatei
		echo "0 127.0.0.1:$port" > $addressfile
		for ((i = 1 ; i <= numberOfNodes ; i++)); do 
			((port+=1))
			echo "$i 127.0.0.1:$port" >> $addressfile
		done
	else
		addressfile=$4
	fi

	# Starte Knoten und Initiator
	for ((i = 1 ; i <= numberOfNodes ; i++)); do 
		./Debug/NetAVA $addressfile node $i $config $graphfile &
	done

	gnome-terminal -e "./Debug/NetAVA $addressfile initiator"

fi



