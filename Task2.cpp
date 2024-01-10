#include <iostream>
#include <random>
#include <vector>

void init(std::vector<int>& array, std::mt19937& randomGenerator) {
	#pragma omp parallel for
		for (auto element = array.begin(); element != array.end(); element++) {
			std::uniform_int_distribution<int> distribution(-100, 100);
			*element = distribution(randomGenerator);
		}
}

void print(const std::vector<int>& array) {
	std::cout << "array = { ";
	for (const auto& element : array) {
		std::cout << element << " ";
	}
	std::cout << "}" << std::endl << std::endl;
}

void Qsort(std::vector<int>& array, const int& f, const int& l) {
	int first = f, last = l;
	int separator = array.at((first + last) / 2);

	while (first <= last) {
		while (array.at(first) < separator) {
			first++;
		}
		while (array.at(last) > separator) {
			last--;
		}
		if (first <= last) {
			std::swap(array.at(first), array.at(last));
			first++;
			last--;
		}
	}

	#pragma omp task shared(array)
	{
		if (f < last) {
			Qsort(array, f, last);
		}
	}
	#pragma omp task shared(array)
	{
		if (first < l) {
			Qsort(array, first, l);
		}
	}
	#pragma taskwait
}

void Hoar_sort(std::vector<int>& array) {
	Qsort(array, 0, array.size() - 1);
}

int main() {
	setlocale(LC_ALL, "ru_RU.UTF-8");
	std::mt19937 randomGenerator;
	std::random_device device;
	randomGenerator.seed(device());

	unsigned n;
	std::cout << "Введите размер массива: ";
	std::cin >> n;
	std::vector<int> array(n);

	init(array, randomGenerator);
	if (n <= 100) {
		std::cout << "Исходный массив: " << std::endl;
		print(array);
	}

	#pragma omp parallel shared(array)
	{
		#pragma omp single
		{
			Hoar_sort(array);
		}
	}

	if (n <= 100) {
		std::cout << "Отсортированный массив: " << std::endl;
		print(array);
	}

	system("pause");
	return 0;
}