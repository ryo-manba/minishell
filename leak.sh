while [ 1 ]
do
leaks $1 1>/dev/null
if [ $? -ne 0 ]; then
    leaks $pid
fi
sleep 5
done
