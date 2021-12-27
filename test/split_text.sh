#!/bin/bash

unset i;

while read -r L;
	do echo $L > "$(echo -n $L | md5sum | cut -d ' ' -f1).txt";
done

