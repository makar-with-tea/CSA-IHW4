#include <cctype>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <string>
#include <queue>
#include <unistd.h>
#include <vector>

pthread_mutex_t mutex;
const int dep_num = 3; // количество отделов в магазине
std::vector<std::queue<int>> queues(dep_num); // очереди 3-х отделов
int n, finished, min_t, max_t, max_d;
std::vector<sem_t> semaphores;
std::ofstream fout;

// функция, возвращающая целое число в пределах [min; max]
int my_rand(int min, int max) {
    return min + std::rand() % (max - min + 1);
}

// функция, вызывающая sleep для случайного времени - от 3 до 7 секунд
void my_sleep() {
    sleep(my_rand(min_t, max_t));
}

void my_print(std::string s) {
    std::cout << s << '\n';
    fout << s << '\n';
}

bool str_isdigit(std::string s) {
    for (char e : s) {
        if (!std::isdigit(e)) {
            return false;
        }
    }
    return true;
}

void* f_customer(void *args){
    int n = *(int*)args; // номер покупателя
    for (int i = 0; i < my_rand(1, max_d); ++i) { // каждый покупатель посещает от 1 до max_d отделов, возможны повторы
        int department = my_rand(1, dep_num); // номер посещаемого отдела - от 1 до dep_num (то есть 3)
        pthread_mutex_lock(&mutex); // захват мьютекса для работы с очередью
        queues[department - 1].push(n);
        my_print("Покупатель " + std::to_string(n) + " встал в очередь в отделе " + std::to_string(department));
        pthread_mutex_unlock(&mutex); // высвобождение мьютекса, конец работы с очередью
        sem_wait(&semaphores[n - 1]); // ожидание обслуживания от продавца данного отдела
        my_sleep(); // время, которое покупатель тратит на дорогу до следующего отдела, поиск товаров или выход из магазина
    }
    ++finished; // показываем, что данный покупатель покинул магазин
    return nullptr;
}

void* f_department(void *args){
    int dep = *(int*)args; // номер отдела
    while (finished < n) { // цикл работает, пока не все покупатели завершили покупки
        if (queues[dep - 1].empty()) continue;
        my_sleep();
        pthread_mutex_lock(&mutex); // захват мьютекса, начало работы с очередью
        int customer = queues[dep - 1].front();
        queues[dep - 1].pop(); // удаление обслуженного покупателя из очереди
        my_print("Отдел " + std::to_string(dep) + " обслужил покупателя " + std::to_string(customer));
        sem_post(&semaphores[customer - 1]);
        pthread_mutex_unlock(&mutex); // высвобождение мьютекса, конец работы с очередью
    }
    return nullptr;
}

int main(int argc, char* argv[]) {
    // для достижения получений более случайных чисел используем текущее время как seed
    std::srand(std::time(nullptr)); 
    
    std::string filename;
    std::string pos = argv[1];
    if (!str_isdigit(pos)) { // первый аргумент - не число => это имя файла
        std::ifstream in(pos);
        std::string cur_s;
        in >> cur_s;
        n = std::stoi(cur_s);
        in >> cur_s;
        min_t = std::stoi(cur_s);
        in >> cur_s;
        max_t = std::stoi(cur_s);
        in >> cur_s;
        max_d = std::stoi(cur_s);
        in >> filename;
        
    } else {
        n = std::stoi(argv[1]);
        min_t = std::stoi(argv[2]);
        max_t = std::stoi(argv[3]);
        max_d = std::stoi(argv[4]);
        filename = argv[5];
    }
    
    fout.open(filename); // связываем fout с файлом с именем filename
    
    // проверка корректности введенных числовых данных
    if (n <= 0) {
        my_print("Число покупателей должно быть больше 0, попробуйте снова.");
        return 0;
    }
    if (min_t <= 0) {
        my_print("Минимальное число секунд процесса должно быть больше 0, попробуйте снова.");
        return 0;
    }
    if (max_t <= 0) {
        my_print("Максимальное число секунд процесса должно быть больше 0, попробуйте снова.");
        return 0;
    }
    if (max_d <= 0) {
        my_print("Максимальное число посещаемых покупателем отделов должно быть больше 0, попробуйте снова.");
        return 0;
    }
    
    semaphores = std::vector<sem_t>(n);
    finished = 0;
    std::vector<pthread_t> threads(n); // треды для покупателей
    std::vector<int> r(n); // номера покупателей
    for (int i = 0; i < n; ++i) {
        sem_init(&semaphores[i], 0, 0); // инициализация семафоров покупателей
    }
    pthread_mutex_init(&mutex, nullptr); // инициализация мьютекса
    
    for(int i = 0; i < n; ++i) {
        r[i] = i + 1;
        pthread_create(&threads[i], nullptr, f_customer, (void*)&r[i]); // создание тредов покупателей
    }
    
    pthread_t departments[dep_num];
    int r_deps[dep_num]; // номера отделов
    for(int i = 0; i < dep_num; ++i) {
        r_deps[i] = i + 1;
        pthread_create(&departments[i], nullptr, f_department, (void*)&r_deps[i]); // создание тредов отделов
    }
    
    for(int i = 0; i < dep_num; ++i) {
        pthread_join(departments[i], nullptr); // завершение тредов отделов
    }
    for(int i = 0; i < n; ++i){
        pthread_join(threads[i], nullptr); // завершение тредов покупателей
    }
    pthread_mutex_destroy(&mutex); // удаление мьютекса
    for (int i = 0; i < n; ++i) {
        sem_destroy(&semaphores[i]); // удаление семафоров
    }
    my_print("КОНЕЦ РАБОТЫ МАГАЗИНА");
    fout.close(); // закрываем файл
}