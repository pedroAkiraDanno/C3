#include <stdio.h>
#include <stdlib.h>
#include<time.h>
int main(){

char buf[0x100];
int num = 0;

   printf( "Enter name of file:");
   gets( buf );
snprintf(buf, sizeof(buf), "%s.txt", buf);

FILE *f = fopen(buf, "w");
printf( "How many numbers ?:");
  scanf("%d", &num);

   srand(time(0));

for(int x =0;x < num; x++){
   fprintf(f,"%d\n", rand());
}
   fclose(f);

return 1;
}


// reference: https://github.com/IneedtoGitaJob/Random-Number-File-Generator