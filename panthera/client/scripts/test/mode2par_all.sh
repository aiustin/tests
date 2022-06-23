#!/bin/bash
readarray -t arr < symbols.txt
for i in "${arr[@]}"
do
	./client/bin/testApp $i@20:38 > client_$i.out &
done


wait
echo "All done!"


# OSX
#!/bin/zsh
#for line in "${(@f)"$(<symbols.txt)"}"
#{
#  ./client/bin/testApp $line@20:38 > client_$line.out &
#}


