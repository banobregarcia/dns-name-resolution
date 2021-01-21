FILE1 = "client/config/RootServers"
FILE2 = "client/config/RequeteFile"
if [ -r $FILE1 ] && [ -r $FILE2 ]; then
   echo "The files '$FILE1' '$FILE2' exist and they are readable."
else
   echo "The files '$FILE1' '$FILE2' are not found."
fi
