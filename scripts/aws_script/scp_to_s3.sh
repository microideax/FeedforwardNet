#!/bin/bash

echo "copying --- $1 --- to s3 bucket"
aws s3 cp $1 s3://f1.final.test/dcpfiles/

echo "File copied!"
