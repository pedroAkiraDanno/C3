
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









# ranstr4.c

Program to create a random with 10000 words and each word has 10 caracteres

**obs:.**
This program just a simple example of random - it not the complex random program

NEED USE ranstr4.h include

## STEPS by STEPS on Linux Environment (completed)

    sudo su -
    sudo apt-get install gcc -y

    cd ~
    rm -fr C2/
    git clone https://github.com/pedroAkiraDanno/C2
    cd C2/ ; git checkout develop ; cd ..
    cd C2/src/tree/


    gcc -Wall -O2 ranstr4.c -o ranstr4
    #OR
    gcc ranstr4.c -o ranstr4

    ./ranstr4

     
        
---        








Inspired in [devdocs](https://devdocs.io/c/) , [docs.microsoft](https://docs.microsoft.com/en-us/cpp/c-language/?view=msvc-170) , [cppreference](https://en.cppreference.com/w/c/language) , [programiz](https://www.programiz.com/c-programming) and [gnu](https://www.gnu.org/software/gnu-c-manual/gnu-c-manual.html), this is C-minimalistic version.

end:
