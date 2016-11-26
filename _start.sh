#!/bin/bash

addressfile=_addresses.txt

gnome-terminal -e "./Debug/NetAVA $addressfile -1" --geometry 50x50--0--0
gnome-terminal -e "./Debug/NetAVA $addressfile 1 2 3" --geometry 50x10+55+15
gnome-terminal -e "./Debug/NetAVA $addressfile 2 1 4 9" --geometry 50x10+55+215
gnome-terminal -e "./Debug/NetAVA $addressfile 3 1 4" --geometry 50x10+55+415
gnome-terminal -e "./Debug/NetAVA $addressfile 4 2 3 5" --geometry 50x10+55+615
gnome-terminal -e "./Debug/NetAVA $addressfile 5 4 6" --geometry 50x10+555+15
gnome-terminal -e "./Debug/NetAVA $addressfile 6 5 7" --geometry 50x10+555+215
gnome-terminal -e "./Debug/NetAVA $addressfile 7 6 8" --geometry 50x10+555+415
gnome-terminal -e "./Debug/NetAVA $addressfile 8 7 9" --geometry 50x10+555+615
gnome-terminal -e "./Debug/NetAVA $addressfile 9 7 2" --geometry 50x10+555+815


