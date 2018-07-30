#!/bin/bash


case $1 in
	-h) echo "help table:"
	echo "argv 1: dcpfile name"
	echo "argv 2: user input name for it"
	echo "argv 3: description";;
	h) echo "help table:"
	echo "argv 1: dcpfile name"
	echo "argv 2: user input name for it"
	echo "argv 3: description";;
esac

exit 0

echo "creating afi with dcpfile $1  name $2"
aws ec2 create-fpga-image --region us-east-1 --name $2 --description $3 --input-storage-location Bucket=f1.final.test,Key=dcpfiles/$1 --logs-storage-location Bucket=f1.final.test,Key=logfiles/logfile
