#include "expParserImpl.h"

QString ExpParserImpl::error{"none"};

ExpParserImpl::ExpParserImpl()
{
    m_LuaStater = luaL_newstate();
    luaL_openlibs(m_LuaStater);

    QString str{"sin = math.sin; cos = math.cos;"};
    luaL_dostring(m_LuaStater, str.toStdString().c_str());
}

ExpParserImpl::~ExpParserImpl()
{
    lua_close(m_LuaStater);
}

bool ExpParserImpl::addVariableNum(const QString &name, double num)
{
    const char *m_Name = name.toStdString().c_str();

    int type = lua_getglobal(m_LuaStater, m_Name);
    if (type == LUA_TNUMBER)
    {
        error = QString("error: %1 exist").arg(name);
        return false;
    }

    lua_pushnumber(m_LuaStater, num);
    lua_setglobal(m_LuaStater, m_Name);
    error = "none";
    return true;
}

bool ExpParserImpl::addVariableExp(const QString &name, const QString &exp)
{
    int type = lua_getglobal(m_LuaStater, name.toStdString().c_str());
    if (type == LUA_TNUMBER)
    {
        error = QString("error: %1 exist").arg(name);
        return false;
    }

    QString str{QString("%1 = %2").arg(name).arg(exp)};
    int errorCode = luaL_dostring(m_LuaStater, str.toStdString().c_str());
    if (errorCode != LUA_OK)
    {
        const char *errorMessage = lua_tostring(m_LuaStater, -1);
        lua_pop(m_LuaStater, 1);

        error = QString("error: %1").arg(errorMessage);
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
        double value = lua_tonumber(m_LuaStater, -1);
        error = "none";
        return qMakePair(true, value);
    }
    else
    {
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
        error = QString("error: %1 not exist").arg(name);
        return false;
    }

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
        error = QString("error: %1 not exist").arg(name);
        return false;
    }

    QString str{QString("%1 = %2").arg(name).arg(exp)};
    int errorCode = luaL_dostring(m_LuaStater, str.toStdString().c_str());
    if (errorCode != LUA_OK)
    {
        const char *errorMessage = lua_tostring(m_LuaStater, -1);
        lua_pop(m_LuaStater, 1);

        error = QString("error: %1").arg(errorMessage);
        return false;
    }

    error = "none";
    return true;
}

QPair<bool, double> ExpParserImpl::evalExp(const QString &exp)
{
    QString str{QString("return %1").arg(exp)};
    int errorCode = luaL_dostring(m_LuaStater, str.toStdString().c_str());
    if (errorCode != LUA_OK)
    {
        const char *errorMessage = lua_tostring(m_LuaStater, -1);
        lua_pop(m_LuaStater, 1);

        error = QString("error: %1").arg(errorMessage);
        return qMakePair(false, 0.0);
    }

    double value = lua_tonumber(m_LuaStater, -1);
    error = "none";
    return qMakePair(true, value);
}

QPair<bool, double> ExpParserImpl::evalScript(const QString &script)
{
    int errorCode = luaL_dostring(m_LuaStater, script.toStdString().c_str());
    if (errorCode != LUA_OK)
    {
        const char *errorMessage = lua_tostring(m_LuaStater, -1);
        lua_pop(m_LuaStater, 1);

        error = QString("error: %1").arg(errorMessage);
        return qMakePair(false, 0.0);
    }

    double value = lua_tonumber(m_LuaStater, -1);
    error = "none";
    return qMakePair(true, value);
}
