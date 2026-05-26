#!/bin/bash

set -eufo pipefail

<<<<<<< HEAD
for i in usermode-count syscall-count {uprobe,uretprobe}-{nop,push,ret,nop5} usdt-nop usdt-nop5
=======
for i in usermode-count syscall-count {uprobe,uretprobe}-{nop,push,ret,nop5}
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
do
	summary=$(sudo ./bench -w2 -d5 -a trig-$i | tail -n1 | cut -d'(' -f1 | cut -d' ' -f3-)
	printf "%-15s: %s\n" $i "$summary"
done
