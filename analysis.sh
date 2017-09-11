sudo echo ""
clear
echo "Real/Pretend Experiment Log File Analysis"
echo ""

# look for contents of /data...
for entry in data/*.dat
do
    #echo "$entry"
    ./LogAnalysis "$entry"
done
echo "All Done"
echo ""
