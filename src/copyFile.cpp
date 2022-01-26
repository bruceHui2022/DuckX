#include <fstream>
#include <iostream>

/*****************************************************************************************
Function:       CopyFile
Description:    复制文件
Input:          SourceFile:原文件路径 NewFile:复制后的文件路径
Return:         1:成功 0:失败
******************************************************************************************/
int CopyFile(char *SourceFile, char *NewFile) {
    std::ifstream in;
    std::ofstream out;

    try {
        in.open(SourceFile, std::ios::binary); //打开源文件
        if (in.fail())                         //打开源文件失败
        {
            std::cout << "Error 1: Fail to open the source file." << std::endl;
            in.close();
            out.close();
            return 0;
        }
        out.open(NewFile, std::ios::binary); //创建目标文件
        if (out.fail())                      //创建文件失败
        {
            std::cout << "Error 2: Fail to create the new file." << std::endl;
            out.close();
            in.close();
            return 0;
        } else //复制文件
        {
            out << in.rdbuf();
            out.close();
            in.close();
            return 1;
        }
    } catch (std::exception e) {
    }
}