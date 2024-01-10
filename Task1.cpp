#include <iostream>
#include <functional>
#include <cmath>
#include <iomanip>

// Рекурсивное интегрирование методом трапеций
double integrateTrapezoidalRecursive(const double& a, const double& b, const unsigned& n, const std::function<double (const double&)> function) {
    if (n <= 1) {
        return (function(a) + function(b)) * 0.5 * (b - a);
    }
    double h = (b - a) / n;

    return integrateTrapezoidalRecursive(a + h * 2, b, n - 2, function) + (function(a) + function(a + h)) * 0.5 * h;
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    double a, b;
    unsigned n;

    std::cout << "Введите нижнюю границу интегрирования (a): ";
    std::cin >> a;
    std::cout << "Введите верхнюю границу интегрирования (b): ";
    std::cin >> b;
    std::cout << "Введите количество разбиений (n): ";
    std::cin >> n;

    double even, odd, result;
    #pragma omp parallel shared(result)
    {
        #pragma omp single
        {
            #pragma omp task shared(odd)
            {
                odd = integrateTrapezoidalRecursive(a, b, n, [](const double& x) { return sin(x); });
            }
            #pragma omp task shared(even)
            {
                even = integrateTrapezoidalRecursive(a + (b - a) / n, b, n - 1, [](const double& x) { return sin(x); });
            }
            #pragma omp taskwait
            result = odd + even;
        }
    }
    std::cout << std::setprecision(16) << "Рекурсивное интегрирование методом трапеций: " << result << std::endl;

    system("pause");
	return 0;
}