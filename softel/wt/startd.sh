
export LD_LIBRARY_PATH=/usr/local/lib

killall wthttpd
killall zmqsub

nohup ../../../bin/zmqsub module=ModulePublish proto=MDF delay=1000 > /dev/null & 

nohup ../../../bin/wthttpd --docroot /usr/local/share/Wt --http-address 0.0.0.0 --http-port 9090 > /dev/null &

