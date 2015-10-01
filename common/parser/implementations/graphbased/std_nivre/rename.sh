for filename in *nivre*
do
newname=`echo $filename|sed -n 's/nivre/std_nivre/p'`
mv $filename $newname
sed -i 's/nivre/std_nivre/g' $newname
sed -i 's/NIVRE/STD_NIVRE/g' $newname
done
