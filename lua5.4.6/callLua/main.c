#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "lua.hpp"


/*
1栈底
-1栈顶

//压入元素
void lua_pushnil(lua_State *L);

void lua_pushnumber(lua_State *L, double n);
void lua_pushinteger(lua_State *L, long long n);

const char *lua_pushlstring (lua_State *L, const char *s, size_t len);
const char *lua_pushstring (lua_State *L, const char *s);
const char *lua_pushvfstring (lua_State *L, const char *fmt, va_list argp);
const char *lua_pushfstring (lua_State *L, const char *fmt, ...);

void lua_pushcclosure (lua_State *L, lua_CFunction fn, int n);
void lua_pushboolean (lua_State *L, int b);
void lua_pushlightuserdata (lua_State *L, void *p);
int lua_pushthread (lua_State *L);


//检测栈空间
int  lua_checkstack(lua_State *L, int n); //检查栈上空余的格子是否满足n个
void luaL_checkstack(lua_State *L, int sz, const char *msg); //检查栈上空余格子数,不满足则抛出一个异常信息


//检查栈中元素类型
int lua_isnumber (lua_State *L, int idx); //idx是索引位置
int lua_isstring (lua_State *L, int idx);
int lua_iscfunction (lua_State *L, int idx);
int lua_isinteger (lua_State *L, int idx);
int lua_isuserdata (lua_State *L, int idx);

int lua_type (lua_State *L, int idx);//将得到的值和LUA_TNIL,LUA_TNUMBER等比较来获取类型
const char* lua_typename(lua_State *L, int idx);//返回类型名称"nil","boolean","number"等


//获取栈上元素
lua_Number lua_tonumberx (lua_State *L, int idx, int *isnum);//isnum表示返回是否是number类型
lua_Integer lua_tointegerx (lua_State *L, int idx, int *isnum);
#define lua_tonumber(L,i) lua_tonumberx(L,(i),NULL )//直接得到number
#define lua_tointeger(L,i) lua_tointegerx(L,(i),NULL)

int  lua_toboolean (lua_State *L, int idx);
const char* lua_tolstring  (lua_State *L, int idx, size_t *len);
lua_CFunction lua_tocfunction (lua_State *L, int idx);
void *(lua_touserdata (lua_State *L, int idx);
lua_State *lua_tothread (lua_State *L, int idx);
const void*lua_topointe) (lua_State *L, int idx);


//操作栈元素
int  lua_gettop (lua_State *L); //返回栈上元素的个数

void lua_settop (lua_State *L, int idx); //将idx处元素设置为栈顶,多的删除,少的用nil补齐
#define lua_pop(L, n) lua_settop(L, - (n) - 1)//弹出栈顶n个元素

void lua_pushvalue (lua_State *L, int idx); //将idx处元素复制一个并放到栈顶

void lua_rotate (lua_State *L, int idx, int n);//将从idx到栈顶的元素朝着栈顶轮转n个位置,n为负数时表示朝栈底轮转
#define lua_remove(L, idx) (lua_rotate(L, (idx), -1), lua_pop(L, 1)) //移除idx处的元素
#define lua_insert(L, idx) lua_rotate(L, (idx), 1) //将栈顶处的元素旋转到idx处

void lua_copy (lua_State *L, int fromidx, int toidx) //将fromidx处元素复制并覆盖toidx处元素
#define lua_replace(L,idx) (lua_copy(L, -1, (idx)), lua_pop(L, 1)) //用栈顶元素复制并覆盖到idx处,并将栈顶元素弹出


//全局变量
int lua_getglobal (lua_State *L, const char *name);//将name对应的值入栈,并返回该值的类型
void lua_setglobal (lua_State *L, const char *name);//从栈顶弹出一个值,并将该值设置为全局变量name的新值


//表
int lua_gettable (lua_State *L, int idx);//取t[k]的值并入栈,t是idx对应位置的表,k是栈顶的值.将k出栈,然后将t[k]入栈,并返回值的类型,可能触发index元方法
int lua_getfield (lua_State *L, int idx, const char *k);//取t[k]的值并入栈,t是idx对应位置的表.返回入栈值的类型,可能触发index元方法
int lua_geti (lua_State *L, int idx, lua_Integer n);//取t[n]的值并入栈,t是idx对应位置的表.返回入栈值的类型,可能触发index元方法

int lua_rawget (lua_State *L, int idx);//类似于lua_gettable,只是不触发元方法
int lua_rawgeti (lua_State *L, int idx, lua_Integer n);

int lua_settable (lua_State *L, int idx);//等价于执行t[k]=v, t是idx处的表,v是栈顶处的值,k是栈顶下面的值.可能触发newindex元方法
int lua_setfield (lua_State *L, int idx, const char *k);//等价于执行t[k]=v, t是idx处的表,v是栈顶处的值.可能触发newindex元方法
int lua_seti (lua_State *L, int idx, lua_Integer n);//等价于执行t[n]=v, t是idx处的表,v是栈顶处的值.可能触发newindex元方法

int lua_rawset (lua_State *L, int idx);//类似于lua_settable,不触发newindex元方法
int lua_rawseti (lua_State *L, int idx, lua_Integer n);

void  lua_createtable (lua_State *L, int narr, int nrec);//创建一个新的表并入栈,narr为建议序列元素个数,nrec为序列外建议个数
#define lua_newtable(L) lua_createtable(L, 0, 0)


//调用函数
int lua_pcall(lua_State *L, int nargs, int nresults, int msgh);
调用lua_pcall前, 需将要调用的函数, 参数依次入栈.
nargs表示参数个数, nresults表示返回值个数, Lua会自动裁剪或者用nil补齐返回值.
lua_pcall将函数和所有参数出栈, 再将所有返回值依次入栈.

如果调用过程中出现错误,lua_pcall会返回一个错误码, 并尝试调用一个错误处理函数, msgh可以指定一个错误处理函数的位置,为0时表示使用默认的错误处理函数.错误处理函数会将一个错误消息入栈.

不管是调用函数还是进行其他的操作,都有义务去保持栈和操作前一样.



//数组
void lua_rawgeti (lua_State *L, int index, int key);
void lua_rawseti (lua_State *L, int index, int key);
index 指向 table 在栈中的位置；key 指向元素在 table 中的位置






lua_State* L = luaL_newstate(); //堆栈指针
lua_close(L); //释放lua资源


lua_load()函数 //当这个函数返回0时表示加载
luaL_loadfile(filename) //只加载lua文件, 0表示没有错误。
luaL_dofile //加载且执行lua文件, 0表示没有错误。
*/

