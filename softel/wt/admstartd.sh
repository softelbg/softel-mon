
export LD_LIBRARY_PATH=/usr/local/lib

#killall wthttpd
#killall zmqsub
pgrep -f wtm|xargs kill -9
pgrep -f tmcore|xargs kill -9
pgrep -f admtrade|xargs kill -9


DOCROOT=/usr/local/share/Wt
#cp main.css $DOCROOT/style
#cp -r img $DOCROOT/

#nohup ../../../bin/zmqsub module=ModulePublish proto=MDF delay=1000 > /dev/null &
#nohup ../../../bin/wthttpd --docroot $DOCROOT --http-address 0.0.0.0 --http-port 9090 > /dev/null &

nohup ../../../bin/admtrade --docroot $DOCROOT --http-address 0.0.0.0 --http-port 4004 > admtrade.log &

