if [ $# -lt 1 ] 
then
   echo "There is no parameter! Please enter one or two parameters."
   exit
fi

if [ $# -gt 2 ] 
then
   echo "Too many parameters! Please enter one or two parameters."
   exit
fi

if [ $# -eq 1 ] #pattern
then
   path=./ 
   pattern=$1
else #pattern and path
   path=$1
   pattern=$2
fi

find $path -name "*.[ch]" | xargs grep -Hn $pattern 
