#include <iostream>
#include <ctime>
using namespace std;

#include "expParserImpl.h"


/*
lua_pop(L, num)函数从栈顶开始移除。

当num>0时从栈顶移除指定个数。
当num=0时栈不受影响
当num=-1时栈中元素全部移除
其他负数逻辑就有点乱了，建议不要使用其他(<-1)负值。最终的解释权还得看lua_settop.
#define lua_pop(L,n)        lua_settop(L, -(n)-1)

当n为-1时 lua_settop(L,0) 移除所有元素 ，正常
当n<-1 时，-(n)-1为正数，堆栈可能会高于原来的堆栈，此时却压入了新的栈，lua不会报错。违背了lua_pop的初衷。
当n>0时，-(n)-1 等于负数，从栈顶开始移除n个参数，但是 -n-1不得超过栈底，会报错，及n不能超过当前栈中个数。
如果不想从栈顶为起点移除除元素，而是移除从栈底起指定元素到栈顶的所有元素，直接用lua_settop即可。
*/

/*
lua_settop函数说明
该函数用于指定栈的高度，栈只能从栈顶压栈，不能从栈底添加数据。所以栈底的数据会保持不变。
当新的高度大于原来的高度时，会从栈顶压入数据，压入的数据不可用(因为是随机的)。
当新的高度小于原来的高度时，会从栈顶移除多余的元素。
当输入参数为负数时，表示从栈顶开始的索引（最栈顶元素为-1）。

该函数会移除栈顶到该元素之间的所以元素。-1则无，-2 则移除-1 。
-3则移除-1，-2。以此类推。
但是负数编号不能超出栈底的负数索引，超出会抛出异常。
lua_pop函数及是使用了该特性。
*/

int main()
{
    if(0)
    {
        lua_State *m_LuaStater = luaL_newstate();
        luaL_openlibs(m_LuaStater);

        QString str{"sin = math.sin; cos = math.cos;"};
        luaL_dostring(m_LuaStater, str.toStdString().c_str());
        cout <<"top: " << lua_gettop(m_LuaStater) <<endl;

        /*
        int stackSize = 30;
        lua_checkstack(m_LuaStater, stackSize);

        for(int i=0; i<stackSize; i++){//栈大小默认44
            lua_pushinteger(m_LuaStater, i);
            cout <<"top: " << lua_gettop(m_LuaStater) <<endl;
        }
        */

        /*
        //出栈20个数据
        //lua_pop(m_LuaStater, 20);
        //cout <<"top: " << lua_gettop(m_LuaStater) <<endl;

        //获取指定栈位置数据，不会出栈
        //int value = lua_tointeger(m_LuaStater, -2);
        //cout <<"value: " <<value <<endl;
        //cout <<"top: " << lua_gettop(m_LuaStater) <<endl;

        //指定栈中数据个数，0清空栈
        //lua_settop(m_LuaStater, 0);
        //cout <<"top: " << lua_gettop(m_LuaStater) <<endl;

*/


        string m_Name = "a";



        /*
//设置变量
        //栈顶压入数据
        lua_pushinteger(m_LuaStater, 12);
        cout <<"top: " << lua_gettop(m_LuaStater) <<endl;

        //栈顶数据赋值给变量，会出栈
        lua_setglobal(m_LuaStater, m_Name.c_str());
        cout <<"top: " << lua_gettop(m_LuaStater) <<endl;
    */

        /*
        //获取变量，变量值(变量不存在时为nil)会压入栈顶，根据类型判断变量是否存在
        //int type = lua_getglobal(m_LuaStater, m_Name.c_str());
        //cout <<"top: " << lua_gettop(m_LuaStater) <<endl;

        //获取栈顶数据，不出栈
        int value = lua_tointeger(m_LuaStater, -1);
        cout <<"value: " <<value <<endl;
        cout <<"top: " << lua_gettop(m_LuaStater) <<endl;

        //栈顶数据出栈
        lua_pop(m_LuaStater, 1);
        cout <<"top: " << lua_gettop(m_LuaStater) <<endl;
*/


        //a = 25; dreturn 1+2;，表达式运行错误时a不会被赋值
        //a = 25, 栈顶不变
        //return，栈顶+1
        int errorCode = luaL_dostring(m_LuaStater, "a = 25; return 1+2;");
        cout <<"top: " << lua_gettop(m_LuaStater) <<endl;

        int value = lua_tointeger(m_LuaStater, -1);
        cout <<"value: " <<value <<endl;

        if (errorCode != LUA_OK)
        {
            const char *errorMessage = lua_tostring(m_LuaStater, -1);
            cout<<"error: " <<string(errorMessage) <<endl;
        }

        lua_pop(m_LuaStater, 1);
        cout <<"top: " << lua_gettop(m_LuaStater) <<endl;




        int type = lua_getglobal(m_LuaStater, m_Name.c_str());
        cout <<"top: " << lua_gettop(m_LuaStater) <<endl;

        //获取栈顶数据，不出栈
        value = lua_tointeger(m_LuaStater, -1);
        cout <<"value: " <<value <<endl;
        cout <<"top: " << lua_gettop(m_LuaStater) <<endl;

        //栈顶数据出栈
        lua_pop(m_LuaStater, 1);
        cout <<"top: " << lua_gettop(m_LuaStater) <<endl;



        /*
        //栈+1
        luaL_dostring(m_LuaStater, "1+2");
        cout <<"top: " << lua_gettop(m_LuaStater) <<endl;

        value = lua_tointeger(m_LuaStater, -1);
        cout <<"value: " <<value <<endl;
        cout <<"top: " << lua_gettop(m_LuaStater) <<endl;


        //栈+1
        luaL_dostring(m_LuaStater, "return 2+3");
        cout <<"top: " << lua_gettop(m_LuaStater) <<endl;


        //栈不变
        luaL_dostring(m_LuaStater, "a = 1+2");
        cout <<"top: " << lua_gettop(m_LuaStater) <<endl;
        */


        lua_close(m_LuaStater);

    }


    if(1){
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


        cout<< vars.getVariableNum("b").second <<endl;
        QPair<bool, double> result = vars.evalScript("if(11>100)then return 11;else return 22; end");
        cout << QString("%1 = %1").arg(result.second).toStdString() << endl;
        cout<< vars.getVariableNum("a").second<<endl;


        string script{"_testA = 22; return 256;"};
        result = vars.evalScript(script.c_str());
        cout << QString("rusult = %1").arg(result.second).toStdString() << endl;
        cout<< vars.getVariableNum("_testA").second<<endl;

        script = "";

        //多个变量赋值
        for(int i=0; i<1000; i++)
            script += "a" + to_string(i) + "=255+sin(a)-cos(b);";

        //多个变量运算
        script += "a=";
        for(int i=0; i<1000; i++)
            script += "sin(a" + to_string(i) + ")+";
        script += "6;";

        script += "return 123";

        result = vars.evalScript(script.c_str());
        cout << QString("rusult = %1").arg(result.second).toStdString() << endl;

        for(int i=0; i<1000; i++)
            cout<< vars.getVariableNum(("a" + to_string(i)).c_str()).second<<endl;
        cout<< vars.getVariableNum("a").second<<endl;
    }



    cout << "Over!\n";
    return 0;
}
