#ifndef S21_VECTOR_TPP_
#define S21_VECTOR_TPP_

namespace s21 {

template <typename T>  // Конструктор пустого вектора
vector<T>::vector() : data_(nullptr), size_(0), capacity_(0) {}

template <typename T>  // Конструктор вектора с параметром фиксированного
                       // размера, обнуляя элементы
vector<T>::vector(size_type n) : size_(n), capacity_(n) {
  if (n > 0) {
    this->data_ = new typename vector<T>::value_type[n]{};
    // скобки {} или () будут устанавливать элементы нулями по умолчанию
  } else {
    this->data_ = nullptr;
  }
}
// к примеру для ясности можно объявить через круглые скобки ()
// где первым аргументом размер как в предыдущем конструкторе
// и второй аргумент если есть то значение в каждом элементе -----====-----
// а через инит лист можно в фигурные скобки {}аля uniform initialization
//(где просто вкладываешь значения по списку не думая о введении размера
template <typename T>
vector<T>::vector(
    std::initializer_list<typename vector<T>::value_type> const& items)
    : size_(items.size()), capacity_(items.size()) {
  // learn.microsoft.com/ru-ru/cpp/standard-library/initializer-list-class?view=msvc-170
  if (this->size_ > 0) {
    this->data_ = new value_type[items.size()];  // здесь вычисляется размер
    std::copy(items.begin(), items.end(), this->data_);
  } else {
    // использование класса initializer_list который определяется установкой
    // через фигурные скобки
    data_ = nullptr;
  }
}

template <typename T>  // Конструктор копирования
vector<T>::vector(const vector& v) : size_(v.size_), capacity_(v.capacity_) {
  if (v.size_ == 0 || v.capacity_ == 0) {
    data_ = nullptr;
  } else if (capacity_ > 0) {
    data_ = new value_type[v.capacity_];
    std::copy(v.data_, v.data_ + v.size_, data_);
  }
}

template <typename T>
// Конструктор перемещения ---- (move _Tp&& __t) noexcept по cppreference.com
vector<T>::vector(vector&& v) noexcept
    : data_(v.data_), size_(v.size_), capacity_(v.capacity_) {
  // порядок как в заголовке для отключения предупреждений
  v.data_ = nullptr;
  v.size_ = 0;
  v.capacity_ = 0;
}

template <typename T>
// Оператор присваивания с перемещением
vector<T>& vector<T>::operator=(vector&& v) noexcept {
  if (this !=
      &v) {  // проверка на то не пытается ли вектор перемещать на самого себя
    delete[] data_;
    data_ = v.data_;
    size_ = v.size_;
    capacity_ = v.capacity_;

    v.data_ = nullptr;
    v.size_ = 0;
    v.capacity_ = 0;
  }
  return *this;
}

template <typename T>  // Деструктор вектора
vector<T>::~vector() {
  delete[] this->data_;
}

}  // namespace s21

#endif  // S21_VECTOR_TPP_
