
export LD_LIBRARY_PATH=/usr/local/lib

#killall wthttpd

../../../bin/wthttpd --docroot /usr/local/share/Wt --http-address 0.0.0.0 --http-port 9091

