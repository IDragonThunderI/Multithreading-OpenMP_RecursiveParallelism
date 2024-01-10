#include <iostream>
#include <random>
#include <vector>
#include <array>
#include <cmath>

void randomizeArray(std::vector<long long>& array, std::mt19937& randomGenerator) {
	#pragma omp parallel for
		for (auto element = array.begin(); element != array.end(); element++) {
			std::uniform_int_distribution<int> distribution(0, 100);
			*element = distribution(randomGenerator);
		}
}

void printArray(const std::vector<long long>& array) {
	std::cout << "Array = { ";
	for (const auto& element : array) {
		std::cout << element << " ";
	}
	std::cout << "}" << std::endl << std::endl;
}

void binaryPrintArray(const std::vector<long long>& array) {
	std::string bin;
	std::cout << "Array = { ";
	for (auto element : array) {
		while (element) {
			bin += char('0' + element % 2);
			element /= 2;
		}
		std::cout << std::string(bin.crbegin(), bin.crend()) << " ";
		bin = "";
	}
	std::cout << "}" << std::endl << std::endl;
}

void radixSort(std::vector<long long>& array) {
	unsigned maxDigits = 0;
	long long count;
	unsigned currentDigit;

	#pragma omp parallel for reduction(max: maxDigits) shared(array)
		for (auto element = array.begin(); element != array.end(); element++) {
			if ((count = ceil(log2(*element))) > maxDigits) {
				maxDigits = count;
			}
		}

	for (int i = 0; i < maxDigits; i++) {
		std::array<std::vector<long long>, 2> buffer;
		for (const auto& element : array) {
			currentDigit = (element & (1ll << i)) != 0;
			buffer.at(currentDigit).push_back(element);
		}
		array.clear();
		for (auto& subbuffer : buffer) {
			array.insert(array.end(), subbuffer.begin(), subbuffer.end());
		}
	}
}

int main() {
	setlocale(LC_ALL, "ru_RU.UTF-8");
	std::mt19937 randomGenerator;
	std::random_device device;
	randomGenerator.seed(device());

	unsigned arraySize;
	std::cout << "Введите размер массива: ";
	std::cin >> arraySize;
	std::cout << "Инициализация массива..." << std::endl;
	std::vector<long long> array(arraySize);

	std::cout << "Заполнение массива..." << std::endl;
	randomizeArray(array, randomGenerator);
	if (arraySize <= 100) {
		std::cout << "Исходный массив: " << std::endl;
		printArray(array);
		std::cout << "Исходный массив в двоичном представлении: " << std::endl;
		binaryPrintArray(array);
	}

	std::cout << "Сортировка массива..." << std::endl;
	radixSort(array);
	if (arraySize <= 100) {
		std::cout << "Отсортированный массив в двоичном представлении: " << std::endl;
		binaryPrintArray(array);
		std::cout << "Отсортированный массив: " << std::endl;
		printArray(array);
	}

	system("pause");
	return 0;
}