void error (lua_State *L, const char *fmt, ...) {
    va_list argp;

    va_start(argp, fmt);
    vfprintf(stderr, fmt, argp);
    va_end(argp);

    lua_close(L);

    exit(EXIT_FAILURE);
}

//Dump栈
static void stackDump (lua_State *L) {
    int i;
    int top = lua_gettop(L); //获取栈上元素个数
    for (i = 1; i <= top; i++) {
        int t = lua_type(L, i);
        switch (t) {
        case LUA_TSTRING : {
            printf("'%s'", lua_tostring(L, i));
            break;
        }
        case LUA_TBOOLEAN : {
            printf(lua_toboolean(L, i) ? "true" : "false");
            break;
        }
        case LUA_TNUMBER : { //TNUMBER包含两种类型 float和integer
            if (lua_isinteger(L, i)) //integer
                printf("%lld", lua_tointeger(L, i));
            else
                printf("%g", lua_tonumber(L, i));
            break;
        }
        default : {
            printf("%s", lua_typename(L, t));
            break;
        }
            printf("  ");
        }
        printf("\n");
    }
}



void callLua(){
    lua_State *L = luaL_newstate(); /* opens Lua */

    luaopen_base(L); /* opens the basic library */
    luaopen_table(L); /* opens the table library */
    luaopen_io(L); /* opens the I/O library */
    luaopen_string(L); /* opens the string lib. */
    luaopen_math(L); /* opens the math lib. */

    char buff[256];
    while (fgets(buff, sizeof(buff), stdin) != NULL) {
        int error = luaL_loadbuffer(L, buff, strlen(buff),  "line") || lua_pcall(L, 0, 0, 0);
        if (error) {
            fprintf(stderr, "%s\n", lua_tostring(L, -1));
            lua_pop(L, 1);/* pop error message from the stack */
        }
    }
    lua_close(L);

    lua_pushnil(L);
}

