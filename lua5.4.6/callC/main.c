#include <stdio.h>
#include <math.h>
#include <dirent.h>
#include <errno.h>

#include "lua.hpp"


/*

    luaL_checknumber(L, 1);

*/


static int l_sin(lua_State *L)
{
    // 如果给定虚拟栈中索引处的元素可以转换为数字，则返回转换后的数字，否则报错。
    double d = luaL_checknumber(L, 1);
    lua_pushnumber(L, sin(d));  // push result

    /* 这里可以看出，C可以返回给Lua多个结果，
     * 通过多次调用lua_push*()，之后return返回结果的数量。
     */
    return 1;  // number of results
}

static void test1(){

    lua_State *L = luaL_newstate();    // 创建Lua状态机。
    luaL_openlibs(L);    // 打开Lua状态机"L"中的所有Lua标准库。

    /* 这两句话还有更简单的方法：
         * lua_register(L, "mysin", l_sin)
         * 将C函数"l_sin"定义为Lua的全局变量"mysin"。
         * 其实现是如下宏：
         * #define lua_register(L,n,f) \
         *      (lua_pushcfunction(L, f), lua_setglobal(L, n))
         */
    lua_pushcfunction(L, l_sin);    // 将C函数转换为Lua的"function"并压入虚拟栈。
    lua_setglobal(L, "mysin");    // 弹出栈顶元素，并在Lua中用名为"mysin"的全局变量存储。

    const char* testfunc = "print(mysin(3.14 / 2))";

    if(luaL_dostring(L, testfunc))    // 执行Lua命令。
        printf("Failed to invoke.\n");

    lua_close(L);    // 关闭Lua状态机。
}


static int l_dir(lua_State *L)
{
    // 如果给定虚拟栈中索引处的元素可以转换为字符串，则返回转换后的字符串，否则报错。
    const char *path = luaL_checkstring(L, 1);

    DIR *dir = opendir(path);// open directory
    if(dir == NULL) {
        // 出错返回"nil"加上一个描述错误信息的字符串。
        lua_pushnil(L);
        lua_pushstring(L, strerror(errno));
        return 2;    // "nil"加上字符串，共两个返回值。
    }

    lua_newtable(L);// create result table
    struct dirent *entry;
    int i = 1;
    while((entry = readdir(dir)) != NULL)    // 逐一读取目录中的文件。
    {
        lua_pushnumber(L, i++);  // push key
        lua_pushstring(L, entry->d_name);  // push value
        lua_settable(L, -3);    // t[k] = v
    }

    closedir(dir);

    return 1;    // 返回值只有一个，"table"。
}

static void test2() {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    // 将C函数"l_dir"定义为Lua的全局变量"mydir"。
    lua_register(L, "mydir", l_dir);

    // 打印"/home/"目录下的所有文件。
    const char* testfunc = "for i, v in pairs(mydir('C:\\Users\\mali\\Desktop\\')) do print(i, v) end";

    if(luaL_dostring(L, testfunc))    // 执行Lua命令。
        printf("Failed to invoke.\n");

    lua_close(L);
}



static int l_SayHello(lua_State *L)
{
    const char *d = luaL_checkstring(L, 1);//获取参数，字符串类型
    char str[100] = "hello ";
    strcat(str, d);
    lua_pushstring(L, str);  /* 返回给lua的值压栈 */
    return 1;
}

static void test3() {
    lua_State *L = luaL_newstate();  /* 创建lua状态机 */
    luaL_openlibs(L);   /* 打开Lua状态机中所有Lua标准库 */

    lua_register(L, "SayHello", l_SayHello);//注册C函数到lua

    const char* testfunc = "print(SayHello('lijia'))";//lua中调用c函数
    if(luaL_dostring(L, testfunc))    // 执行Lua命令。
        printf("Failed to invoke.\n");

    /*清除Lua*/
    lua_close(L);
}

int main()
{
    test1();
    test2();
    test3();

    return 0;
}
