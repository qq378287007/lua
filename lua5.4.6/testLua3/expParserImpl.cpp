#include "expParserImpl.h"

//#include <cmath>

#include <math.h>

static int l_cot(lua_State *L)
{
    // 如果给定虚拟栈中索引处的元素可以转换为数字，则返回转换后的数字，否则报错。
    double d = luaL_checknumber(L, 1);
    lua_pushnumber(L, cos(d) / sin(d));  // push result

    /* 这里可以看出，C可以返回给Lua多个结果，
     * 通过多次调用lua_push*()，之后return返回结果的数量。
     */
    return 1;  // number of results
}




static int l_log10(lua_State *L)
{
    // 如果给定虚拟栈中索引处的元素可以转换为数字，则返回转换后的数字，否则报错。
    double d = luaL_checknumber(L, 1);
    lua_pushnumber(L, log10(d));  // push result

    /* 这里可以看出，C可以返回给Lua多个结果，
     * 通过多次调用lua_push*()，之后return返回结果的数量。
     */
    return 1;  // number of results
}

QString ExpParserImpl::error{"none"};

ExpParserImpl::ExpParserImpl()
{
    m_LuaStater = luaL_newstate();
    luaL_openlibs(m_LuaStater);

    QString str{"sin = math.sin; cos = math.cos;"
                "tan = math.tan; abs = math.abs;"
                "acos = math.acos; asin = math.asin;"
                "atan = math.atan; exp = math.exp;"
                "log = math.log; "
                "sqrt = math.sqrt; int = math.floor;"
                "min = math.min; max = math.max;"};
    luaL_dostring(m_LuaStater, str.toStdString().c_str());


    lua_pushcfunction(m_LuaStater, l_cot);    // 将C函数转换为Lua的"function"并压入虚拟栈。
    lua_setglobal(m_LuaStater, "cot");    // 弹出栈顶元素，并在Lua中用名为"mysin"的全局变量存储。

    //printf("top: %d\n", lua_gettop(m_LuaStater));

    lua_pushcfunction(m_LuaStater, l_log10);    // 将C函数转换为Lua的"function"并压入虚拟栈。
    lua_setglobal(m_LuaStater, "log10");    // 弹出栈顶元素，并在Lua中用名为"mysin"的全局变量存储。

    //printf("top: %d\n", lua_gettop(m_LuaStater));

    //默认栈大小是44
    lua_checkstack(m_LuaStater, 100);
}

ExpParserImpl::~ExpParserImpl()
{
    lua_close(m_LuaStater);
}

bool ExpParserImpl::addVariableNum(const QString &name, double num)
{
    const char *m_Name = name.toStdString().c_str();

    //获取变量，变量值（不存在时为nil）会压入栈顶
    int type = lua_getglobal(m_LuaStater, m_Name);
    if (type == LUA_TNUMBER)
    {
        //栈顶移除一个数据
        lua_pop(m_LuaStater, 1);

        //指定栈中数据个数，0清空栈
        //lua_settop(m_LuaStater, 0);

        error = QString("error: %1 exist").arg(name);
        return false;
    }

    //栈顶压入数据
    lua_pushnumber(m_LuaStater, num);

    //栈顶数据赋值给变量，会出栈
    lua_setglobal(m_LuaStater, m_Name);

    error = "none";
    return true;
}

bool ExpParserImpl::addVariableExp(const QString &name, const QString &exp)
{
    int type = lua_getglobal(m_LuaStater, name.toStdString().c_str());
    if (type == LUA_TNUMBER)
    {
        lua_pop(m_LuaStater, 1);

        error = QString("error: %1 exist").arg(name);
        return false;
    }

    QString str{QString("%1 = %2").arg(name).arg(exp)};
    int errorCode = luaL_dostring(m_LuaStater, str.toStdString().c_str());
    if (errorCode != LUA_OK)
    {
        const char *errorMessage = lua_tostring(m_LuaStater, -1);
        error = QString("error: %1").arg(errorMessage);

        lua_pop(m_LuaStater, 1);

        return false;
    }

    error = "none";
    return true;
}

