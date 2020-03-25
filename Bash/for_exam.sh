#!/bin/bash
for((i=0;i<30;i++)); do
	nohup python test.py test$i.log &
	echo $i
done
