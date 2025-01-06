mkdir -p ~/lib/;
mkdir -p ~/goinfre/


git clone https://github.com/catchorg/Catch2.git ~/goinfre/Catch2;
cmake -B ~/goinfre/Catch2/build -S ~/goinfre/Catch2 -DBUILD_TESTING=OFF -DCMAKE_INSTALL_PREFIX=~/lib
cmake --build ~/goinfre/Catch2/build/ --target install 
