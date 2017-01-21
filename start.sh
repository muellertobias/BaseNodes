#!/bin/bash

graphfile=graph.gv
addressfile=addresses.txt
port=5000

if [ "$#" -lt 3 ]
then
	echo "./start.h NumberOfNodes NumberOfEdge Config [Graphfile]"
else

	numberOfNodes=$1
	numberOfEdges=$2
	config=$3

	gnome-terminal -e "./Debug/NetAVA $addressfile listener"

	if [ "$#" -eq 5 ]
	then
		echo "use graphfile"
		graphfile=$5
	else
		# Erstelle zufälligen Graphen
		./GraphGen $graphfile $numberOfNodes $numberOfEdges
	fi
	# Erstelle Adressdatei
	echo "0 127.0.0.1:$port" > $addressfile
	for ((i = 1 ; i <= numberOfNodes ; i++)); do 
		((port+=1))
		echo "$i 127.0.0.1:$port" >> $addressfile
	done

	# Starte Knoten und Initiator
	for ((i = 1 ; i <= numberOfNodes ; i++)); do 
		./Debug/NetAVA $addressfile node $i $config $graphfile &
	done

	./Debug/NetAVA $addressfile initiator

fi



