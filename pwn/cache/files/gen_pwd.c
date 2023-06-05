#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>



int fnct1(int i){
    int tmp;
    do{
        tmp = rand();
        tmp = tmp / (int)(0x7fffffff / (long)(i + 1));
    }while(i<tmp);

    return tmp;
}

char fnct2(char *s){
  int tmp;
  size_t len;

  len = strlen(s);
  tmp = fnct1((int)len + -1);
  return s[tmp];
}


char *main() {
    time_t seed;
    char c;
    int i=0x14;
    int index;
    int pwd_seed = 4;
    char pwd[20] ={0};
    char *alphabet[4];

    alphabet[0] = "1234567890";
    alphabet[1] = "abcdefghijklmnoqprstuvwyzx";
    alphabet[2] = "ABCDEFGHIJKLMNOPQRSTUYWVZX";
    alphabet[3] = "!@#$%^&*(){}[]:<>?,./";


    seed = time((time_t*)0x00);
    srand((int)seed);

    for(; i!=0; i--){
        index = fnct1(pwd_seed -1);
        c = fnct2(alphabet[index]);
        strncat(pwd, &c, 1);
        }

    printf("%s", pwd);

    exit(0);
}
