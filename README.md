# MINI ALTAVISTA project

Search engine ALTAVISTA implementation. Web demo for indexation and search text using wikicorpus.

![alt text](https://www.tuexperto.com/wp-content/uploads/2013/06/altavista.jpg)

You can try the demo search engine using the web or the CLI client.

#### Clone the project
```sh
git clone https://github.com/joshjo/mini-google.git
```

#### Copy and extract the wikicorpus

Download the file from http://www.cs.upc.edu/~nlp/wikicorpus/raw.ca.tgz 

Create a folder in the same folder of the repository.
```sh
mkdir files
```

Extract the data inside the `files` folder.

#### Requirements
```sh
Install brew
copy and paste the next command in terminal 
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```

#### Dependencies
```sh
Install libboost-all-dev
command for install libboost-all-dev
brew install boost
```
```sh
Install g++
command for install g++ 
brew install g++ 
```
```sh
Install cmake
command for install cmake
brew install cmake 
```

```sh
Install make
command for install make
brew install make 
```

## Web version

#### Compile and run server

```sh
cd build
cmake ..
make
`./run_server`
```

#### Run web files

Go to `web` folder and open the `index.html` file
```sh
cd web
open index.html
```

## CLI version

Being in the root of the projcet move to `models` folder and run the executable

```sh
cd models
g++ main.cpp -o engine
```

#### Run the project
```sh
./engine
```# mini-google
