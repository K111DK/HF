#include <stdio.h>
#include "DeCompress.h"
int main() {
//char *a="0100";
//char *b="1234";
//char *f= BinaryToDemical(a);
//f;
//    printf("%s\n",f);



    char *oriPath="C:\\Users\\35802\\Desktop\\source\\pdf.pdf";
    char *ComPath="C:\\Users\\35802\\Desktop\\compress\\pdf.taz";
    char *DePath="C:\\Users\\35802\\Desktop\\Decompress\\";
    printf("size of file %d\n", file_size(oriPath)/1024);
    FILE *fpin,*fpout;
    CompressInfo *CInfo;
    HuffmanTree *HT;
    char*pre="\0";
    CInfo=MapConstruct(1,2,oriPath);

    HT=HuffmanTreeConstruct(CInfo);


    HuffmanCode(HT->Head,0,CInfo->HuffBranch,pre,CInfo);


    CompressFileGen(CInfo,HT,oriPath,ComPath);
    DeCompress(ComPath,DePath);



//    char a[3];
//    a[0]=0x00;
//    a[1]=0x0F;
//    a[3]='\0';
//    char *new="1000111";//TODO由二进制串转化回来时会反转？？
//    printf("%s\n", BinaryToDemical(new));
    return 0;
}