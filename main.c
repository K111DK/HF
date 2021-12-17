#include <stdio.h>
#include "DeCompress.h"
int main() {
//char *a="0100";
//char *b="1234";
//char *f= BinaryToDemical(a);
//f;
//    printf("%s\n",f);
//    char*oriPath="C:\\Users\\35802\\Desktop\\HuffmanCompress\\实验08_信号处理及有限状态机.pdf";
//    char*tarPath="C:\\Users\\35802\\Desktop\\HuffmanCompress\\实验08_信号处理及有限状态机.taz";
    char *oriPath="C:\\Users\\35802\\Desktop\\source\\pdf.pdf";
    char *ComPath="C:\\Users\\35802\\Desktop\\compress\\pdf.tar";
    char *DePath="C:\\Users\\35802\\Desktop\\Decompress\\";
    char*TestFile="C:\\Users\\35802\\Desktop\\testFile\\pdf.pdf";
    printf("size of file %d\n", file_size(oriPath)/1024);
    FILE *fpin,*fpout;
    CompressInfo *CInfo;
    HuffmanTree *HT;
    char*pre="\0";
    CInfo=MapConstruct(1,2,oriPath);
    MapPrint(CInfo);
    HT=HuffmanTreeConstruct(CInfo);
    HuffmanCode(HT->Head,0,CInfo->HuffBranch,pre,CInfo);
    Test(CInfo,HT,oriPath,TestFile);
//    CompressFileGen(CInfo,HT,oriPath,ComPath);
//    DeCompress(ComPath,DePath);
//    char a[3];
//    a[0]=0x25;
//    a[1]=0x50;
//    a[3]='\0';
//    char*temp= BiChConverse(a,1,2);
//    printf("%s\n",temp);
    return 0;
}