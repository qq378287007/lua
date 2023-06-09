#pragma once

#include <QPair>
#include <QString>

#include "libLua/lua.hpp"

class ExpParserImpl
{
public:
    ExpParserImpl();
    ~ExpParserImpl();

    bool addVariableNum(const QString &name, double num);
    bool addVariableExp(const QString &name, const QString &exp);

    bool removeVariable(const QString &name);

    QPair<bool, double> getVariableNum(const QString &name);

    bool updateVariableNum(const QString &name, double num);
    bool updateVariableExp(const QString &name, const QString &exp);

    QPair<bool, double> evalExp(const QString &exp);
    QPair<bool, double> evalScript(const QString &script);

    inline static QString getError()
    {
        return error;
    }

private:
    lua_State *m_LuaStater;
    static QString error;
};
