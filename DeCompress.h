//
// Created by 35802 on 2021/12/17.
//

#ifndef HF_DECOMPRESS_H
#define HF_DECOMPRESS_H
#include "utils.h"
#include "BasicStruct.h"
#include "Compress.h"
CompressInfo *HeadInfoRead(FILE*fp){
    CompressInfo *Info=(CompressInfo*) malloc(sizeof (CompressInfo));
    int num=0;
    fread(Info,sizeof (CompressInfo),1,fp);
    fread(&num,sizeof (int),1,fp);
    fread(Info->name,num,1,fp);
    fread(&num,sizeof (int),1,fp);
    fread(Info->Extension,num,1,fp);
    fread(Info->UnitSet,sizeof (CompressNode)*(Info->UnitNum),1,fp);
    int i=0;
    for(i=0;i<Info->UnitNum;++i){
        fread(&num,sizeof (int),1,fp);
        fread(Info->UnitSet[i].unit, num,1,fp);

        fread(&num,sizeof (int),1,fp);
        fread(Info->UnitSet[i].HuffCode, num,1,fp);
    }
    return Info;
}
char*GenCompressString(FILE*fp,double basicUnitSize,char*preString,CompressInfo*CInfo){//由原串得到huffman串
    char *temp;
    char *temp1;
    temp=ReadString(fp,basicUnitSize,CInfo->FileSize);//读字符串，长度为BasicNum*2字节，保证整字节
    if((ftell(fp) >= CInfo->FileSize)) {//到文件末，证明读到的是尾串
        temp = BiChConverse(temp, 1, temp[(int)(basicUnitSize*2)]);
        temp = StringCombina(CInfo->UnitSet[CInfo->UnitNum-1].HuffCode,NULL);
    } else{//证明读到的非尾串
        temp = BiChConverse(temp, 1, (int)(basicUnitSize*2));
        temp1= StringCut(temp,(int)(basicUnitSize*2*8)/2-1);
        temp = StringCombina(CInfo->UnitSet[FindInMap(temp1,CInfo)].HuffCode,CInfo->UnitSet[FindInMap(temp,CInfo)].HuffCode);
    }
    temp = StringCombina(preString,temp);
    if(*preString!='\0')free(preString);
    return temp;
}
HuffmanNode *HuffSearch(HuffmanNode *node,CompressInfo*CInfo,char*preString){
    if(strlen(preString)<CInfo->bit||node->code==Node_SINGAL){
        exit(114514);
    }else{
        char *now= StringCut(preString,CInfo->bit-1);
        char *a=BinaryToDemical(now);
        int c=*a-48;
        return &node->Child[c];
    }
}
void DeCompress(char*oriPath,char*tarPath){
    FILE *fori= fopen(oriPath,"rb+");
    printf("%d\n",feof(fori));
    CompressInfo *CInfo= HeadInfoRead(fori);
    printf("%d\n",feof(fori));
    HuffmanTree *HT=HuffmanTreeConstruct(CInfo);
    char *outname= StringCombina(tarPath, StringCombina(CInfo->name,CInfo->Extension));
    FILE *ftar= fopen(outname,"wb+");
    HuffmanNode *Hn=HT->Head;
    char*preString="\0";
    char*preBiString="\0";
    char*write;
    int written_bite=0;
    int FileSize= file_size(oriPath);
    while(ftell(fori)<FileSize){//TODO Translate the readString into
        preString= StringCombina(preString, BiChConverse(ReadString(fori,(double )CInfo->bit*4, file_size(oriPath)),1,(int)(CInfo->bit*8)));
        while(strlen(preString)>=CInfo->bit&&!feof(fori)){
            if(ftell(fori)<FileSize||CInfo->completeSize==0){
                if(Hn->code==Node_SINGAL){
                    preBiString= StringCombina(preBiString,CInfo->UnitSet[Hn->num].unit);
                    Hn=HT->Head;
                }else{
                    Hn= HuffSearch(Hn,CInfo,preString);
                }
            }
        }
        if(strlen(preBiString)>=8){
            write=BiChConverse(StringCut(preBiString, 8*(floor(strlen(preBiString)/8))-1),0,0);
            written_bite+= strlen(write);
            printf("%d\n",written_bite);
            fwrite(write, sizeof (char), strlen(write),ftar);
        }
    }
    if(CInfo->completeSize!=0){
        preString= StringCut(preString,CInfo->bit-CInfo->completeSize-1);
        while(strlen(preString)>=CInfo->bit){
                if(Hn->code==Node_SINGAL){
                    preBiString= StringCombina(preBiString,CInfo->UnitSet[Hn->num].unit);
                    Hn=HT->Head;
                }else{
                    Hn= HuffSearch(Hn,CInfo,preString);
                }
        }
        if(strlen(preBiString)>=8){
            write=BiChConverse(StringCut(preBiString, 8*(floor(strlen(preBiString)/8)-1)),0,0);
            fwrite(write, sizeof (char), strlen(write),ftar);
        }
    }
    fclose(fori);
    fclose(ftar);
}
#endif //HF_DECOMPRESS_H
