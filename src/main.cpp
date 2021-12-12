
#include <iostream>
#include <type_traits>
#include <vector>
#include <list>

/**
 * @brief Проверяет, является ли тип вектором или листом
 */
template<typename T>
struct is_vector_or_list
{
    static const bool value = false;
};
/**
 * @brief Частичная специализация для std::vector
 */
template<typename U>
struct is_vector_or_list<std::vector<U> >
{
    static const bool value = true;
};
/**
 * @brief Частичная специализация для std::list
 */
template<typename U>
struct is_vector_or_list<std::list<U> >
{
    static const bool value = true;
};

/**
 * @brief Печать айпи, специализация для целочисленных типов и std::string
 */
template <typename T>
void print_ip(T _ip, typename std::enable_if<!is_vector_or_list<T>::value>::type* = nullptr)
{
    if constexpr (std::is_same<T, std::string>::value)
    {
        std::cout << _ip << std::endl;
    }
    else
    {
        _ip = _ip < 0 ? -_ip : _ip;
        std::cout << (((_ip / 256) / 256) / 256) % 256 << '.'
                  << ((_ip / 256) / 256) % 256 << '.'
                  << (_ip / 256) % 256 << '.'
                  << _ip % 256 << std::endl;
    }
}
/**
 * @brief Печать айпи, специализация для std::vector и std::list
 */
template <typename T>
void print_ip(T _ip, typename std::enable_if<is_vector_or_list<T>::value>::type* = nullptr)
{
    if (_ip.size() != 4)
        return;

    auto i = _ip.cbegin();
    for (; i != std::prev(_ip.cend()); std::advance(i, 1))
    {
        std::cout << *i << '.';
    }
    std::cout << *i << std::endl;
}


int main (int, char **)
{
    print_ip(char(-1));
    print_ip(short(0));
    print_ip(int(2130706433));
    print_ip(long(8875824491850138409));
    print_ip(std::string("192.168.0.0"));
    print_ip(std::vector<int>{192, 168, 0, 1});
    print_ip(std::list<int>{192, 168, 0, 2});

    return 0;
}