int callua2(){
    lua_State *L = luaL_newstate();

    luaL_openlibs(L);//打开状态机L中所有Lua标准库

    //int retLoad = luaL_loadfile(L, "test.lua");//加载
    int retLoad = luaL_dofile(L, "test.lua");//加载且执行

    if (retLoad == 0)
        printf("load file success retLoad: %d\n", retLoad);
    else if(lua_pcall(L, 0, 0, 0))
    {
        printf("error %s\n", lua_tostring(L, -1));
        return 1;
    }

    lua_getglobal(L, "name"); // 获取全局变量name并压栈
    lua_getglobal(L, "age");  // 获取全局变量age，并返回栈顶

    // 注意读取顺序
    int age = lua_tointeger(L, -1);         // 栈顶
    const char *name = lua_tostring(L, -2); // 次栈顶
    printf("name = %s\n", name);
    printf("age = %d\n", age);


    //调用函数,依次压入参数
    lua_getglobal(L, "add");
    lua_pushnumber(L, 10.0);
    lua_pushnumber(L, 20.0);

    //查看压入栈的元素
    for (int i=1; i<=2; i++)
        printf("number: %f\n", lua_tonumber(L, -i));

    //2个输入参数，1个函数值，0表示错误处理函数在栈中的索引值，压入结果前会弹出参数和函数
    int pcallRet = lua_pcall(L, 2, 1, 0); //将计算好的值压入栈顶,并返回状态值
    if (pcallRet != 0)
    {
        printf("error %s\n", lua_tostring(L, -1));
        return 1;
    }
    printf("pcallRet: %d\n", pcallRet);

    double val = lua_tonumber(L, -1); //在栈顶取出数据
    printf("val: %f\n", val);
    lua_pop(L, -1); //弹出栈顶

    //再次查看栈内元素，发现什么都没有，因为lua在返回函数计算值后会清空栈,只保留返回值
    for (int i=1; i<=2;i++)
        printf("number: %f\n", lua_tonumber(L, -i));


    //将全局变量mytable压入栈
    lua_getglobal(L, "mytable");
    //压入表中的key
    lua_pushstring(L, "name");

    //lua_gettable会在栈顶取出一个元素并且返回把查找到的值压入栈顶
    lua_gettable(L, 1);
    const char *name2 = lua_tostring(L, -1); //在栈顶取出数据
    printf("name: %s\n", name2);

    lua_pushstring(L,"id");//压入id
    lua_gettable(L, 1);//在lua mytable表中取值返回到栈顶
    int id = lua_tonumber(L, -1); //在栈顶取出数据
    printf("id: %d\n", id);

    luaL_dostring(L, "print(\"print by lual\")");

    luaL_dostring(L, "return 1+2+3");
    int value = lua_tonumber(L, -1); //在栈顶取出数据
    printf("value: %d\n", value);


    luaL_dostring(L, "age = 2 ^ 5;");
    //luaL_dostring(L, "age = math.exp(5);");
    lua_getglobal(L, "age");
    age = lua_tointeger(L, -1);
    printf("age: %d\n", age);


    lua_close(L);
    return 0;
}


int luaadd(int x, int y)
{
    lua_State* L = luaL_newstate();  /* 创建lua状态机 */

    luaL_openlibs(L);   /* 打开Lua状态机中所有Lua标准库 */

    luaL_dofile(L, "test.lua"); /*加载lua脚本*/

    lua_getglobal(L, "add"); /*函数名*/
    lua_pushnumber(L, x); /*参数入栈*/
    lua_pushnumber(L, y); /*参数入栈*/

    lua_call(L, 2, 1); /*开始调用函数，有2个参数，1个返回值*/
    int sum = (int)lua_tonumber(L, -1); /*取出返回值*/

    /*
    if (lua_pcall(L, 2, 1, 0) != LUA_OK)
        error(L, "error running function 'f': %s", lua_tostring(L, -1));
    int isnum;
    int sum = lua_tonumberx(L, -1, &isnum);//获取返回值
    if (!isnum)
        error(L, "function 'f' shound return a number");
*/

    lua_pop(L, 1); /*清除返回值的栈*/

    lua_close(L);/*清除Lua*/

    return sum;
}

void test(){
    int sum = luaadd(99, 10);
    printf("The sum is %d \n", sum);

}


void callLua3(){
    lua_State *L = luaL_newstate();
    lua_pushboolean(L, 1);
    lua_pushnumber(L, 10);
    lua_pushnil(L);
    lua_pushstring(L, "hello");

    //stackDump(L);                       // print: true 10 nil  'hello'

    //复制-4处元素并压栈
    lua_pushvalue(L, -4); //stackDump(L); // print: true 10 nil  'hello' true

    //复制栈顶元素至3处并出栈
    lua_replace(L, 3);    //stackDump(L); // print: true 10 true 'hello'

    //将6处元素设置为栈顶, 多的删除, 少的用nil补齐
    lua_settop(L, 6);     //stackDump(L); // print: true 10 true 'hello' nil nil

    //将从3到栈顶的元素朝着栈顶轮转n个位置,n为负数时表示朝栈底轮转
    lua_rotate(L, 3, 1);  //stackDump(L); // print: true 10 nil  true 'hello' nil

    //移除3处的元素
    lua_remove(L, -3);   // stackDump(L); // print: true 10 nil 'hello' nil

    //将-5处元素设置为栈顶, 多的删除, 少的用nil补齐
    lua_settop(L, -5);    stackDump(L); // print: true

    lua_close(L);
}



