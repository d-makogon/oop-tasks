#pragma once

#include <map>
#include <memory>

template<typename T>
class Factory
{
public:
    template<typename TDerived>
    void Register(const std::string& name)
    {
        static_assert(std::is_base_of<T, TDerived>::value,
                      "Factory::Register doesn't accept this type because it doesn't derive from base class");

        _createFuncs[name] = createFunc<TDerived>;
    }

    std::unique_ptr<T> Create(const std::string& name)
    {
        auto it = _createFuncs.find(name);
        if (it != _createFuncs.end())
        {
            return it->second();
        }
        return nullptr;
    }

private:
    using TCreateFunc = std::unique_ptr<T>(*)();

    template <typename TDerived>
    static std::unique_ptr<T> createFunc()
    {
        return std::make_unique<TDerived>();
    }

    std::map<std::string, TCreateFunc> _createFuncs;
};
