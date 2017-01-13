#!/bin/bash

graphfile=graph.gv
addressfile=addresses.txt
port=5000

numberOfNodes=$1
numberOfEdges=$2
impl=$3

# Erstelle zufälligen Graphen
./GraphGen $graphfile $numberOfNodes $numberOfEdges

# Erstelle Adressdatei
echo "1 127.0.0.1:$port" > $addressfile
for ((i = 2 ; i <= numberOfNodes ; i++)); do 
	((port+=1))
	echo "$i 127.0.0.1:$port" >> $addressfile
done

# Starte Knoten und Initiator
for ((i = 1 ; i <= numberOfNodes ; i++)); do 
	#gnome-terminal -e "./Debug/NetAVA $addressfile node $i $graphfile"
	./Debug/NetAVA $addressfile node $i $impl $graphfile &
done

#gnome-terminal -e "./Debug/NetAVA $addressfile listener"
./Debug/NetAVA $addressfile initiator


#gnome-terminal -e "./Debug/NetAVA $addressfile 1 $graphfile" --geometry 50x10+55+15
#gnome-terminal -e "./Debug/NetAVA $addressfile 2 $graphfile" --geometry 50x10+55+215
#gnome-terminal -e "./Debug/NetAVA $addressfile 3 $graphfile" --geometry 50x10+55+415
#gnome-terminal -e "./Debug/NetAVA $addressfile 4 $graphfile" --geometry 50x10+55+615
#gnome-terminal -e "./Debug/NetAVA $addressfile 5 $graphfile" --geometry 50x10+555+15

