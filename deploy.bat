if "%1"=="-i"  goto init
if "%1"=="" goto end
goto deploy

:init
mkdir output
cd output
git clone -b gh-pages git@github.com:tracholar/wiki.git ./
cd ..
goto end

:deploy
git add . --all
git commit -am %1
git pull origin master
git push origin master

simiki g
cd output
git add . --all
git commit -am %1
git pull origin gh-pages
git push origin gh-pages
cd ..
goto end

:end