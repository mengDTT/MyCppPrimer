#include <iostream>
using namespace std;


int main()
{
    const char ca[] = {'h','e','l','l','o'};  // ע��˴�û��'\0'�����ʱ�����⣬֪������һ��'\0'��ֹͣ
    const char *cp = ca;
    while(*cp){
        cout << *cp << endl;
        ++cp;
    }
    return 0;
}