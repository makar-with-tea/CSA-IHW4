# Макаревич Мария, БПИ228
Индивидуальное домашнее задание 4 по дисциплине "Архитектура вычислительных систем".
**Работа выполнена на 9 баллов.**
## Вариант 13. Условие:
Первая задача о магазине. В магазине работают три отдела, каждый отдел обслуживает один продавец. Покупатель, зайдя в магазин, делает покупки в одном или нескольких произвольных отделах, обходя их в произвольном (случайном) порядке. Если в выбранном отделе продавец не свободен, покупатель становится в очередь и ожидает, пока продавец не освободится. Создать многопоточное приложение, моделирующее рабочий день магазина. Каждого покупателя и продавцов моделировать отдельными потоками. Размер очереди не оговаривается. Считается, что для данной задачи она не ограничена (но моделирование должно быть в разумных пределах).
## Критерии на 4-5 баллов
- Соблюдены общие требования к отчету.
- В отчете должен быть приведен сценарий, описывающий одновременное поведение представленных в условии задания сущностей в терминах предметной области. То есть, описан сценарий, задающий ролевое поведение субъектов и объектов задачи (интерпретация условия с большей степенью детализации происходящего), а не то, как это будет реализовано в программе.
- Описана модель параллельных вычислений, используемая при разработке многопоточной программы.
- Описаны входные данные программы, включающие вариативные диапазоны, возможные при многократных запусках.
- Реализовано консольное приложение, решающее поставленную задачу с использованием одного варианта изученных синхропримитивов.
- Ввод данных в приложение реализован с консоли во время выполнения программы (без использования аргументов командной строки).
- Для используемых генераторов случайных чисел описаны их диапазоны и то, как интерпретируются данные этих генераторов.
- Вывод программы должен быть информативным, отражая все ключевые протекающие в ней события в терминах предметной области. Наблюдатель на основе вывода программы должен понимать, что в ней происходит в каждый момент времени ее работы.
- В программе присутствуют комментарии, поясняющие выполняемые действия и описание используемых объектов и переменных.

**Сценарий**
Покупатель выбирает желаемый отдел, набирает товары и подходит к продавцу отдела, дабы их купить. Если продавец занят, покупатель занимает очередь и ждет, пока он освободится. После покупки товаров в данном отделе покупатель либо покидает магазин, либо возвращается в тот же отдел, чтобы докупить какие-то забытые товары, либо переходит в другой отдел. Процесс покупки и перехода занимает какое-то ненулевое время. В магазине параллельно работают 3 отдела с продавцами (по одному в каждом), а также находится ненулевое число покупателей, совершающих покупки и перемещающихся параллельно и независимо друг от друга.

**Модель**
Отделы и покупателей моделируют отдельные потоки. Взаимодействие происходит по приципу "Клиенты и серверы": клиентский поток (покупатель) запрашивает сервер (отдел) и ждет ответа. Серверный поток (отдел) ожидает запроса от клиента (покупателя), затем действует в соответствии с поступившим запросом.

Входные данные, запрашиваемые от пользователя, включают в себя:
- число покупателей;
- минимальное и максимальное число секунд, которое может занимать процесс (покупка, переход в другой отдел и т.д.);
- максимальное число отделов, посещаемых одним покупателем, с возможностью повторения.
Входные данные проверяются, неположительные числа не допускаются. Ввод данных реализован через командную строку (см. критерий на 6-7 баллов).

В программе использованы такие синхропримитивы, как мьютексы и семафоры. Вывод программы показывает все ключевые события, в ней происходящие. Комментарии в программе присутствуют. Диапазоны и логика работы генератора случайных чисел описана в комментариях в самой программе.

## Критерии на 6-7 баллов
- В отчете подробно описан обобщенный алгоритм, используемый при реализации программы исходного словесного сценария. В котором показано, как на программу отображается каждый из субъектов предметной области.
- В программу добавлена генерация случайных данных в допустимых диапазонах.
- Реализован ввод исходных данных из командной строки при запуске программы вместо ввода параметров с консоли во время выполнения программы.

**Обобщенный алгоритм**

Генерация случайных данных добавлена: функции my_rand, my_sleep. Ввод исходных данных (n, min_t, max_t, max_d) реализован через командную строку.

## Критерии на 8 баллов
- В программу, наряду с выводом в консоль, добавлен вывод результатов в файл. Имя файла для вывода данных задается в командной строке как один из ее параметров.
- Результаты работы программы должны выводиться на экран и записываться в файл.
- Наряду с вводом исходных данных через командную строку добавлен альтернативный вариант их ввода из файла, который по сути играет роль конфигурационного файла. Имя этого файла задается в командной строке вместо параметров, которые в этом случае не вводятся. Управление вводом в командной строке осуществляется с использованием соответствующих ключей.
- Приведено не менее трех вариантов входных и выходных файлов с различными исходным данными и результатами выполнения программы.
- Ввод данных из командной строки расширен с учетом введенных изменений.


## Критерии на 9 баллов
- 

## Критерии на 10 баллов
- 
## Тесты
Корректность работы программы подтверждается прохождением 4 тестов из папки tests, а также верными результатами работы тестирующей программы на 6 других тестах из [tests.asm](code/tests.asm).

## Котики
