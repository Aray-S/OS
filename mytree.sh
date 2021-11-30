function myTree(){ #Recursive function
	   for file in `ls $1` 
	   do
	      if [ -d "$1/$file" ] #folder
	      then
	         echo "$2$file" 
	         myTree "$1/$file" "    $2" 
	      else 
	         echo "$2$file" 
	      fi
	   done
	}
	thispath='./'
	myTree $thispath "" 

