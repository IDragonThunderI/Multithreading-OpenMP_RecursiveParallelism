#include <iostream>
#include <random>

int randomNumber(std::mt19937& randomGenerator) {
	std::uniform_int_distribution<int> distribution(0, 100);
	return distribution(randomGenerator);
}

class BinTree { // Класс "Бинарное дерево"
private:
    struct Node { // Структура "Узел дерева"
        int data; // Поле данных
        Node* left = NULL; // Левый узел
        Node* right = NULL; // Правый узел
        Node* parent = NULL; // Родительский узел
    };
    void Add(int data, Node* current, Node* parent); // Рекурсивный метод добавления
    void RemoveSubtree(Node* subroot); // Удаление поддерева

public:
    Node* root; // Корень дерева
    BinTree() : root(NULL) {}; // Конструктор
    ~BinTree(); // Деструктор
    void AddNode(int data); // Добавление нового узла
    void Print(Node* node); // Вывод узлов дерева на экран
    int SumNodes(Node* root); // Сумма всех узлов дерева
};

BinTree::~BinTree() {
    RemoveSubtree(root);
}

void BinTree::RemoveSubtree(Node* subroot) {
    if (subroot != NULL) {
        RemoveSubtree(subroot->left); // Удаляем левую часть дерева.
        RemoveSubtree(subroot->right); // Удаляем правую часть дерева.

        delete subroot;
    }
}

void BinTree::AddNode(int data) {
    if (root == NULL) {
        root = new Node; // Выделяем пямять под корень дерева
        root->data = data; // Заполняем корень данными
        root->left = NULL;
        root->right = NULL;
        root->parent = NULL;
    } else {
        if (data < root->data) {
            Add(data, root->left, root);
        } else if (data > root->data) {
            Add(data, root->right, root);
        }
    }
}

void BinTree::Add(int data, Node* current, Node* parent) {
    if (current == NULL) {
        Node* temp = new Node; // Выделяем память под узел
        temp->data = data; // Заполняем узел данными
        temp->left = NULL;
        temp->right = NULL;
        temp->parent = parent; // Связываем узел с родительским

        if (temp->data <= parent->data) {
            parent->left = temp;
        } else if (temp->data > parent->data) {
            parent->right = temp;
        }
    } else if (data <= current->data) { // Условие добавления левого потомка
        Add(data, current->left, current);
    } else if (data > current->data) { // Условие добавления правого потомка
        Add(data, current->right, current);
    }
}

void BinTree::Print(Node* node) {
    if (node == NULL) {
        return;
    } else {
        Print(node->left);
        std::cout << node->data << " ";
        Print(node->right);
    }
}

int BinTree::SumNodes(Node* root) {
    int sum = 0;
    if (root != NULL) {
        sum += root->data;
        #pragma omp task shared(sum)
        {
            sum += SumNodes(root->left);
        }
        #pragma omp task shared(sum)
        {
            sum += SumNodes(root->right);
        }
        #pragma omp taskwait
    }
    return sum;
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    std::mt19937 randomGenerator;
	std::random_device device;
	randomGenerator.seed(device());

    unsigned treeSize;
	std::cout << "Введите количество узлов бинарного дерева: ";
	std::cin >> treeSize;
    
    BinTree tree;
    std::cout << "Заполнение бинарного дерева..." << std::endl;
    for (unsigned i = 0; i < treeSize; i++) {
        tree.AddNode(randomNumber(randomGenerator));
    }
    tree.Print(tree.root);
    std::cout << std::endl;

    unsigned result;
    #pragma omp parallel
    {
        #pragma omp single
        {
            result = tree.SumNodes(tree.root);
        }
    }
    std::cout << "Сумма узлов бинарного дерева: " << result << std::endl;

    system("pause");
    return 0;
}