#include <iostream>
#include <ctime>
using namespace std;

#include "expParserImpl.h"

int main()
{
    clock_t start;
    clock_t end;

    ExpParserImpl vars;

    start = clock();
    cout << "****add****" << endl;
    vars.addVariableNum("a", 6);
    vars.addVariableExp("b", "1+2+3*a");
    vars.addVariableNum("c", 10);


   auto ok = vars.addVariableExp("a0", "60");

    //vars.removeVariable("a");
    auto t = vars.getVariableNum("a");
    t = vars.getVariableNum("b");
    auto m = vars.evalExp("b+c");

    vars.addVariableExp("c", "1+2+3*a+b");
    vars.addVariableExp("d", "1+2+3.25+cos(b)");
    vars.addVariableExp("e", "1+2+3*c+b");
    vars.addVariableExp("f", "1+2+3+b-a+9+(-c*d)");
    for (int i = 0; i < 9999; i++)
    {
        vars.addVariableExp(QString("var%1").arg(i), QString("%1+a").arg(i));
    }
    cout << "****add****" << endl;
    end = clock();
    cout << "add time: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
    cout << "\n\n";

    start = clock();
    cout << "****remove****" << endl;
    vars.removeVariable("c");
    cout << "****remove****" << endl;
    end = clock();
    cout << "remove time: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
    cout << "\n\n";

    start = clock();
    cout << "****update****" << endl;
    vars.updateVariableExp("a", "2*8");
    cout << "****update****" << endl;
    end = clock();
    cout << "update time: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
    cout << "\n\n";

    start = clock();
    cout << "****eval****" << endl;
    for (int i = 0; i < 9999; i++)
    {
        QString exp{QString("sin(cos(var%1 - a) + b) - d*a").arg(i)};
        QPair<bool, double> result = vars.evalExp(exp);
        cout << QString("%1 = %2").arg(exp).arg(result.second).toStdString() << endl;
    }
    cout << "****eval****" << endl;
    end = clock();
    cout << "eval time: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
    cout << "\n\n";




    cout << "Over!\n";

    return 0;
}
