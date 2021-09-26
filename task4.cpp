#include <set>
#include <vector>
#include <sstream>
#include <iostream>

struct Printer {

    std::stringstream newStringStream;

    std::string str() const {
        return newStringStream.str();
    }

    template<typename T>
    Printer &format(const T &t) {
        newStringStream << t;
        return *this;
    }

    template<typename T>
    Printer &format(const std::set<T> &t) {
        brackets(t, '{', '}');
        return *this;
    }

    template<typename T>
    Printer &format(const std::vector<T> &t) {
        brackets(t, '[', ']');
        return *this;
    }

    template<typename First, typename Second>
    Printer &format(const std::pair<First, Second> &t) {
        newStringStream << "( ";
        format(t.first).format(", ").format(t.second);
        newStringStream << " )";
        return *this;
    }

    template<typename... T>
    Printer &format(const std::tuple<T...> &t) {
        newStringStream << "( ";

        bool isFirst = true;
        std::apply(
                [this, &isFirst](const auto &...item) {
                    (std::invoke([&] { // [&] для захвата из первой лямбда функции
                        if (isFirst) { isFirst = false; }
                        else { newStringStream << ", "; }
                        format(item);
                    }), ...);
                }, t
        );

        newStringStream << " )";
        return *this;
    }

    template<typename T>
    Printer &brackets(const T &t, char openBracket, char closeBracket) {
        newStringStream << openBracket << ' ';

        bool isFirst = true;
        for (const auto &value : t) {
            if (isFirst) { isFirst = false; }
            else { newStringStream << ", "; }
            format(value);
        }

        newStringStream << ' ' << closeBracket;
        return *this;
    }

};

template<typename T>
std::string format(const T &t) {
    return Printer().format(t).str();
}

int main() {
    std::tuple<std::string, int, int> t = {"xyz", 1, 2};
    std::vector<std::pair<int, int> > vp = {{1, 4},
                                            {5, 6}};
    std::vector<std::tuple<std::set<int>, std::vector<int>>> vtsv = {{{2, 4,  8, 16}, {4, 16, 64}},
                                                                     {{5, 25, 125},   {3, 9,  27, 81}}};
    std::string s1 = Printer().format(" vector: ").format(vp).str();
    std::string s2 = Printer().format(t).format(" ! ").format(0).str();
    std::string s3 = format(vtsv);
    assert(s1 == " vector: [ ( 1, 4 ), ( 5, 6 ) ]");
    assert(s2 == "( xyz, 1, 2 ) ! 0");
    assert(s3 == "[ ( { 2, 4, 8, 16 }, [ 4, 16, 64 ] ), ( { 5, 25, 125 }, [ 3, 9, 27, 81 ] ) ]");
    std::cout << s1 << std::endl << s2 << std::endl << s3;
}