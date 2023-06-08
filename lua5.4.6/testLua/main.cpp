#include <string>
#include <iostream>
#include <cmath>
using namespace std;

#include "lua.hpp"

static int l_sin(lua_State *L)
{
    double d = luaL_checknumber(L, 1);
    lua_pushnumber(L, sin(d));  // push result
    return 1;  // number of results
}

static const luaL_Reg mylib[] = {
    {"mysin", l_sin},
    {NULL, NULL},
};

int luaopen_my(lua_State* L)
{
    luaL_newlib(L, mylib);
    return 1; //return one value
}

class LuaManager {
public:
    LuaManager(){
        L = luaL_newstate();

        luaL_openlibs(L);

        luaL_requiref(L, "my", luaopen_my, 1);
        lua_pop(L, 1);
    }
    ~LuaManager(){
        lua_close(L);
    }

    bool existVar(const string &name){
        int type = lua_getglobal(L, name.c_str());
        return type != 0;
    }

    double getVar(const string &name, bool *ok = nullptr){
        if(ok != nullptr)
            *ok = false;

        if(!existVar(name))
            return 0.0;

        if(ok != nullptr)
            *ok = true;
        return lua_tonumber(L, -1);
    }

    void setVar(const string &name, double value){
        lua_getglobal(L, name.c_str());
        lua_pushnumber(L, value);
        lua_setglobal(L, "a");
    }

    bool setVar(const string &name, const string &value){
        string str{name + " = " + value};
        return luaL_dostring(L, str.c_str()) == 0;
    }

    double eval(const string &expression, bool *ok = nullptr){
        if(ok != nullptr)
            *ok = false;

        string str{"return " + expression};
        if(luaL_dostring(L, str.c_str()))
            return 0.0;

        if(ok != nullptr)
            *ok = true;

        return lua_tonumber(L, -1);
    }

    double evalStr(const string &expression, bool *ok = nullptr){
        if(ok != nullptr)
            *ok = false;

        if(luaL_dostring(L, expression.c_str()))
            return 0.0;

        if(ok != nullptr)
            *ok = true;

        return lua_tonumber(L, -1);
    }

private:
    lua_State *L;
};

int main(){
    if(1)
    {
        LuaManager m_Lua;
        bool ok;
        double value;
        string expression;

        m_Lua.getVar("a", &ok);
        if(!ok){
            cout<<"a not exist\n";
        }

        m_Lua.setVar("a", 1.256);
        value = m_Lua.getVar("a", &ok);
        if(ok){
            cout<<"a exist\n";
            cout<<"a = " <<value <<endl;
        }

        ok = m_Lua.setVar("b", "10.2387");
        if(ok){
            cout<<"b set ok\n";
        }
        value = m_Lua.getVar("b", &ok);
        if(ok){
            cout<<"b exist\n";
            cout<<"b = " <<value <<endl;
        }

        expression = "a + b * 5 + 6.9 + math.sin(math.pi)";
        value = m_Lua.eval(expression, &ok);
        if(ok){
            cout<<expression <<" = " <<value <<endl;
        }

        expression = "a + b * 5 + 6.9 + my.mysin(math.pi)";
        value = m_Lua.eval(expression, &ok);
        if(ok){
            cout<<expression <<" = " <<value <<endl;
        }

        expression = "if(a>b) then return 2.5; else return 3.6;end";
        value = m_Lua.evalStr(expression, &ok);
        if(ok){
            cout<<expression <<" = " <<value <<endl;
        }
    }

    return 0;
}
