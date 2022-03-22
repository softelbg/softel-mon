
export LD_LIBRARY_PATH=/usr/local/lib

pgrep -f ml|xargs kill -9

DOCROOT=/usr/local/share/Wt
#cp main.css $DOCROOT/style
#cp -r img $DOCROOT/

#iptables -I INPUT -p tcp --dport 3003 -d 192.168.10.0/24 -j ACCEPT

nohup ../../../bin/ml --docroot $DOCROOT --http-address 0.0.0.0 --http-port 3003 > ml.log &

