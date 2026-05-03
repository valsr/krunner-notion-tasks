#pragma once

#include <QString>

template<typename T>
struct ApiResult {
    enum class Code { Ok, Error };

    Code    code    = Code::Error;
    QString message;
    T       value   = {};

    bool isOk()    const { return code == Code::Ok; }
    bool isError() const { return code == Code::Error; }

    static ApiResult ok(T val = {})           { return {Code::Ok,    {},  std::move(val)}; }
    static ApiResult err(const QString &msg)  { return {Code::Error, msg, {}}; }
};
