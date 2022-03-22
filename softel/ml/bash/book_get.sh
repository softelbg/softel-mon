


#cat nauchna-fantastika |grep txt.zip|cut -d "\"" -f 2

CHITANKA_LINK="https://chitanka.info"
STR_LINKS="nauchna-fantastika"


function getAllBooks {
	getBook $STR_LINKS

	for i in `seq 2 25`
	do
		getBook $i
	done
}

function getBook {
	for i in $(cat $1 |grep txt.zip|cut -d "\"" -f 2)
	do
		echo wget $CHITANKA_LINK$i
		wget $CHITANKA_LINK$i
	done
}

function getLinks {

	for i in `seq 2 25`
	do
		GET_LINK=$CHITANKA_LINK/books/category/nauchna-fantastika.html/$i
		echo wget $GET_LINK
		wget $GET_LINK
	done
}

function doUnzip {
	for i in $(ls *.zip)
	do
		echo unzip $i
		unzip $i
	done
}


#getLinks
#getBook $STR_LINKS

#getAllBooks

doUnzip

