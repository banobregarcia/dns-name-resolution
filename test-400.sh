FILE3 = "server/config/RootServer"
FILE4 = "server/config/SubDomainServer"
FILE5 = "server/config/MachineNameServer"
if [ -r $FILE3 ] && [ -r $FILE4 ] && [ -r $FILE5 ]; then
   echo "The files '$FILE3' '$FILE4' '$FILE5'exist and they are readable."
else
   echo "The files '$FILE3' '$FILE4' '$FILE5' are not found."
fi