void callLua4(){
    lua_State *L = luaL_newstate(); //创建一个状态机

    lua_pushnil(L); //nil
    int type = lua_type(L, -1);
    printf("nil type = %d\n", type);
    if(lua_isnil(L, -1)){
        printf("------nil-----\n");
    }

    lua_pushboolean(L, 0); //boolean
    type = lua_type(L, -1);
    printf("boolean type = %d\n", type);
    if(lua_isboolean(L, -1))
        printf("--------boolean------\n");

    lua_pushlightuserdata(L, NULL); //lightuserdata
    type = lua_type(L, -1);
    printf("lightuserdata type = %d\n", type);
    if(lua_islightuserdata(L, -1))
        printf("--------lightuserdata------\n");

    lua_pushnumber(L, 10); //number
    type = lua_type(L, -1);
    printf("number type = %d\n", type);
    if(lua_isnumber(L, -1))
        printf("--------number------\n");

    lua_pushstring(L, "string"); //string
    type = lua_type(L, -1);
    printf("string type = %d\n", type);
    if(lua_isstring(L, -1))
        printf("--------string------\n");

    lua_newtable(L); //table, 创建空表，并压入栈
    type = lua_type(L, -1);
    printf("table type = %d\n", type);
    if(lua_istable(L, -1))
        printf("--------table------\n");

    lua_newuserdata(L, 1024); //userdata, 分配1024大小的内存块，并把内存地址压入栈
    type = lua_type(L, -1);
    printf("userdata type = %d\n", type);
    if(lua_isuserdata(L, -1))
        printf("--------userdata------\n");

    lua_pushthread(L); //thread, 创建一个lua新线程,并将其压入栈。lua线程不是OS线程
    type = lua_type(L, -1);
    printf("thread type = %d\n", type);
    if(lua_isthread(L, -1))
        printf("--------thread------\n");

    lua_close(L); //关闭状态机
}


//加载运行lua脚本
void callLua5(){
    lua_State *L = luaL_newstate(); //创建一个状态机
    luaL_openlibs(L); //打开所有lua标准库

    int ret = luaL_loadfile(L, "test.lua"); //加载但不运行lua脚本
    if(ret != LUA_OK){
        const char *err = lua_tostring(L, -1); //加载失败，会把错误信息压入栈顶
        printf("-------loadfile error = %s\n", err);
        lua_close(L);
        return ;
    }

    ret = lua_pcall(L, 0, 0, 0); //保护模式调用栈顶函数
    if(ret!=LUA_OK){
        const char *err = lua_tostring(L, -1); //发生错误，会把唯一值(错误信息)压入栈顶
        printf("-------pcall error = %s\n", err);
        lua_close(L);
        return ;
    }

    lua_close(L);
}

//操作Lua中全局变量
void callLua6(){

    lua_State *L = luaL_newstate(); //创建一个状态机
    luaL_openlibs(L); //打开所有lua标准库



    lua_getglobal(L, "var"); //获取全局变量var的值并压入栈顶
    int var = lua_tonumber(L, -1);
    printf("var = %d\n", var);

    lua_pushnumber(L, 10);
    lua_setglobal(L, "var"); //设置全局变量var为栈顶元素的值，即10

    lua_pushstring(L, "c str");
    lua_setglobal(L, "var2"); //设置全局变量var2为栈顶元素的值，即c str

    lua_getglobal(L, "f");
    lua_pcall(L, 0, 0, 0);

    lua_close(L);
}


void dump_table(lua_State *L, int index){
    if(lua_type(L, index)!=LUA_TTABLE)
        return;
    // 典型的遍历方法
    lua_pushnil(L);  //nil入栈，相当于从表的第一个位置遍历
    while(lua_next(L, index)!=0){ //没有更多元素，lua_next返回0
        //key-value入栈， key位于栈上-2处，value位于-1处
        printf("%s-%s\n", lua_typename(L,lua_type(L,-2)), lua_typename(L,lua_type(L,-1)));
        lua_pop(L,1); //弹出一个元素，即把value出栈，此时栈顶为key，供下一次遍历
    }
}

