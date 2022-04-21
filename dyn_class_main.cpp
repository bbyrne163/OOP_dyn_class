#include <iostream>
#include <initializer_list>
#include <map>
#include <functional>
#include <any>

class Dyn_class
{
public:
    Dyn_class()
    {

    }
    template <typename T>
    Dyn_class(std::initializer_list<T*> a_list);

    template <typename T>
    void add_func(T* a_func);

    template <typename T>
    void remove_func(T* a_func);

    template <typename T>
    T* operator()(T* a_key);

private:
    // Stores all functions
    std::map<void*, std::any*> m_functions;
};

// Add a function with any number of parameters
template <typename T>
void Dyn_class::add_func(T* a_func)
{
    std::any* temp = new std::any(a_func);
    m_functions[(void*)a_func] = temp;
}

// Remove a function with any number of parameters
template <typename T>
void Dyn_class::remove_func(T* a_key)
{
    if (m_functions.find((void*)a_key) == m_functions.end())
    {
        std::cout << "Function doesn't exist" << std::endl;
        return;
    }

    delete m_functions.at((void*)a_key);
    m_functions.erase((void*)a_key);
}

template <typename T>
T* Dyn_class::operator()(T* a_key)
{
    if (m_functions.find((void*)a_key) == m_functions.end())
    {
        std::cout << "Function doesn't exist" << std::endl;
        return nullptr;
    }
    return std::any_cast<T*>(*m_functions.at((void*)a_key));
}

// Test Functions
void print() { std::cout << "Hello World." << std::endl; }
int mult(int a) { return 2 * a; }
char increment(char a_string) { return a_string + 1; }
std::string message(std::string a_string) { return a_string + " are fun"; }

class A
{
public:
    int f(int x, int y)
    {
        return x * y;
    }
};

int main()
{
    Dyn_class d;
    // d(function)(a)(b);

    auto lam = [](int x) { return x * 2; };
    std::function func_obj = increment;

    // Add functions
    d.add_func(print);
    d.add_func(mult);
    d.add_func(message);
    d.add_func(&lam);
    d.add_func(&func_obj);

    // Access functions
    d(print)();
    std::cout << d(mult)(4) << std::endl;
    std::cout << d(message)("Ducks") << std::endl;
    std::cout << (*d(&lam))(4) << std::endl;
    std::cout << (*d(&func_obj))('a') << std::endl;

    // Remove function
    d.remove_func(print);
    d(print)();

    /*
    Storing Member functions

    A a1;
    a1.A::f(2, 3);
    auto member = &A::f;
    d.add_func(&member);
    void (A:: * q)(int, int) = *d(&member);
    std::cout << q << std::endl;
    */

    return 0;
}