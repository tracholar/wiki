if [ "$1" = "-i" ] ; then
    mkdir output
    cd output
    git clone -b gh-pages git@github.com:tracholar/wiki.git ./
    cd ..
    exit 0
elif [ "$1" = "" ] ; then
    exit 0
else
    git add . --all
    git commit -am "$1"
    git pull origin master
    git push origin master

    simiki g
    cd output
    git add . --all
    git commit -am "$1"
    git pull origin gh-pages
    git push origin gh-pages
    cd ..


fi