//操作Lua中的table
void callLua7(){

    lua_State *L = luaL_newstate(); //创建一个状态机
    luaL_openlibs(L); //打开所有lua标准库



    // 读取table
    lua_getglobal(L, "t");
    lua_getfield(L, 1, "a");  //从索引为1的位置(table)获取t.a，并压栈
    lua_getfield(L, 1, "b");
    lua_getfield(L, -1, "c"); //从索引为-1的位置(栈顶)获取t.c，并压栈

    // 修改table
    lua_settop(L, 1); //设置栈的位置为1，此时栈上只剩一个元素t
    lua_pushnumber(L, 10);
    lua_setfield(L, 1, "a"); //t.a=10
    lua_pushstring(L, "hello c");
    lua_setfield(L, 1, "e"); //t.e="hello c"

    dump_table(L, 1); //遍历table

    //新建一个table
    lua_settop(L, 0); //清空栈
    lua_newtable(L); //创建一个table
    lua_pushboolean(L, 0);
    lua_setfield(L, 1, "new_a");
    lua_pushboolean(L, 1);
    lua_setfield(L, 1, "new_b");

    dump_table(L, 1); //遍历table

    lua_close(L);
}



//传递Lua多个参数
void callLua8(){
    lua_State* g_L =  g_L = luaL_newstate();//创建Lua栈

    lua_checkstack(g_L, 60);//修改Lua栈大小为60，防止在C和Lua之间传递大数据时，崩溃

    luaL_openlibs(g_L);//运行Lua虚拟机

    if(luaL_loadfile(g_L, "test.lua"))//装载lua脚本
    {
        printf("load test.lua fail.");
        return;
    }
    else
    {
        printf("load test.lua ok.");
        if(lua_pcall(g_L,0,0,0))//运行lua脚本
        {
            printf("run test.lua fail.%s", lua_tostring(g_L, -1));
            return ;
        }
        else
        {
            printf("run test.lua ok.");
        }
    }


    lua_getglobal(g_L, "set_data");    // 获取lua函数 set_data

    int array[10] = {11,22,33,44,55,66,77,88,99,100};

    for(int n=0;n<10;n++)
    {
        lua_pushinteger(g_L, array[n]);        //将数组的数据入栈
    }

    if(lua_pcall(g_L, 10, 0, 0) != 0)//调用lua函数 set_data
        printf("error running function 'set_data': %s", lua_tostring(g_L, -1));

    lua_pop(g_L, 1);


}



//Lua返回多个参数
void callLua9(){

    lua_State* g_L =  g_L = luaL_newstate();//创建Lua栈

    lua_checkstack(g_L, 60);//修改Lua栈大小为60，防止在C和Lua之间传递大数据时，崩溃

    luaL_openlibs(g_L);//运行Lua虚拟机

    if(luaL_loadfile(g_L, "test.lua"))//装载lua脚本
    {
        printf("load test.lua fail.");
        return;
    }
    else
    {
        printf("load test.lua ok.");
        if(lua_pcall(g_L,0,0,0))//运行lua脚本
        {
            printf("run test.lua fail.%s", lua_tostring(g_L, -1));
            return ;
        }
        else
        {
            printf("run test.lua ok.");
        }
    }


    lua_getglobal(g_L, "get_data");    // 获取lua函数 set_data

    if(lua_pcall(g_L, 0, 1, 0) != 0)
        printf("error running function 'get_data': %s", lua_tostring(g_L, -1));



    int index = lua_gettop(g_L);//获取Lua栈顶的内容的索引值

    lua_pushnil(g_L);//nil 入栈作为初始 key

    if(lua_isnil(g_L, index))//栈顶内容为空
    {
        return ;
    }

    while(lua_next(g_L, index))//循环遍历栈中内容
    {
        printf("%d ", lua_tonumber(g_L, -1));//取出栈顶内容

        lua_pop(g_L, 1);//弹出栈顶内容，然后下一个数据处在栈顶
    }
}


/*
 表操作
background = {r=0.30, g=0.10, b=0}

lua_getglobal(L, "background");
if (!lua_istable(L, -1))
 error(L, "`background' is not a valid color table");

red = getfield("r");
green = getfield("g");
blue = getfield("b");

// assume that table is on the stack top *
int getfield (const char *key) {

 lua_pushstring(L, key);
 lua_gettable(L, -2); // get background[key]
 //上面两行可以替换为：lua_gettable(L, -1, key)

if (!lua_isnumber(L, -1))
 error(L, "invalid component in background color");
 int result = (int)lua_tonumber(L, -1) * MAX_COLOR;
 lua_pop(L, 1); // remove number
return result;
}

*/

int main()
{
    //test();
    callLua4();
    return 0;
}
