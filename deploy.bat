if "%1"=="-i" then goto init
goto deploy

:init
mkdir output
git clone -b gh-pages git@github.com:tracholar/wiki.git
goto end

:deploy
@cd output
git add .
git commit -m %1
git pull origin gh-pages
git push origin gh-pages
@cd ..
goto end

:end