bool ExpParserImpl::removeVariable(const QString &name)
{
    const char *m_Name = name.toStdString().c_str();
    int type = lua_getglobal(m_LuaStater, m_Name);
    if (type != LUA_TNUMBER)
    {
        lua_pop(m_LuaStater, 1);

        error = QString("error: %1 not exist").arg(name);
        return false;
    }

    // 变量赋值nil（删除）
    lua_pushnil(m_LuaStater);
    lua_setglobal(m_LuaStater, m_Name);

    error = "none";
    return true;
}

QPair<bool, double> ExpParserImpl::getVariableNum(const QString &name)
{
    int type = lua_getglobal(m_LuaStater, name.toStdString().c_str());
    if (type == LUA_TNUMBER)
    {
        //获取指定栈位置数据，不会出栈
        double value = lua_tonumber(m_LuaStater, -1);
        lua_pop(m_LuaStater, 1);

        error = "none";
        return qMakePair(true, value);
    }
    else
    {
        lua_pop(m_LuaStater, 1);

        error = QString("error: %1 not exist").arg(name);
        return qMakePair(false, 0.0);
    }
}

bool ExpParserImpl::updateVariableNum(const QString &name, double num)
{
    const char *m_Name = name.toStdString().c_str();
    int type = lua_getglobal(m_LuaStater, m_Name);
    if (type != LUA_TNUMBER)
    {
        lua_pop(m_LuaStater, 1);

        error = QString("error: %1 not exist").arg(name);
        return false;
    }
    lua_pop(m_LuaStater, 1);

    lua_pushnumber(m_LuaStater, num);
    lua_setglobal(m_LuaStater, m_Name);

    error = "none";
    return true;
}

bool ExpParserImpl::updateVariableExp(const QString &name, const QString &exp)
{
    const char *m_Name = name.toStdString().c_str();
    int type = lua_getglobal(m_LuaStater, m_Name);
    if (type != LUA_TNUMBER)
    {
        lua_pop(m_LuaStater, 1);

        error = QString("error: %1 not exist").arg(name);
        return false;
    }
    lua_pop(m_LuaStater, 1);

    QString str{QString("%1 = 0 + %2").arg(name).arg(exp)};
    int errorCode = luaL_dostring(m_LuaStater, str.toStdString().c_str());
    if (errorCode != LUA_OK)
    {
        const char *errorMessage = lua_tostring(m_LuaStater, -1);
        error = QString("error: %1").arg(errorMessage);

        lua_pop(m_LuaStater, 1);

        return false;
    }

    error = "none";
    return true;
}

QPair<bool, double> ExpParserImpl::evalExp(const QString &exp)
{
    //return，栈顶+1
    QString str{QString("return 0 + %1").arg(exp)};
    int errorCode = luaL_dostring(m_LuaStater, str.toStdString().c_str());
    if (errorCode != LUA_OK)
    {
        const char *errorMessage = lua_tostring(m_LuaStater, -1);
        error = QString("error: %1").arg(errorMessage);

        lua_pop(m_LuaStater, 1);

        return qMakePair(false, 0.0);
    }

    double value = lua_tonumber(m_LuaStater, -1);
    lua_pop(m_LuaStater, 1);

    if(isnan(value)){
        error = "not a number";
        return qMakePair(false, value);
    }


    if(isinf(value)){
        error = "infinite";
        return qMakePair(false, value);
    }

    error = "none";
    return qMakePair(true, value);
}

//Script中必须有return
//"if(a>10)then return a;else return b; end"
QPair<bool, double> ExpParserImpl::evalScript(const QString &script)
{
    int cur_top = lua_gettop(m_LuaStater);
    //a = 25; 栈顶不变
    //a = 25; dreturn 1+2;，表达式运行错误时a不会被赋值
    int errorCode = luaL_dostring(m_LuaStater, script.toStdString().c_str());
    if (errorCode != LUA_OK)
    {
        const char *errorMessage = lua_tostring(m_LuaStater, -1);
        error = QString("error: %1").arg(errorMessage);

        lua_pop(m_LuaStater, 1);

        return qMakePair(false, 0.0);
    }

    if(cur_top == lua_gettop(m_LuaStater))
        return qMakePair(false, 0.0);

    double value = lua_tonumber(m_LuaStater, -1);
    lua_pop(m_LuaStater, 1);

    if(isnan(value)){
        error = "not a number";
        return qMakePair(false, value);
    }


    if(isinf(value)){
        error = "infinite";
        return qMakePair(false, value);
    }

    error = "none";
    return qMakePair(true, value);
}
