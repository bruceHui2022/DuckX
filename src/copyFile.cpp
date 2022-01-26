#include <fstream>
#include <iostream>

/*****************************************************************************************
Function:       CopyFile
Description:    �����ļ�
Input:          SourceFile:ԭ�ļ�·�� NewFile:���ƺ���ļ�·��
Return:         1:�ɹ� 0:ʧ��
******************************************************************************************/
int CopyFile(char *SourceFile, char *NewFile) {
    std::ifstream in;
    std::ofstream out;

    try {
        in.open(SourceFile, std::ios::binary); //��Դ�ļ�
        if (in.fail())                         //��Դ�ļ�ʧ��
        {
            std::cout << "Error 1: Fail to open the source file." << std::endl;
            in.close();
            out.close();
            return 0;
        }
        out.open(NewFile, std::ios::binary); //����Ŀ���ļ�
        if (out.fail())                      //�����ļ�ʧ��
        {
            std::cout << "Error 2: Fail to create the new file." << std::endl;
            out.close();
            in.close();
            return 0;
        } else //�����ļ�
        {
            out << in.rdbuf();
            out.close();
            in.close();
            return 1;
        }
    } catch (std::exception e) {
    }
}