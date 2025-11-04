#ifndef S21_VECTOR_H_
#define S21_VECTOR_H_

#include <algorithm>  // отсюда берем для move copy методов
#include <cstddef>    // size_t для member types
#include <initializer_list>  // initializer_list для конструктора
#include <limits>  // numeric_limits  max() функция для capacity метода max_size
#include <stdexcept>
/* stdexcept для обработки исключений:
за пределы текущего размера out_of_range,
 пустого вектора runtime_error и
 bad_alloc для неудачного выделения памяти
*/
#include <utility>  // std::swap метод

namespace s21 {

template <typename T>
class vector {
 public:
  // Vector member types
  using value_type = T;  // опредление типа элемента
  using reference = T&;  // отсылка(ссылка) на элемент
  using const_reference = const T&;  // константная ссылка
  using iterator = T*;  // псевдоним(алиас) для вектора
  using const_iterator = const T*;  // константный
  using size_type = std::size_t;  // для определения размера контейнера (size_t
                                  // целочисленный супербольшой тип)

  // Vector member functions
  vector();  // пустой конструктор
  vector(size_type n);  // конструктор с явным параметром размера n
  vector(std::initializer_list<value_type> const&
             items);  // вектор с классом инит лист (!)

  vector(const vector& v);  // конструктор копирования с другого вектора
  vector(vector&& v) noexcept;  // конструктор перемещения с другого вектора и
                                // тот обнуляется
  ~vector();                    // деструктор
  vector& operator=(
      vector&& v) noexcept;  // оперетор присваивания для перемещения объекта

  // Vector Element access
  reference at(
      size_type pos);  // доступ к элементу по определенному индексу которая
                       // проверяет индекс не вышел ли за пределы массива
  reference operator[](size_type pos);  // доступ к элементу по определенному
                                        // индексу БЕЗ проверки сверху
  const_reference front();  // доступ к первому элементу
  const_reference back();   // к последнему
  value_type* data();  // прямой доступ к базовому массиву

  // Vector Iterators
  iterator begin();  // возвращает итератор к началу
  iterator end();    // к концу

  // Vector Capacity
  bool empty();  // чек пустого контейнера
  size_type size();  // возвращает размер кол-ва элементов
  size_type max_size();  // возвращает максимально возможный размер
  void reserve(size_type size);  // выделяет хранилище для хранения элементов
                                 // массива к новому массиву
  size_type capacity();  // возвращает размер кол-ва элементов которые могу
                         // храниться в выделенном хранилище
  void shrink_to_fit();  // уменьшает размер выделенной памяти освобождая
                         // неиспользованную

  // Vector Modifiers
  void clear();  // чистит контейнер
  iterator insert(
      iterator pos,
      const_reference value);  // добавляет значение к конкретной позиции
  void erase(iterator pos);  // чистит элемент на определенном индексе
  void push_back(const_reference value);  // добавляет к концу вектора элемент
  void pop_back();  // удаляет последний в векторе элемент
  void swap(
      vector& other);  // меняет содержимое вектора с содержимым другого вектора

  // methods for test

 private:
  T* data_;             // массив данных
  size_type size_;      // размер вложенных элементов
  size_type capacity_;  // размер массива (его максимум)
};

}  // namespace s21

#include "s21_vector_capacity.tpp"
#include "s21_vector_elem_access.tpp"
#include "s21_vector_membfuncs.tpp"
#include "s21_vector_modifiers.tpp"

#endif  // S21_VECTOR_H_
