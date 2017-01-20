#!/bin/bash

graphfile=graph.gv
addressfile=addresses.txt
port=5000

if [ "$#" -lt 4 ]
then
	echo "./start.h NumberOfNodes NumberOfEdge Impl Config [Graphfile]"
else

	numberOfNodes=$1
	numberOfEdges=$2
	impl=$3
	config=$4

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
	echo "1 127.0.0.1:$port" > $addressfile
	for ((i = 2 ; i <= numberOfNodes ; i++)); do 
		((port+=1))
		echo "$i 127.0.0.1:$port" >> $addressfile
	done

	# Starte Knoten und Initiator
	for ((i = 1 ; i <= numberOfNodes ; i++)); do 
		./Debug/NetAVA $addressfile node $i $impl $config $graphfile &
	done

	./Debug/NetAVA $addressfile initiator

fi



