// 1006.cpp : Defines the entry point for the console application.
// Do the Untwist
// http://acm.zju.edu.cn/onlinejudge/showProblem.do?problemCode=1006

#include <iostream>
#include <string>

using namespace std;

const int CODE_COUNT = 28;
const int TEXT_MIN_VALUE = (int)'.';
const int TEXT_MAX_SIZE = (int)'z' + 1;
const int TEXT_MIN_SIZE = TEXT_MAX_SIZE - TEXT_MIN_VALUE;


int main(int argc, char* argv[])
{
    char pcCode2Char[CODE_COUNT] = { '_' };
    int pnChar2Code[TEXT_MAX_SIZE] = { 0 };
    int i = 0, nKey = 0;
    std::string strCipher, strPlain;

    // 初始化码表

    for (i=1; i<(CODE_COUNT-1); i++)
    {
        pcCode2Char[i] = 'a' + (char)(i - 1);
        pnChar2Code[pcCode2Char[i]] = i;
    }
    pcCode2Char[i] = '.';
    pnChar2Code[pcCode2Char[i]] = i;
    // 循环处理输入字串
    // 边界条件：
    //  （1）a twisted message containing at least one and at most 70 characters
    //  （2）the key k will be a positive integer not greater than 300
    //  （3）a line containing only the number 0 that signals the end
    // rule: '_' = 0, 'a' = 1, 'b' = 2, ..., 'z' = 26, and '.' = 27
    // ciphercode[i] = (plaincode[(k * i) mod n] - i) mod 28
    // 输入为 ciphercode，需要将 ciphercode 解码为 plaincode
    while (std::cin >> nKey && nKey != 0 && std::cin >> strCipher)
    {
        int n = (int)strCipher.size();

        if (nKey > 300 || n <= 0 || n > 70)
            continue;
        // 第 i 个字符的解码过程
        // twist_index = (k * i) mod n
        // cipher_code = pnChar2Code[strCipher[i]]
        // plain_code = (cipher_code + i) mod 28
        // strPlain[twist_index] = pcCode2Char[plain_code]
        strPlain.resize(n);
        for (i=0; i<n; i++)
        {
            strPlain[(nKey * i) % n] = pcCode2Char[(pnChar2Code[strCipher[i]] + i) % CODE_COUNT];
        }
        std::cout << strPlain << std::endl;
    }
	return 0;
}