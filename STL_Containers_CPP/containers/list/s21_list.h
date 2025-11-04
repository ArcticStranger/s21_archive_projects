#ifndef _S21_LIST_H
#define _S21_LIST_H

#include <algorithm>         // std move copy методы
#include <cstddef>           // size_t
#include <initializer_list>  // initializer_list для конструктора
#include <iterator>
/*
  с <iterator> берем bidirectional_iterator_tag
  - тэг для двунаправленности вперед (++ ) и назад(--)
  в ListIterator и ListConstIterator
*/
#include <limits>     // numeric_limits(max)
#include <stdexcept>  // out_of_range
#include <utility>    // swap

namespace s21 {
template <typename T>
class list {
 private:
  /*
  ---- BEGINLINE NODE INIT ----
  */
  struct Node {  // структура узла двусявзного
    T data;      // данные
    Node* next;  // указатель к следующему элементу-узлу
    Node* prev;  // указатель к предыдущему
    // конструктор узла с назначением value в data
    Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
    // конструктор узла для вставки границ конца самого List
    Node() : next(nullptr), prev(nullptr) {}
  };

  Node* create_node(const T& value) {
    // выденление памяти для узла с инициалзиацией значения
    Node* new_node = new Node(value);
    return new_node;
  }
  // удаление узла
  void delete_node(Node* node) { delete node; }

  /*
---- ENDLINE NODE INIT ----
*/

  /*
------=------ BEGINLINE LISTITERATOR INIT ------=------
*/
 public:
  class ListIterator {  // класс итератора
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type =
        std::ptrdiff_t;  // ptrdiff_t аналог typedef difference_type
    using pointer = T*;
    using reference = T&;

    ListIterator(Node* ptr) : current_ptr(ptr) {}  // Constructor
    T& operator*() { return current_ptr->data; }   // Разыменование
    const T& operator*() const {
      return current_ptr->data;
    }  // Конст разыменование

    T* operator->() const { return &(current_ptr->data); }  // Дать доступ
    // const T* operator->() const { return &(current_ptr->data); }

    ListIterator& operator++() {
      current_ptr = current_ptr->next;
      return *this;
    }  // префиксный инкремент
    ListIterator operator++(int) {
      ListIterator tmp = *this;
      current_ptr = current_ptr->next;
      return tmp;
    }  // постфиксный инкремент

    ListIterator& operator--() {
      current_ptr = current_ptr->prev;
      return *this;
    }  // Префиксный декремент
    ListIterator operator--(int) {
      ListIterator tmp = *this;
      current_ptr = current_ptr->prev;
      return tmp;
    }  // Постфиксный декремент

    bool operator==(const ListIterator& other) const {
      return current_ptr == other.current_ptr;
    }  // сравнение итераторов
    bool operator!=(const ListIterator& other) const {
      return !(*this == other);
    }

    friend class list<T>;

   private:
    Node* current_ptr;
  };

  /*
------=------ ENDLINE LISTITERATOR INIT ------=------
*/

  /*
------=------ BEGINLINE CONST_LISTITERATOR INIT ------=------
*/
  class ListConstIterator {  // класс константного итератора
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    ListConstIterator(const Node* ptr) : current_ptr(ptr) {}
    ListConstIterator(const ListIterator& it) : current_ptr(it.current_ptr) {}
    const T& operator*() const { return current_ptr->data; }
    const T* operator->() const { return &(current_ptr->data); }
    ListConstIterator& operator++() {
      current_ptr = current_ptr->next;
      return *this;
    }
    ListConstIterator operator++(int) {
      ListConstIterator tmp = *this;
      current_ptr = current_ptr->next;
      return tmp;
    }
    ListConstIterator& operator--() {
      current_ptr = current_ptr->prev;
      return *this;
    }
    ListConstIterator operator--(int) {
      ListConstIterator tmp = *this;
      current_ptr = current_ptr->prev;
      return tmp;
    }
    bool operator==(const ListConstIterator& other) const {
      return current_ptr == other.current_ptr;
    }
    bool operator!=(const ListConstIterator& other) const {
      return !(current_ptr == other.current_ptr);
    }

    friend class list<T>;
    friend class ListIterator;

   private:
    Node* current_ptr;
  };

  /*
------=------ ENDLINE CONST_LISTITERATOR INIT ------=------
*/

  // member types
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using iterator = ListIterator;
  using const_iterator = ListConstIterator;
  using size_type = std::size_t;

 private:
  // ----- ПОЛЯ СПИСКА ------
  Node* sentry;  // for checking empty list (p.s:сам пишу на английском for some
                 // practice)
  /* sentry->next в не пустом списке указывает на первый элемент
     sentry->prev указывает на последний элемент
  */
  std::size_t list_size;

 public:
  // constructs, destr, assign
  list();             // пустой список
  list(size_type n);  // параметризованный список
  list(std::initializer_list<value_type> const& items);  // инит лист список
  list(const list& l);  // копирующий список
  list(list&& l);       // перемещение списка
  ~list();              // деструктор списка
  list& operator=(list&& l) noexcept;  // оператор присваивания с перемещением

  // public methods for private elements
  const_reference front();  // к первому элементу доступ
  const_reference back();   // к последнему

  // public methods for iterators
  iterator begin() const;  // итератор к началу
  iterator end() const;    // итератор к концу

  // public methods for capacity
  bool empty();          // проверка на пустоту
  size_type size();      // вывод размера
  size_type max_size();  // вывод максимума

  // public methods for edit container list
  void clear();  // очистка контейнера
  iterator insert(iterator pos, const_reference value);  // вставка по позиции
  void erase(iterator pos);  // очистка элемента по позиции
  void push_back(const_reference value);  // вставка к концу
  void pop_back();  // удаление с конца списка
  void push_front(const_reference value);  // добавление к началу
  void pop_front();  // удаление с начала списка
  void swap(list& other);  // смена содержимого между двумя списками
  void merge(list& other);  // слияние двух списков
  void splice(const_iterator pos,
              list& other);  // перенос элементов со списка к другому списку
                             // начиная с pos
  void reverse();            // смена порядка элементов
  void unique();             // удаление дубликатов
  void sort();               // соритровка элементов
};

}  // namespace s21

#include "s21_elemaccess_it_list.tpp"
#include "s21_list_capacity.tpp"
#include "s21_list_edit.tpp"
#include "s21_list_funcs.tpp"

#endif