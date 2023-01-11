
# B-Tree in C

Program to B-Tree in C

**obs:.**

B-tree is a self-balancing tree data structure that maintains sorted data and allows searches, sequential access, insertions, and deletions in logarithmic time.

### STEPS on Linux Environment

begin:

---

## 1- step install gcc

    ### login server:
    ### On Ubuntu ###
    sudo su -
    sudo apt update  --yes
    sudo apt-get update && sudo apt-get upgrade -y
    apt list --upgradable
    sudo apt-get install -y gcc
    sudo apt-get install build-essential
    ## Installing Clang on Linux
    sudo apt install -y clang

## Compile projects and files (handmade)

    gcc -Wall -O2 mycreatelargefile.c -o mycreatelargefile
    #OR
    gcc mycreatelargefile.c -o mycreatelargefile

## Running Example

    ./mycreatelargefile

## STEPS by STEPS on Linux Environment (completed)

   sudo su -
   sudo apt-get install gcc -y

    cd ~
    rm -fr C2/
    git clone https://github.com/pedroAkiraDanno/C2
    cd C2/ ; git checkout develop ; cd ..
    cd C2/src/B-Tree/


     gcc -Wall -O2 mycreatelargefile.c -o mycreatelargefile
     #OR
     gcc mycreatelargefile.c -o mycreatelargefile

     ./mycreatelargefile

---









# B-tree4.c

Program to create a B-Tree 

**obs:.**
This program just a simple example of B-Tree 

## STEPS by STEPS on Linux Environment (completed)

    sudo su -
    sudo apt-get install gcc -y

    cd ~
    rm -fr C2/
    git clone https://github.com/pedroAkiraDanno/C2
    cd C2/ ; git checkout develop ; cd ..
    cd C2/src/B-Tree/


    gcc -Wall -O2 B-tree4.c -o B-Tree
    #OR
    gcc B-tree4.c -o B-tree4

    ./B-Tree

     
        
---        








Inspired in [devdocs](https://devdocs.io/c/) , [docs.microsoft](https://docs.microsoft.com/en-us/cpp/c-language/?view=msvc-170) , [cppreference](https://en.cppreference.com/w/c/language) , [programiz](https://www.programiz.com/c-programming) and [gnu](https://www.gnu.org/software/gnu-c-manual/gnu-c-manual.html), this is C-minimalistic version.

end:
