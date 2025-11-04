#ifndef _S21_test_vector_
#define _S21_test_vector_

namespace s21 {
// проверка на пустоту
template <typename T>
bool vector<T>::empty() {
  return size_ == 0;
}
// чекинг размера
template <typename T>
typename vector<T>::size_type vector<T>::size() {
  return size_;
}

// вызов метода numeric_limits::max() для вывода максимума для вектора
template <typename T>
typename vector<T>::size_type vector<T>::max_size() {
  return std::numeric_limits<std::size_t>::max() / sizeof(value_type);
}

// выделение хранилища для массива к новой области памяти
template <typename T>
void vector<T>::reserve(size_type size) {
  if (size <= capacity_) {  // если запрашиваемый размер меньше текущей ёмкости,
                            // ничего не делаем
    return;
  }
  if (size > max_size()) {  // проверка на максимальный размер
    throw std::out_of_range("ReserveError: Too large size for a new capacity");
  }

  // Выделяем новую память через new[]
  T* new_data = new T[size];

  try {
    // Переносим существующие элементы в новую память
    if constexpr (std::is_nothrow_move_constructible_v<T>) {
      for (size_t i = 0; i < size_; ++i) {
        new_data[i] = std::move(data_[i]);
      }
    } else {
      for (size_t i = 0; i < size_; ++i) {
        new_data[i] = data_[i];
      }
    }
  } catch (...) {
    delete[] new_data;  // если исключение, освобождаем выделенное
    throw;
  }

  // Освобождаем старую память через delete[]
  delete[] data_;

  data_ = new_data;  // присваиваем новый указатель и обновляем ёмкость
  capacity_ = size;
}

// размер массива вектора
template <typename T>
typename vector<T>::size_type vector<T>::capacity() {
  return capacity_;
}

template <typename T>
void vector<T>::shrink_to_fit() {
  if (size_ < capacity_) {  // если текущий размер не заполнен до максимума
    iterator new_data = new value_type[size_];
    std::copy(begin(), end(), new_data);
    delete[] data_;
    data_ = new_data;
    capacity_ = size_;  // то текущий размер стал максимумом
  }
}

// итератор к началу
template <typename T>
typename vector<T>::iterator vector<T>::begin() {
  return data_;
}
// итератор к концу
template <typename T>
typename vector<T>::iterator vector<T>::end() {
  return data_ + size_;
}

// перенос методов в s21_vector_elem_access.tpp
}  // namespace s21

#endif