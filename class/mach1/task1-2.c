#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    enum {size = 4};
    double res_sum = 0, x;
    int i, j, ind = 0, k = 0, len = 32;
    char *res_concat = calloc(len, sizeof(char));
    char *str;
    for(i = 1; i < argc; i++){
        x = strtod(argv[i], NULL);
        if (x != 0){
            res_sum += x;
        }else{
            str = argv[i];
            /*if(str[0] != '.'){
                for(y = 0; str[y]; y++){
                    z = str[y];
                    if((z != '.') && (z != '0')){
                        break;
                    }else{
                        if((str[y] == '.')&&flag)
                            break;
                        if(str[y] == '.')
                            flag = 1;
                    }
                }
                if(str[y] == '\0'){
                    printf("end!\n");
                    i++;
                }
            }*/
            for(j = 0; str[j]; j++){
                if(ind == ((len-1) + size*k)){
                    len += size;
                    res_concat = (char*)realloc(res_concat,len);
                    k++;
                }
                res_concat[ind] = str[j];
                ind++;
            }

        }
    }
    printf("res_sum = %f\n", res_sum);
    printf("res_concat = %s\n", res_concat);
    return 0;
}