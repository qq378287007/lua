#include <math.h>

#include "lua.hpp"

#ifdef __cplusplus
extern "C" {
#endif
__declspec(dllexport) int luaopen_libmylib(lua_State* L);
#ifdef __cplusplus
}
#endif

#define TYPE_BOOLEAN 1
#define TYPE_NUMBER 2
#define TYPE_STRING 3


static int l_sin(lua_State *L)
{
    double d = luaL_checknumber(L, 1);
    lua_pushnumber(L, sin(d));  // push result
    return 1;  // number of results
}


static int ladd(lua_State *L){
    double op1 = luaL_checknumber(L, -2);
    double op2 = luaL_checknumber(L, -1);
    lua_pushnumber(L, op1+op2);
    return 1;
}

static int lsub(lua_State *L){
    double op1 = luaL_checknumber(L, -2);
    double op2 = luaL_checknumber(L, -1);
    lua_pushnumber(L, op1-op2);
    return 1;
}

static int lavg(lua_State *L){
    double avg = 0.0;
    int n = lua_gettop(L);
    if(n==0){
        lua_pushnumber(L,0);
        return 1;
    }
    int i;
    for(i=1;i<=n;i++){
        avg += luaL_checknumber(L, i);
    }
    avg = avg/n;
    lua_pushnumber(L,avg);
    return 1;
}

static int fn(lua_State *L){
    int type = lua_type(L, -1);
    printf("type = %d\n", type);
    if(type==LUA_TBOOLEAN){
        lua_pushvalue(L, lua_upvalueindex(TYPE_BOOLEAN));
    } else if(type==LUA_TNUMBER){
        lua_pushvalue(L, lua_upvalueindex(TYPE_NUMBER));
    } else if(type==LUA_TSTRING){
        lua_pushvalue(L, lua_upvalueindex(TYPE_STRING));
    }
    return 1;
}





/* 需要一个"luaL_Reg"类型的结构体，其中每一个元素对应一个提供给Lua的函数。
 * 每一个元素中包含此函数在Lua中的名字，以及该函数在C库中的函数指针。
 * 最后一个元素为“哨兵元素”（两个"NULL"），用于告诉Lua没有其他的函数需要注册。
 */
static const struct luaL_Reg mylib[] = {
{"mysin", l_sin},
{"add", ladd},
{"sub", lsub},
{"avg", lavg},
{NULL, NULL},
};

/* 此函数为C库中的“特殊函数”。
 * 通过调用它注册所有C库中的函数，并将它们存储在适当的位置。
 * 此函数的命名规则应遵循：
 * 1、使用"luaopen_"作为前缀。
 * 2、前缀之后的名字将作为"require"的参数。
 */
int luaopen_libmylib(lua_State* L)
{
    /* void luaL_newlib (lua_State *L, const luaL_Reg l[]);
     * 创建一个新的"table"，并将"l"中所列出的函数注册为"table"的域。
     */
    luaL_newlib(L, mylib);

    lua_pushliteral(L, "BOOLEAN");
    lua_pushliteral(L, "NUMBER");
    lua_pushliteral(L, "STRING");
    lua_pushcclosure(L, fn, 3);

    lua_setfield(L, -2, "fn");



    return 1;
}
