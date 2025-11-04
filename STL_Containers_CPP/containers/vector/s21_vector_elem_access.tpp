#ifndef _S21_VECTOR_2_TPP
#define _S21_VECTOR_2_TPP

namespace s21 {
// доступ к элементу по индексу с проверкой на выход за пределами
template <typename T>
typename vector<T>::reference vector<T>::at(vector<T>::size_type pos) {
  if (pos >= size_) {
    throw std::out_of_range(
        "vector::at: index out of range");  // вместо noexcept обработчик
                                            // исключений out_of_range
  }
  return data_[pos];
};
// доступ к элементу по индексу без предусловий
template <typename T>
typename vector<T>::reference vector<T>::operator[](vector<T>::size_type pos) {
  return data_[pos];
};

// доступ на первый элемент вектора если он не пустой
template <typename T>
typename vector<T>::const_reference vector<T>::front() {
  if (size_ == 0) {
    throw std::runtime_error("vector::front: container is empty");
  }
  return data_[0];
};

// доступ на последний элемент если не пустой
template <typename T>
typename vector<T>::const_reference vector<T>::back() {
  if (size_ == 0) {
    throw std::runtime_error("vector::back: container is empty");
  }
  return data_[size_ - 1];
};

// доступ на массив
template <typename T>
typename vector<T>::value_type* vector<T>::data() {
  return data_;
};

}  // namespace s21

#endif