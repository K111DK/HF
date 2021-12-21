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
    Info->name=(char*) malloc(sizeof (char)*(num+1));
    fread(Info->name,num,1,fp);
    fread(&num,sizeof (int),1,fp);

    Info->Extension=(char*) malloc(sizeof (char)*(num+1));
    fread(Info->Extension,num,1,fp);
    Info->UnitSet=(CompressNode*) malloc(sizeof (CompressNode)*(Info->UnitNum));
    fread(Info->UnitSet,sizeof (CompressNode)*(Info->UnitNum),1,fp);
    int i=0;
    for(i=0;i<Info->UnitNum;++i){

        fread(&num,sizeof (int),1,fp);
        Info->UnitSet[i].unit=(char*) malloc(sizeof (char )*(num+1));
        fread(Info->UnitSet[i].unit, num,1,fp);

        fread(&num,sizeof (int),1,fp);
        Info->UnitSet[i].HuffCode=(char*) malloc(sizeof (char )*(num+1));
        fread(Info->UnitSet[i].HuffCode, num,1,fp);
    }
    return Info;
}
char*GenCompressString(FILE*fp,double basicUnitSize,char*preString,CompressInfo*CInfo){//由原串得到huffman串
    char *temp;
    char *temp1;
    char *ger1;
    char*ger2;
    char*ger3;
    ger1=ReadString(fp,basicUnitSize,CInfo->FileSize);//读字符串，长度为BasicNum*2字节，保证整字节
    if((ftell(fp) >= CInfo->FileSize)) {//到文件末，证明读到的是尾串
        temp = StringCombina(CInfo->UnitSet[CInfo->UnitNum-1].HuffCode,NULL);
    } else{//证明读到的非尾串
        ger2 = BiChConverse(ger1, 1, (int)(basicUnitSize*2));
        free(ger1);
        temp1= StringCut(ger2,(int)(basicUnitSize*2*8)/2-1);
        temp = StringCombina(CInfo->UnitSet[FindInMap(temp1,CInfo)].HuffCode,CInfo->UnitSet[FindInMap(ger2,CInfo)].HuffCode);
        free(temp1);
        free(ger2);
    }
    ger3=preString;
    ger2=temp;
    temp = StringCombina(preString,temp);
    free(ger2);
    if(strlen(preString)>0)free(ger3);
    return temp;
}
HuffmanNode *HuffSearch(HuffmanNode *node,CompressInfo*CInfo,char*preString){
    if(strlen(preString)<CInfo->bit||node->code==Node_SINGAL){
        exit(114514);
    }else{
        char *now= StringCut(preString,CInfo->bit-1);
        unsigned int a=BinaryToDemical(now);
        return &node->Child[a];
    }
}
void DeCompress(char*oriPath,char*tarPath){
    printf("decompressing\n");
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
            char*te1;
            te1=StringCut(preBiString, 8*(floor(strlen(preBiString)/8))-1);
            write=BiChConverse(te1,0,0);
            fwrite(write, sizeof (char), strlen(te1)/8,ftar);
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
            char*tep2;
            tep2=StringCut(preBiString, 8*(floor(strlen(preBiString)/8)-1));
            write=BiChConverse(tep2,0,0);
            fwrite(write, sizeof (char), strlen(tep2)/8,ftar);
        }
    }
    fclose(fori);
    fclose(ftar);
    printf("decompress done\n");
}
#endif //HF_DECOMPRESS_H
